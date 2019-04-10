#include <algorithm>
using std::min;
using std::max;

#include <boost/make_shared.hpp>

#include <QtCore/QTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QGridLayout>

#include <osg/MatrixTransform>
#include <osg/Point>
#include <osg/Texture1D>
#include <osg/Texture2D>
#include <osg/TextureRectangle>
#include <osg/Version>

#include <osgDB/ReadFile>

#include <osgGA/NodeTrackerManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/UFOManipulator>
#include <osgGA/FirstPersonManipulator>

#include <osgUtil/Statistics>

#include <osgViewer/Renderer>
#include <osgViewer/Viewer>

#include <osgViewer/ViewerEventHandlers>
#include <CameraUpdateCallback.h>
#include <Globals.h>
#include <MyGraphicsWindowQt.h>
#include <hud.h>
#include <viewingWindowQt.h>

#include <iomanip>

using namespace::boost::asio;

int INVISIBLE_MASK = 0x01;

ViewingWindowQt::ViewingWindowQt( int nCameras, std::vector< GraphWidget* > g,
                                  osg::ArgumentParser& arguments, Scene_Model* model ) :
	QObject( ), _model( model ), osgViewer::Viewer( arguments ), widgets( 0 ), numCameras( 1 ),
	_fov( 90.f ), _fov_offset( 45.0f ), _rotateCamera( true ),
	_initiated( false ), _x_center_distorts_y( false ),
	_x_focal_length( 1 ), _y_focal_length( 1 ),
	_use_distortion( 1 ), _clearColor( osg::Vec4( 0.f, 0.f, 0.f, 1.f ) ),
	_bottom_texcoord( 1 ), _indicator( 0 ), _indicatorSize( 100 ),
	_indicatorBorderSize( 20 ), _indicatorOn( 1 ), _hud( 0 ), _buffer_samples( 60 ),
	_avg_frame_buffer( _buffer_samples ), _averageFrames( false ), _port( 0 ), _graph( g ),
	_packFrames( false ), _last_output_time( -10000.0 ), _output_rate( 1.f / 60.f ),
	_output_treadmill_data( false ), _use_reduced_output( true ), _custom_widget_enabled( false ),
	_use_custom_frustum( false ), _frustum( 1 ), _screen_type(Flat), _brightness_adjustment_enabled ( false )
{
	_x_focal_length[0] = 1.0f;
	_y_focal_length[0] = 1.0f;
	_bottom_texcoord[0] = osg::Vec2( -0.5f, -0.5f );
	_use_distortion[0] = true;

	_keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

	_keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
	_keyswitchManipulator->addMatrixManipulator( '2', "UFO", new osgGA::UFOManipulator() );

	osgGA::FirstPersonManipulator * fpm = new osgGA::FirstPersonManipulator();
	fpm->setWheelMovement(5.0);
	_keyswitchManipulator->addMatrixManipulator( '4', "UFO", fpm );

	setCameraManipulator( _keyswitchManipulator.get() );
	addEventHandler( new osgViewer::StatsHandler );

	_indicator = new osg::Switch;
	_childNum = 0;

	for ( int i = 0; i < nCameras; i++ )
	{
		_graph[i]->set_viewer( this );
		_globalBrightness.push_back( osg::Vec4( 1.f, 1.f, 1.f, 1.f ) );
	}

	_swapBuffers = boost::make_shared< bool >( true );
	setSceneData( _model->scene_root() );
	averageFrames();
}

ViewingWindowQt::~ViewingWindowQt()
{
   _timer.stop();
   disconnect( &_timer );
   stopThreading();

   for ( int i = 0; i < numCameras; i++ )
      widgets[ i ]->hide();

   delete [] widgets;

   if ( _port )
	   delete _port;
}

std::ostream& ViewingWindowQt::reportCameraConfiguration( std::ostream& os ) 
{
	os << "# Number of cameras = " << numCameras << std::endl;
	for (int c = 0; c < numCameras; c++) 
	{
		os << "#  Camera " << c+1 << " width = " << _width[c] <<  " pixels" << std::endl;
		os << "#  Camera " << c+1 << " height = " << _height[c] <<  " pixels" << std::endl;
		os << "#  Camera " << c+1 << " X focal length = " << _x_focal_length[c] <<  " " << std::endl;
		os << "#  Camera " << c+1 << " Y focal length = " << _y_focal_length[c] <<  " " << std::endl;
		os << "#  Camera " << c+1 << " bottom texture coordinate = (";
			os << _bottom_texcoord[c].x() << ", ";
			os << _bottom_texcoord[c].y();
			os <<  ") NDCs" << std::endl;
		os << "#  Camera " << c+1 << " warping is " << (_use_distortion[c] ? "ON" : "OFF") << std::endl;
		os << "#  Camera " << c+1 << " frustum = [";
			os << "left=" << _frustum[c].left << ", ";
			os << "right=" << _frustum[c].right << ", ";
			os << "bottom=" << _frustum[c].bottom << ", ";
			os << "top=" << _frustum[c].top << ", ";
			os << "near=" << _frustum[c].near_ << ", ";
			os << "far=" << _frustum[c].far_ << ", ";
			os << "aperture=" << _frustum[c].aperture << " radians, ";
			os << "distance=" << _frustum[c].distance << ", ";
			os << "screen_distance=" << _frustum[c].screen_distance << ", ";
			os << "eye_sep=" << _frustum[c].eye_sep << ", ";
			os << "shift_left=" << (_frustum[c].shift_left ? "TRUE" : "FALSE") << ", ";
			os << "use_matrix=" << (_frustum[c].use_matrix ? "TRUE" : "FALSE");
			os << "]" << std::endl;
		os << "#  Camera " << c+1 << " matrix = [";
			os << _frustum[c].mat;
			os << "]" << std::endl;
	}
	return os;
}

void
ViewingWindowQt::initializeCameras( int nCameras, int num_displays, int starting_display )
{
   numCameras = nCameras;
   _whichScreen = numCameras - 1;

   _width.resize( numCameras );
   _height.resize( numCameras );

   widgets = new QWidget*[ numCameras ];

   _x_focal_length.resize( numCameras );
   _y_focal_length.resize( numCameras );
   _use_distortion.resize( numCameras );
   _bottom_texcoord.resize( numCameras );
   _frustum.resize( numCameras );
   _screen_geometry.resize(numCameras);

   _screenNum = starting_display;

   _total_width = 0;

   for ( int i = 0; i < numCameras; i++ )
   {
	   _x_focal_length[i] = 1.0f;
	   _y_focal_length[i] = 1.0f;
	   _bottom_texcoord[i] = osg::Vec2( -0.5f, -0.5f );
	   _use_distortion[i] = true;
	   widgets[ i ] = addViewWidget( multipleWindowWithDistortion( i, numCameras, num_displays, starting_display ) );

	   _total_width += _width[ i ];
   }

   getCamera()->setProjectionMatrixAsPerspective( _fov, 1.f, 0.1, 10000.0 );

   for ( int i = 0; i < numCameras; i++ )
      widgets[ i ]->show();

   connect( &_timer, SIGNAL( timeout() ), this, SLOT( paintEvent() ) );

   QDesktopWidget desktopWidget;
   QRect r( desktopWidget.screenGeometry( starting_display ) );
   _indicatorPos = osg::Vec2( r.right() - numCameras*_indicatorSize, r.top() );

   // Create a lighting Shader
   _timer.start( 1 );
}

osg::Node*
ViewingWindowQt::getSceneRoot()
{
	return _model->current_scene();
}

void
ViewingWindowQt::selectScene( int id )
{
	_model->select_scene( id );
}

void
ViewingWindowQt::setSerialPort( std::string port )
{
	try
	{
	// what baud rate do we communicate at
		serial_port_base::baud_rate baud( 115200 );
	// how big is each "packet" of data (default is 8 bits)
		serial_port_base::character_size csize( 8 );
	// what flow control is used (default is none)
		serial_port_base::flow_control flow( serial_port_base::flow_control::none );
	// what parity is used (default is none)
		serial_port_base::parity parity( serial_port_base::parity::none );
	// how many stop bits are used (default is one)
		serial_port_base::stop_bits stop( serial_port_base::stop_bits::one );

		_port = new serial_port( _io, port );
		_port->set_option( baud );
		_port->set_option( csize );
		_port->set_option( flow );
		_port->set_option( parity );
		_port->set_option( stop );
	}
	catch (...)
	{
		clearSerialPort();
		std::cout << "Unable to set serial port to\"" << port << "\"" << std::endl;
	}
}

// Outputs an event string. Prepends a time stamp, appends a newline
void
ViewingWindowQt::outputEvent( std::ostringstream const& eventString,
                              std::ostringstream const& floatEventString,
                              bool force_write,
                              double time_stamp ) const
{
	double refTime;
	if ( time_stamp > 0 )
		refTime = time_stamp;
	else
		refTime = _currSimTime;

	std::ostringstream command, float_command;

	// Set precision of time stamp
	float_command.setf(std::ios::fixed, std::ios::floatfield);
	float_command << std::setprecision(5); // to hundredths of a millisecond
	float_command << refTime - _startRefTime << floatEventString.str() << std::endl;
	_model->output_event( float_command );

	// Complicated accounting to better hit average output rate TODO - not working yet
	static double accumulated_lateness = 0; // to help create correct average output rate
	double time_since_deadline = (refTime - _last_output_time) - (_output_rate - accumulated_lateness);
	// double time_since_deadline = (refTime - _last_output_time) - (_output_rate);
	if (time_since_deadline >= 0) {
		accumulated_lateness = time_since_deadline;
		if (accumulated_lateness > _output_rate) // we are way behind
			accumulated_lateness = _output_rate; // don't let value blow up
		if (accumulated_lateness < 0) // should not happen, but just in case...
			accumulated_lateness = 0;
	}
	if ( force_write || ( time_since_deadline >= 0 ) )
	{
		_last_output_time = refTime;
		command << (int)( ( refTime - _startRefTime ) * 1000.0 )
		        << eventString.str() << std::endl;

		if ( _port )
			write( *_port, buffer( command.str().c_str(),command.str().size() ) );
		else
		{
			std::cout << command.str();
			std::cout.flush();
		}
	}
}

void
ViewingWindowQt::myRenderingTraversals()
{
    bool _outputMasterCameraLocation = false;
    if (_outputMasterCameraLocation)
    {
        Views views;
        getViews(views);

        for(Views::iterator itr = views.begin();
            itr != views.end();
            ++itr)
        {
            osgViewer::View* view = *itr;
            if (view)
            {
                // const osg::Matrixd& m = view->getCamera()->getInverseViewMatrix();
                //OSG_NOTICE<<"View "<<view<<", Master Camera position("<<m.getTrans()<<"), rotation("<<m.getRotate()<<")"<<std::endl;
            }
        }
    }

    Contexts contexts;
    getContexts(contexts);

    // check to see if windows are still valid
#if OSG_VERSION_LESS_THAN( 3, 0, 0 )
    checkWindowStatus();
#else
    checkWindowStatus(contexts);
#endif
    if (_done) return;

    double beginRenderingTraversals = elapsedTime();

    osg::FrameStamp* frameStamp = getViewerFrameStamp();

    if (getViewerStats() && getViewerStats()->collectStats("scene"))
    {
        unsigned int frameNumber = frameStamp ? frameStamp->getFrameNumber() : 0;

        Views views;
        getViews(views);
        for(Views::iterator vitr = views.begin();
            vitr != views.end();
            ++vitr)
        {
            View* view = *vitr;
            osg::Stats* stats = view->getStats();
            osg::Node* sceneRoot = view->getSceneData();
            if (sceneRoot && stats)
            {
                osgUtil::StatsVisitor statsVisitor;
                sceneRoot->accept(statsVisitor);
                statsVisitor.totalUpStats();

                unsigned int unique_primitives = 0;
                osgUtil::Statistics::PrimitiveCountMap::iterator pcmitr;
                for(pcmitr = statsVisitor._uniqueStats.GetPrimitivesBegin();
                    pcmitr != statsVisitor._uniqueStats.GetPrimitivesEnd();
                    ++pcmitr)
                {
                    unique_primitives += pcmitr->second;
                }

                stats->setAttribute(frameNumber, "Number of unique StateSet", static_cast<double>(statsVisitor._statesetSet.size()));
                stats->setAttribute(frameNumber, "Number of unique Group", static_cast<double>(statsVisitor._groupSet.size()));
                stats->setAttribute(frameNumber, "Number of unique Transform", static_cast<double>(statsVisitor._transformSet.size()));
                stats->setAttribute(frameNumber, "Number of unique LOD", static_cast<double>(statsVisitor._lodSet.size()));
                stats->setAttribute(frameNumber, "Number of unique Switch", static_cast<double>(statsVisitor._switchSet.size()));
                stats->setAttribute(frameNumber, "Number of unique Geode", static_cast<double>(statsVisitor._geodeSet.size()));
                stats->setAttribute(frameNumber, "Number of unique Drawable", static_cast<double>(statsVisitor._drawableSet.size()));
                stats->setAttribute(frameNumber, "Number of unique Geometry", static_cast<double>(statsVisitor._geometrySet.size()));
                stats->setAttribute(frameNumber, "Number of unique Vertices", static_cast<double>(statsVisitor._uniqueStats._vertexCount));
                stats->setAttribute(frameNumber, "Number of unique Primitives", static_cast<double>(unique_primitives));

                unsigned int instanced_primitives = 0;
                for(pcmitr = statsVisitor._instancedStats.GetPrimitivesBegin();
                    pcmitr != statsVisitor._instancedStats.GetPrimitivesEnd();
                    ++pcmitr)
                {
                    instanced_primitives += pcmitr->second;
                }

                stats->setAttribute(frameNumber, "Number of instanced Stateset", static_cast<double>(statsVisitor._numInstancedStateSet));
                stats->setAttribute(frameNumber, "Number of instanced Group", static_cast<double>(statsVisitor._numInstancedGroup));
                stats->setAttribute(frameNumber, "Number of instanced Transform", static_cast<double>(statsVisitor._numInstancedTransform));
                stats->setAttribute(frameNumber, "Number of instanced LOD", static_cast<double>(statsVisitor._numInstancedLOD));
                stats->setAttribute(frameNumber, "Number of instanced Switch", static_cast<double>(statsVisitor._numInstancedSwitch));
                stats->setAttribute(frameNumber, "Number of instanced Geode", static_cast<double>(statsVisitor._numInstancedGeode));
                stats->setAttribute(frameNumber, "Number of instanced Drawable", static_cast<double>(statsVisitor._numInstancedDrawable));
                stats->setAttribute(frameNumber, "Number of instanced Geometry", static_cast<double>(statsVisitor._numInstancedGeometry));
                stats->setAttribute(frameNumber, "Number of instanced Vertices", static_cast<double>(statsVisitor._instancedStats._vertexCount));
                stats->setAttribute(frameNumber, "Number of instanced Primitives", static_cast<double>(instanced_primitives));
           }
        }
    }

    osgViewer::ViewerBase::Scenes scenes;
    getScenes(scenes);

    for(osgViewer::ViewerBase::Scenes::iterator sitr = scenes.begin();
        sitr != scenes.end();
        ++sitr)
    {
	    osgViewer::Scene* scene = *sitr;
	    osgDB::DatabasePager* dp = scene ? scene->getDatabasePager() : 0;
	    if (dp)
	    {
		    dp->signalBeginFrame(frameStamp);
	    }

	    if (scene->getSceneData())
	    {
		    // fire off a build of the bounding volumes while we
		    // are still running single threaded.
		    scene->getSceneData()->getBound();
	    }
    }

    // OSG_NOTICE<<std::endl<<"Start frame"<<std::endl;


    osgViewer::ViewerBase::Cameras cameras;
    getCameras(cameras);

    osgViewer::ViewerBase::Contexts::iterator itr;

    bool doneMakeCurrentInThisThread = false;

    if (_endDynamicDrawBlock.valid())
    {
        _endDynamicDrawBlock->reset();
    }

    // dispatch the rendering threads
    if (_startRenderingBarrier.valid()) _startRenderingBarrier->block();

    // reset any double buffer graphics objects
    for(osgViewer::ViewerBase::Cameras::iterator camItr = cameras.begin();
        camItr != cameras.end();
        ++camItr)
    {
        osg::Camera* camera = *camItr;
        osgViewer::Renderer* renderer = dynamic_cast<osgViewer::Renderer*>(camera->getRenderer());
        if (renderer)
        {
            if (!renderer->getGraphicsThreadDoesCull() && !(camera->getCameraThread()))
            {
                renderer->cull();
            }
        }
    }

    for(itr = contexts.begin();
        itr != contexts.end();
        ++itr)
    {
        if (_done) return;
        if (!((*itr)->getGraphicsThread()) && (*itr)->valid())
        {
            doneMakeCurrentInThisThread = true;
            makeCurrent(*itr);
            (*itr)->runOperations();
        }
    }

    // OSG_NOTICE<<"Joing _endRenderingDispatchBarrier block "<<_endRenderingDispatchBarrier.get()<<std::endl;

    // wait till the rendering dispatch is done.
    if (_endRenderingDispatchBarrier.valid()) _endRenderingDispatchBarrier->block();

    for(itr = contexts.begin();
        itr != contexts.end();
        ++itr)
    {
        if (_done) return;

        if (!((*itr)->getGraphicsThread()) && (*itr)->valid())
        {
            doneMakeCurrentInThisThread = true;
            makeCurrent(*itr);
            if ( *_swapBuffers )
	            (*itr)->swapBuffers();
        }
    }

    for(osgViewer::ViewerBase::Scenes::iterator sitr = scenes.begin();
        sitr != scenes.end();
        ++sitr)
    {
	    osgViewer::Scene* scene = *sitr;
        osgDB::DatabasePager* dp = scene ? scene->getDatabasePager() : 0;
        if (dp)
        {
            dp->signalEndFrame();
        }
    }

    // wait till the dynamic draw is complete.
    if (_endDynamicDrawBlock.valid())
    {
        // osg::Timer_t startTick = osg::Timer::instance()->tick();
        _endDynamicDrawBlock->block();
        // OSG_NOTICE<<"Time waiting "<<osg::Timer::instance()->delta_m(startTick, osg::Timer::instance()->tick())<<std::endl;;
    }

    if (_releaseContextAtEndOfFrameHint && doneMakeCurrentInThisThread)
    {
        //OSG_NOTICE<<"Doing release context"<<std::endl;
        releaseContext();
    }

    if (getViewerStats() && getViewerStats()->collectStats("update"))
    {
        double endRenderingTraversals = elapsedTime();

        // update current frames stats
        getViewerStats()->setAttribute(frameStamp->getFrameNumber(), "Rendering traversals begin time ", beginRenderingTraversals);
        getViewerStats()->setAttribute(frameStamp->getFrameNumber(), "Rendering traversals end time ", endRenderingTraversals);
        getViewerStats()->setAttribute(frameStamp->getFrameNumber(), "Rendering traversals time taken", endRenderingTraversals-beginRenderingTraversals);
    }

    _requestRedraw = false;
}


void
ViewingWindowQt::paintEvent()
{
	static clock_t beginTime = clock();
	double frameTime;
	osg::Vec3d eye, center, up;

	if ( _packFrames )
		*_swapBuffers = false;

	double refTime = getFrameStamp()->getReferenceTime();
	frameTime = refTime - _lastRefTime;

	int frame_count = _packFrames ? 3 : 1;
	// Frame rate is already predivided by the frame count
	float frame_rate = _model->average_framerate();

	for ( int i = 0; i < frame_count; i++ )
	{
		if ( _initiated )
		{
			getCamera()->getViewMatrixAsLookAt( eye, center, up );
			_model->set_eye_position( center );
			_model->reset_traversal();
		}

		if ( _packFrames )
			for ( int j = 0; j < numCameras; j++ )
				_frameSwitch[ j ]->setSingleChildOn( i );

		advance();
		eventTraversal();
		updateTraversal();
		if ( _packFrames && i == 2 )
			*_swapBuffers = true;


		if ( _model->physics_enabled() )
		{
			_currSimTime = _prevSimTime + frame_rate;
			_model->step_simulation( _currSimTime - _prevSimTime,
			                         3, frameTime / (double)(6 * frame_count));
			_prevSimTime = _currSimTime;
		}

		myRenderingTraversals();


		if ( _initiated )
		{
			osg::Vec3d dir = center - eye;
			osg::Vec3d x_axis( 1, 0, 0 );
			dir.normalize();
			osg::Vec3d cross = dir ^ x_axis;
			int sign = ( up * cross ) < 0 ? 1 : -1;
			double cos_theta = dir * x_axis;
			double speed;
			btVector3 body_center = _model->camera_position();
//			btVector3 tv = center - _lastPosition;
//			std::cout << "center - <" << center.x() << ", " << center.y() << ", " << center.z() << ">" << std::endl;
//			std::cout << "_lastPosition - <" << _lastPosition.x() << ", " << _lastPosition.y() << ", " << _lastPosition.z() << ">" << std::endl;
//			std::cout << "tv - <" << tv.x() << ", " << tv.y() << ", " << tv.z() << ">" << std::endl;
//			std::cout << "dist - " << tv.length() << " " << frameTime << std::endl;
			if ( !_model->motion_disabled() )
			{
				speed = ( (body_center - _lastPosition).length() ) / frameTime;
				speed = speed < 5.e-4 ? 0 : speed;
			}
			else
			{
				CameraUpdateCallback* callback = dynamic_cast<CameraUpdateCallback*>( getCamera()->getUpdateCallback() );

				osg::Vec3d const& vec = callback->displacement();
				speed = vec.length() / frameTime;
				callback->clear();
			}

			_lastPosition = body_center;
			center.x() = body_center.x() - _model->scene_bound()->xMin();
			center.y() = body_center.y() - _model->scene_bound()->yMin();
			Motion_Data* md = (Motion_Data*)_model->tracker()->getUserData();

			osg::Vec3 pCenter = _model->player_location_in_world();

			std::ostringstream command, float_command;
			if ( _use_reduced_output ) {
				// command << "," << (int)(pCenter.x() * 100.f) << "," << (int)(pCenter.y() * 100.f);
			}
			else {
				command << "," << (int)(pCenter.x() * 100.f) << "," << (int)(pCenter.y() * 100.f) << ","
				        << (int)(pCenter.z() * 100) << ","
				        << (int)( speed * 100.0 ) << ","
				        << (int)( sign * ( 18000.0 / M_PI ) * acos( cos_theta ) );
			}
			float_command << "," << pCenter.x() << "," << pCenter.y() << "," << pCenter.z() << ","
			              << speed << ","
			              << sign * ( 180.0 / M_PI ) * acos( cos_theta );

			if ( _output_treadmill_data ) // Dec 2015 only applies to COM output
			{
				if ( !_use_reduced_output )
					command << "," << (int)md->raw.x() << "," << (int)md->raw.y() << ","
					        << (int)md->raw.z() << "," << (int)md->raw.w();
			}
			// Dec 2015 always include treadmill data in log file output
			float_command << "," << (int)md->raw.x() << "," << (int)md->raw.y() << ","
				              << (int)md->raw.z() << "," << (int)md->raw.w();

			// Report collisions - 7/21/2015 as "maximum penetration distance"
			// command << _model->contact_count();
			// float_command << _model->contact_count();
			float minDist = md->minObstacleDistance;
			// command << "," << int(-minDist);
			float_command << "," << -minDist; // minus, for "penetration distance"

			// Punishment region
			int reinforcementValue = md->reinforcementValue;
			// float minPunishment = md->minPunishmentDistance;
			// command << "," << int(-minPunishment);
			command << "," << reinforcementValue;
			// float_command << "," << -minPunishment; // minus, for "penetration distance"
			float_command << "," << reinforcementValue;

			if ( _model->contact_count() )
			{
				// Turn off physics CMB
				_model->use_dynamics( false );
				// _model->use_dynamics( true );

				std::vector< osg::Node* > const& contact_nodes( _model->contact_nodes() );

				std::vector< osg::Node* >::const_iterator node_it;
				for( node_it = contact_nodes.begin();
				     node_it != contact_nodes.end(); node_it++ )
				{
					command << "," << (*node_it)->getName();
					float_command << "," << (*node_it)->getName();
				}
			}
			else
				_model->use_dynamics( false );

			outputEvent( command, float_command, false, 
				// _currSimTime 
				getFrameStamp()->getReferenceTime()
				);
		}
	}

	if ( _indicatorOn > 0 )
	{
		_indicator->setSingleChildOn( _childNum + frame_count );
		/*
		clock_t endTime = clock();
		double elapsed_secs = double(endTime - beginTime) / CLOCKS_PER_SEC;
		std::ostream * osp = _model->logStream();
		if (osp != nullptr) {
			int state = _childNum + frame_count;
			*osp << "# indicator state = " << _childNum + frame_count;
			switch(state) {
			case 3:
				*osp << " ( 1 0 1 ) ";
				break;
			case 4:
				*osp << " ( 0 1 0 ) ";
				break;
			default:
				*osp << " ( ? ? ? ) ";
				break;
			}
			*osp << "; elapsed time = " << elapsed_secs << " seconds ";
			*osp << std::endl;
		}
		beginTime = endTime;
		*/
		if ( ( _indicatorOn == 1 && _initiated ) || _indicatorOn == 2 ) {
			_childNum = ( _childNum == 1 ? 0 : 1 );
		}
	}
	else
		_indicator->setAllChildrenOff( );

	_lastRefTime = refTime;

	if ( _averageFrames && frameTime > 0 && frameTime < 1.0 )
	{
		if ( _ignoredFramesCount > 0 )
			_ignoredFramesCount--;
		else
		{
			if ( _avg_frame_buffer.full() )
			{
				// Remove first element from the accumulated value
				_avgFrameTime -= _avg_frame_buffer.front();
				_avg_frame_buffer.push_back( frameTime );
				_avgFrameTime += frameTime;
			}
			else
			{
				// Filling the buffer
				_avg_frame_buffer.push_back( frameTime );
				_avgFrameTime += frameTime;
			}

			_avgFrameCount++;
			if ( _avgFrameCount >= _buffer_samples )
			{
				float swap_frame_rate = _avgFrameTime / _buffer_samples;
				unsigned int hertz = (int) ( 1.f / swap_frame_rate );

				if ( _avg_frame_buffer.capacity() < hertz )
				{
					_avg_frame_buffer.resize( hertz, 0.f );
					_avg_frame_buffer.clear();
					_buffer_samples = hertz;
					_avgFrameTime = 0.f;
				}

				_model->set_average_framerate( swap_frame_rate / (double)frame_count );
				_avgFrameCount = 0;
			}
		}
	}


	if ( _packFrames )
		*_swapBuffers = true;

};

void
ViewingWindowQt::loadOSG( std::string filename )
{
	_model->load_osg( filename );
	setUpSlaves();
}

void
ViewingWindowQt::loadModel( osg::ArgumentParser& arguments )
{
	_model->load_model( arguments );
	setUpSlaves();
}

void
ViewingWindowQt::loadImage( std::string filename, bool flip, bool useTextureRectangle )
{
	_model->load_image( filename, flip, useTextureRectangle );
	setUpSlaves();
}

void
ViewingWindowQt::setDiffusePower( float dPower )
{
	_model->set_diffuse_power( dPower );
}

void
ViewingWindowQt::setDiffuseColor( osg::Vec4 diff )
{
	_model->set_diffuse_color( diff );
}

void
ViewingWindowQt::setAmbientColor( osg::Vec4 amb )
{
	_model->set_ambient_color( amb );
}

void
ViewingWindowQt::loadData( )
{
   setUpSlaves();

   // Needed here to initialize the simulation time
   advance();

	_currSimTime = getFrameStamp()->getSimulationTime();
	_prevSimTime = getFrameStamp()->getSimulationTime();
}

void
ViewingWindowQt::updateViewport( int i, int leftOffset, int rightOffset )
{
   osg::Viewport* vp = getSlave( i )._camera->getViewport();

   vp->setViewport( leftOffset, vp->y(), rightOffset-leftOffset, vp->height() );
}

QWidget*
ViewingWindowQt::addViewWidget( osg::Camera* camera )
{
	if ( _custom_widget_enabled )
	{
		MyGraphicsWindowQt* gw = dynamic_cast<MyGraphicsWindowQt*>( camera->getGraphicsContext() );
		return gw ? gw->getOpenGLWidget() : NULL;
	}
	else
	{
		osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>( camera->getGraphicsContext() );
		return gw ? gw->getGLWidget() : NULL;
	}
}

inline std::ostream& operator<<( std::ostream& output, const osg::Viewport& v )
{
	output << "<"
	       << v.x() << ", "
	       << v.y() << ", "
	       << v.width() << ", "
	       << v.height() << ">";
    return output;     // to enable cascading
}

struct Draw_Callback : public osg::Camera::DrawCallback
{
	Draw_Callback( std::string title ): _title( title ) { }

	virtual void operator () (osg::RenderInfo& renderInfo) const
	{
		osg::Camera* camera = renderInfo.getCurrentCamera();
		osg::Viewport* viewport = camera ? camera->getViewport() : 0;
		double left, right, top, bottom, near_, far_;
		camera->getProjectionMatrixAsFrustum( left, right, bottom, top, near_, far_ );

		osg::notify(osg::NOTICE)<<"Camera projection matrix (" << _title << ") " << camera->getProjectionMatrix() << std::endl;
		osg::notify(osg::NOTICE)<<"Camera frustum (" << _title << ") " << left << " " << right << " " << bottom << " " << top << " " << near_ << " " << far_ << " " << std::endl;
		osg::notify(osg::NOTICE)<<"Camera viewport (" << _title << ") " <<  *viewport << std::endl;
	}

	std::string _title;
};

class CullCallback: public osg::NodeCallback
{
public:

	CullCallback( bool replace_matrix, osg::Matrixd mat, Frustum* f ): _replace_matrix( replace_matrix ), _mat( new osg::RefMatrixd( mat ) ), _frustum( f ) { _mat->ref(); }

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		if ( _replace_matrix )
		{
			osgUtil::CullVisitor* cv = dynamic_cast< osgUtil::CullVisitor* >( nv );
			cv->popProjectionMatrix();
			cv->pushProjectionMatrix( _mat );
			osg::RefMatrixd* mvm = cv->getModelViewMatrix();
			osg::Vec3d eye, center, up, eye_dir, right;
			mvm->getLookAt( eye, center, up, _frustum->screen_distance );

			eye_dir = eye - center;
			eye_dir.normalize();

			right = eye_dir ^ up;
			right.normalize();
			right *= (_frustum->eye_sep/2.);

//			eye += eye_dir * _frustum->distance;
//			center += eye_dir * _frustum->distance;
//			eye += right;
//			center += right;

//			eye_dir = eye - center;
//			eye_dir.normalize();
//			std::cout << _frustum->shift_left << ", <" << eye << ">, <" << center << ">, <" << eye_dir << ">" << std::endl;

//			mvm->makeLookAt( eye, center, up );
//			cv->popModelViewMatrix();
//			cv->pushModelViewMatrix( mvm, osg::Transform::ABSOLUTE_RF );
		}

		traverse( node, nv );
	}

private:
	bool _replace_matrix;
	osg::RefMatrixd* _mat;
	Frustum* _frustum;
};

void
ViewingWindowQt::setUpSlaves()
{
	QDesktopWidget desktopWidget;
	QRect r( desktopWidget.screenGeometry( _screenNum ) );
	_indicator = new osg::Switch;
	_hud = createHUD( _indicator, numCameras, r.left(), r.right(),
	                  r.top(), r.bottom(), _indicatorPos,
	                  _indicatorSize, _indicatorBorderSize );

	_frameSwitch.clear();

	double min_near = 100000, max_far = 0.1;

	for( int i = 0; i < (int)getNumSlaves(); i++ )
	{
		double aspectRatioScale = (float)_width[i] / (float)_height[i];
		double translate_x = double(numCameras)-1 + i * -2.0;
		double theta = 0.0;

		if ( numCameras > 1 )
		{
			// Even number of cameras
			if ( numCameras % 2 == 0 )
				theta = osg::inDegrees(-_fov_offset / 2. ) * numCameras / 2 + i*osg::inDegrees(_fov_offset);
			else
				theta = osg::inDegrees(-_fov_offset) * ( numCameras - 1 )/ 2 + i*osg::inDegrees(_fov_offset);
		}


		osg::Node* data;

		if ( _packFrames )
		{
			osg::Switch* switchy = new osg::Switch;
			switchy->setDataVariance( osg::Object::DYNAMIC );

			for ( int idx = 0; idx < 3; idx++ )
				switchy->addChild( createDistortionSubgraphWithPacking( i, idx,
				                                                           getSceneData(),
				                                                           _clearColor,
				                                                           theta,
				                                                           i == _whichScreen ));
			switchy->setAllChildrenOff();
			_frameSwitch.push_back( switchy );
			data = switchy;
		}
		else
			data = createDistortionSubgraph( i, getSceneData(), _clearColor,
			                                 theta, i == _whichScreen );

		osg::Camera* camera = getSlave( i )._camera;
		int index = i;

		if ( !_use_custom_frustum )
		{
			double left, right, top, bottom, near_, far_;
			camera->getProjectionMatrixAsFrustum( left, right, bottom, top, near_, far_ );
			_frustum[index] = Frustum( left, right, bottom, top, near_, far_ );
		}
		else
		{
			min_near = std::min( min_near, _frustum[index].near_ );
			max_far = std::max( max_far, _frustum[index].far_ );
		}

		std::stringstream sstm;
		sstm << "Slave_" << i;
		camera->setName( sstm.str() );

//		camera->setInitialDrawCallback( new Draw_Callback( "Slave Initial" ) );
//		camera->setFinalDrawCallback( new Draw_Callback( "Slave Final" ) );

		getSlave( i )._camera->removeChild( 0, 1 );
		getSlave( i )._camera->addChild( data );
		if ( _rotateCamera )
		{
			getSlave( i )._viewOffset = osg::Matrix::rotate( theta, 0.0, 1.0, 0.0);
			getSlave( i )._projectionOffset = osg::Matrix::scale( aspectRatioScale, 1.0, 1.0 );
		}
		else
		{
			getSlave( i )._viewOffset = osg::Matrix();
			getSlave( i )._projectionOffset = osg::Matrix::scale(aspectRatioScale, 1.0, 1.0)*
				osg::Matrix::translate(translate_x, 0.0, 0.0);
		}

	}

	if ( !_use_custom_frustum )
	{
			min_near = 0.1;
			max_far = 10000.0;
	}
	else
		getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

	getCamera()->setProjectionMatrixAsPerspective( _fov, (float)_width[0] / (float)_height[0],
	                                               min_near, max_far );
}

void
ViewingWindowQt::setTrackNode()
{
	_keyswitchManipulator->addMatrixManipulator( '3', "NodeTracker", _model->tracker().get() );
}

void
ViewingWindowQt::setUpdateCallback( osg::NodeCallback* callback, bool reset )
{
	getCamera()->setUpdateCallback( callback );
	if ( callback != 0 )
	{
		if ( reset )
		{
			_startRefTime = getFrameStamp()->getReferenceTime();
			_currSimTime = getFrameStamp()->getSimulationTime();
			_prevSimTime = getFrameStamp()->getSimulationTime();
		}

		_lastRefTime = 0.f;
		_lastPosition = _model->camera_position();
		_childNum = 0;
		_initiated = true;
	}
	else
	{
		_childNum = 2;
		_initiated = false;
	}
}

osg::Node*
ViewingWindowQt::createDistortionSubgraph( int index, osg::Node* subgraph,
                                           const osg::Vec4& clearColour,
                                           double theta, bool addHud )
{
	osg::Group* distortionNode = new osg::Group;

	// Type cast the arguments to log as windows (VS2010) whines about ambiguous call
	unsigned int tex_width = (unsigned int) pow( 2, ceil( log( (float)(_traits->width) ) / log( 2.f ) ) );
	unsigned int tex_height = (unsigned int) pow( 2, ceil( log( (float)(_traits->height) ) / log( 2.f ) ) );

	if ( tex_width > tex_height )
		tex_height = tex_width;
	else
		tex_width = tex_height;

	osg::Texture2D* texture = new osg::Texture2D;
	texture->setTextureSize(tex_width, tex_height);
	texture->setInternalFormat(GL_RGBA);
	texture->setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR );
	texture->setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR );

	// set up the render to texture camera 1.
	{
		osg::Camera* camera = new osg::Camera;

		// set clear the color and depth buffer
		camera->setClearColor(clearColour);
		camera->setClearMask(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// just inherit the main cameras view
		camera->setReferenceFrame(osg::Transform::RELATIVE_RF);
		camera->setProjectionMatrix(osg::Matrixd::identity());
		camera->setViewMatrix(osg::Matrixd::identity());

		// set viewport
		camera->setViewport(0,0,tex_width,tex_height);

		// set the camera to render before the main camera.
		camera->setRenderOrder(osg::Camera::PRE_RENDER);

		// tell the camera to use OpenGL frame buffer object where supported.
		camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

		// attach the texture and use it as the color buffer.
		camera->attach(osg::Camera::COLOR_BUFFER, texture);

		// Only render visible objects
		camera->setCullMask( ~INVISIBLE_MASK );

		// add subgraph to render
		camera->addChild(subgraph);

		std::stringstream sstm;
		sstm << "RTT_" << index;
		camera->setName( sstm.str() );

//		camera->setInitialDrawCallback( new Draw_Callback( "RTT Initial" ) );
//		camera->setFinalDrawCallback( new Draw_Callback( "RTT Final" ) );

		osg::Matrixd mat;
		mat.makeIdentity();

		if ( _use_custom_frustum )
		{
			camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF_INHERIT_VIEWPOINT);

			if ( _frustum[index].use_matrix )
				mat = _frustum[index].mat;
			else
				mat.makeFrustum( _frustum[index].left, _frustum[index].right,
				                 _frustum[index].bottom, _frustum[index].top,
				                 _frustum[index].near_, _frustum[index].far_ );

			camera->setProjectionMatrix( mat );
		}

		camera->setCullCallback( new CullCallback( _use_custom_frustum, mat, &( _frustum[index] ) ) );

		distortionNode->addChild(camera);

	}

	// set up the hud camera
	{
		// create the quad to visualize.
		osg::Geometry* polyGeom = new osg::Geometry();

		polyGeom->setSupportsDisplayList(false);

		osg::Vec3 origin(0.0f,0.0f,0.0f);
		osg::Vec3 xAxis(1.0f,0.0f,0.0f);
		osg::Vec3 yAxis(0.0f,1.0f,0.0f);
		float height = _traits->height;
		float width = _traits->width;
		int noSteps = 50;

		osg::Vec3Array* vertices = new osg::Vec3Array;
		osg::Vec2Array* texcoords = new osg::Vec2Array;
		osg::Vec4Array* colors = new osg::Vec4Array;

		osg::Vec3 bottom = origin;
		osg::Vec3 dx = xAxis*(width/((float)(noSteps-1)));
		osg::Vec3 dy = yAxis*(height/((float)(noSteps-1)));

		osg::Vec2 dx_texcoord(1.0f/(float)(noSteps-1),0.0f);
		osg::Vec2 dy_texcoord(0.0f,1.0f/(float)(noSteps-1));

		osg::Vec2 texcoord = _bottom_texcoord[index];
		int i,j;
		osg::Vec2 xy_min( 1, 1 );
		osg::Vec2 xy_max( -1, -1 );

		_graph[index]->compute_basis_matrix();

		for(i=0;i<noSteps;++i)
		{
			//osg::Vec3 cursor = bottom+dy*(float)i;
			osg::Vec2 texcoord = _bottom_texcoord[index]+dy_texcoord*(float)i;
			for(j=0;j<noSteps;++j)
			{
				QPointF pt = _graph[index]->evaluate( texcoord.x() - _bottom_texcoord[index].x(),
				                               1.-(texcoord.y() - _bottom_texcoord[index].y()) );
				osg::Vec3 cursor( width * pt.x(), height - (height * pt.y()), 0. );
				vertices->push_back(cursor);

				if ( _use_distortion[index] )
				{
					float th = texcoord.x();
					float h = texcoord.y() / _y_focal_length[index];
					float xh = sinf( th / _x_focal_length[index] );
					float yh = h;
					float zhx = cosf( th / _x_focal_length[index] );
					float zhy;
					if ( _x_center_distorts_y )
						zhy = cosf( th / _y_focal_length[index] );
					else
						zhy = cosf( (th -  _bottom_texcoord[index].x() - 0.5f) / _y_focal_length[index] );

					osg::Vec2 texcoordh( _x_focal_length[index] * xh / zhx - _bottom_texcoord[index].x(),
					                     _y_focal_length[index] * yh / zhy - _bottom_texcoord[index].y() );

					xy_min.x() = min( xy_min.x(), texcoordh.x() );
					xy_max.x() = max( xy_max.x(), texcoordh.x() );
					xy_min.y() = min( xy_min.y(), texcoordh.y() );
					xy_max.y() = max( xy_max.y(), texcoordh.y() );

					texcoords->push_back( texcoordh );
				}
				else
					texcoords->push_back( osg::Vec2( texcoord.x() - _bottom_texcoord[index].x(),
					                                 texcoord.y() - _bottom_texcoord[index].y() ) );
				colors->push_back( _globalBrightness[ index ] );

//				cursor += dx;
				texcoord += dx_texcoord;
			}
		}

		if ( _use_distortion[index] )
		{
			osg::Vec2 scale( 1.f/(xy_max.x()-xy_min.x()),  1.f/(xy_max.y()-xy_min.y()));

			for ( osg::Vec2Array::iterator it = texcoords->begin(); it < texcoords->end(); it++ )
			{
				it->x() = ( it->x() - xy_min.x() ) * scale.x();
				it->y() = ( it->y() - xy_min.y() ) * scale.y();
			}
		}

		// pass the created vertex array to the points geometry object.
		polyGeom->setVertexArray(vertices);

		polyGeom->setColorArray(colors);
		polyGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		polyGeom->setTexCoordArray(0,texcoords);


		for(i=0;i<noSteps-1;++i)
		{
			osg::DrawElementsUShort* elements = new osg::DrawElementsUShort(osg::PrimitiveSet::QUAD_STRIP);
			for(j=0;j<noSteps;++j)
			{
				elements->push_back(j+(i+1)*noSteps);
				elements->push_back(j+(i)*noSteps);
			}
			polyGeom->addPrimitiveSet(elements);
		}


		// new we need to add the texture to the Drawable, we do so by creating a
		// StateSet to contain the Texture StateAttribute.
		osg::StateSet* stateset = polyGeom->getOrCreateStateSet();
		stateset->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);
		stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

		osg::Geode* geode = new osg::Geode();
		geode->addDrawable(polyGeom);

		// set up the camera to render the textured quad
		osg::Camera* camera = new osg::Camera;

		// just inherit the main cameras view
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setViewMatrix(osg::Matrix::identity());
		camera->setProjectionMatrixAsOrtho2D( 0, _traits->width, 0, _traits->height );

		// set the camera to render before the main camera.
		camera->setRenderOrder(osg::Camera::NESTED_RENDER);

		std::stringstream sstm;
		sstm << "TTS_" << index;
		camera->setName( sstm.str() );

//		camera->setInitialDrawCallback( new Draw_Callback( "Tex Initial" ) );
//		camera->setFinalDrawCallback( new Draw_Callback( "Tex Final" ) );

		// add subgraph to render
		camera->addChild(geode);
		if ( addHud )
			camera->addChild( _hud );

		distortionNode->addChild(camera);
	}
	return distortionNode;
}

///////////////////////////////////////////////////////////////////////////
// in-line GLSL source code for the "microshader" example

static const char *shaderVertSource = {
    "// passthru - Simple pass through vertex shader\n"
    "void main(void)\n"
    "{\n"
    "    gl_TexCoord[0].xy = gl_MultiTexCoord0.xy;\n"
    "    gl_Position = ftransform();\n"
    "}\n"
};

static const char *shaderFragSource = {
    "#extension GL_ARB_uniform_buffer_object : enable\n"
    "uniform sampler2D texture1;\n"
    "uniform sampler2D texture2;\n"
    "uniform sampler2D texture3;\n"
    "uniform sampler1D brightness;\n"
    "void main(void)\n"
    "{\n"
    "    vec4  color1;\n"
    "    vec4  color2;\n"
    "    vec4  color3;\n"
    "    float  lum1;\n"
    "    float  lum2;\n"
    "    float  lum3;\n"
    "    float  bright_adj;\n"
    "    bright_adj = texture1D( brightness,  gl_TexCoord[0].s );\n"
    "    color1 = texture2D( texture1, gl_TexCoord[0].st );\n"
    "    lum1 = color1.r * 0.2989 + color1.g * 0.5870 + color1.b * 0.1140;\n"
    "    color2 = texture2D( texture2, gl_TexCoord[0].st );\n"
    "    lum2 = color2.r * 0.2989 + color2.g * 0.5870 + color2.b * 0.1140;\n"
    "    color3 = texture2D( texture3, gl_TexCoord[0].st );\n"
    "    lum3 = color3.r * 0.2989 + color3.g * 0.5870 + color3.b * 0.1140;\n"
	// Hannah's projector uses Blue->Red->Green order
    // "    gl_FragColor = vec4( 0, 0, 0, 1.0 );\n"
    "    gl_FragColor = vec4( bright_adj*lum2, bright_adj*lum3, bright_adj*lum1, 1.0 );\n"
    "}\n"
};

///////////////////////////////////////////////////////////////////////////

osg::Node*
ViewingWindowQt::createDistortionSubgraphWithPacking( int index, int idx,
                                                      osg::Node* subgraph,
                                                      const osg::Vec4& clearColour,
                                                      double theta, bool addHud )
{
	osg::Group* distortionNode = new osg::Group;

	unsigned int tex_width = (unsigned int) pow( 2, ceil( log( (float)(_traits->width) ) / log( 2.f ) ) );
	unsigned int tex_height = (unsigned int) pow( 2, ceil( log( (float)(_traits->height) ) / log( 2.f ) ) );

    
	if ( tex_width > tex_height )
		tex_height = tex_width;
	else
		tex_width = tex_height;    
    
    osg::Image* brightnessImage = new osg::Image;

    brightnessImage->allocateImage(tex_width, 1, 1, GL_RED, GL_UNSIGNED_BYTE);
    unsigned char* brightnessData = brightnessImage->data();
    if (_brightness_adjustment_enabled) {
        float middle = tex_width / 2;
        if (_screen_type == Cylindrical) {
            for (int x = 0; x < tex_width; ++x) {
                float offset = _screen_geometry[index].screen_width*std::abs(middle - x) / tex_width;
                float theta = offset / _screen_radius;
                float height = _frustum[index].distance + _screen_radius 
                    - (std::cos(theta) * _screen_radius);
                float width = std::sin(theta) * _screen_radius;
                float distance = std::sqrt(height * height + width * width);
                float luminosity_adj = (_frustum[index].distance / distance) * (_frustum[index].distance / distance);
                *(brightnessData++) = (char)std::round(255 * luminosity_adj);
            }
        }
        else {
            // Flat
            for (int x = 0; x < tex_width; ++x) {
                float offset = _screen_geometry[index].screen_width*std::abs(middle - x) / tex_width;
                float distance = std::sqrt(offset*offset + _frustum[index].distance*_frustum[index].distance);
                float luminosity_adj = (_frustum[index].distance / distance) * (_frustum[index].distance / distance);
                *(brightnessData++) = (char)std::round(255 * luminosity_adj);
            }
        }
    }
    else {
        for (int x = 0; x < tex_width; ++x) {
            *(brightnessData++) = (char)255.0f;
        }
    }

    _brightness.push_back( new osg::Texture1D);
    _brightness.back()->setTextureWidth(tex_width);
    _brightness.back()->setInternalFormat(GL_RED);
    _brightness.back()->setFilter(osg::Texture1D::MIN_FILTER, osg::Texture1D::LINEAR);
    _brightness.back()->setFilter(osg::Texture1D::MAG_FILTER, osg::Texture1D::LINEAR);
    _brightness.back()->setImage(brightnessImage);

	_texture.push_back( new osg::Texture2D );
	_texture.back()->setTextureSize(tex_width, tex_height);
	_texture.back()->setInternalFormat(GL_RGBA);
	_texture.back()->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR);
	_texture.back()->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);

	osg::Program* program = new osg::Program;
	program->setName( "texture_shader" );
	program->addShader( new osg::Shader( osg::Shader::VERTEX, shaderVertSource ) );
	program->addShader( new osg::Shader( osg::Shader::FRAGMENT, shaderFragSource ) );

	// set up the render to texture camera 1.
	{
		osg::Camera* camera = new osg::Camera;

		// set clear the color and depth buffer
		camera->setClearColor(clearColour);
		camera->setClearMask(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		// just inherit the main cameras view
		camera->setReferenceFrame(osg::Transform::RELATIVE_RF);
		camera->setProjectionMatrix(osg::Matrixd::identity());
		camera->setViewMatrix(osg::Matrixd::identity());

		// set viewport
		camera->setViewport(0,0,tex_width,tex_height);

		// set the camera to render before the main camera.
		camera->setRenderOrder( osg::Camera::PRE_RENDER, idx );

		// tell the camera to use OpenGL frame buffer object where supported.
		camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

		// attach the texture and use it as the color buffer.
		camera->attach( osg::Camera::BufferComponent( osg::Camera::COLOR_BUFFER ),
		               _texture.back() );
		// Only render visible objects
		camera->setCullMask( ~INVISIBLE_MASK );

		// add subgraph to render
		camera->addChild(subgraph);

		std::stringstream sstm;
		sstm << "RTT_" << index;
		camera->setName( sstm.str() );

//		camera->setInitialDrawCallback( new Draw_Callback( "RTT Initial" ) );
//		camera->setFinalDrawCallback( new Draw_Callback( "RTT Final" ) );

		osg::Matrixd mat;
		mat.makeIdentity();

		if ( _use_custom_frustum )
		{
			if ( _frustum[index].use_matrix )
				mat = _frustum[index].mat;
			else
				mat.makeFrustum( _frustum[index].left, _frustum[index].right,
				                 _frustum[index].bottom, _frustum[index].top,
				                 _frustum[index].near_, _frustum[index].far_ );
		}

		camera->setCullCallback( new CullCallback( _use_custom_frustum, mat, &( _frustum[index] ) ) );

		distortionNode->addChild(camera);
	}

	if ( idx == 2 )
	{
		// create the quad to visualize.
		osg::Geometry* polyGeom = new osg::Geometry();

		polyGeom->setSupportsDisplayList(false);

		osg::Vec3 origin(0.0f,0.0f,0.0f);
		osg::Vec3 xAxis(1.0f,0.0f,0.0f);
		osg::Vec3 yAxis(0.0f,1.0f,0.0f);
		float height = _traits->height;
		float width = _traits->width;
		int noSteps = 50;

		osg::Vec3Array* vertices = new osg::Vec3Array;
		osg::Vec2Array* texcoords = new osg::Vec2Array;
		osg::Vec4Array* colors = new osg::Vec4Array;

		osg::Vec3 bottom = origin;
		osg::Vec3 dx = xAxis*(width/((float)(noSteps-1)));
		osg::Vec3 dy = yAxis*(height/((float)(noSteps-1)));

		osg::Vec2 dx_texcoord(1.0f/(float)(noSteps-1),0.0f);
		osg::Vec2 dy_texcoord(0.0f,1.0f/(float)(noSteps-1));

		osg::Vec2 texcoord = _bottom_texcoord[index];
		int i,j;
		osg::Vec2 xy_min( 1, 1 );
		osg::Vec2 xy_max( -1, -1 );

		_graph[index]->compute_basis_matrix();

		for(i=0;i<noSteps;++i)
		{
			//osg::Vec3 cursor = bottom+dy*(float)i;
			osg::Vec2 texcoord = _bottom_texcoord[index]+dy_texcoord*(float)i;
			for(j=0;j<noSteps;++j)
			{
				QPointF pt = _graph[index]->evaluate( texcoord.x() - _bottom_texcoord[index].x(),
				                               1.-(texcoord.y() - _bottom_texcoord[index].y()) );
				osg::Vec3 cursor( width * pt.x(), height - (height * pt.y()), 0. );
				vertices->push_back(cursor);

				if ( _use_distortion[index] )
				{
					float th = texcoord.x();
					float h = texcoord.y() / _y_focal_length[index];
					float xh = sinf( th / _x_focal_length[index] );
					float yh = h;
					float zhx = cosf( th / _x_focal_length[index] );
					float zhy;
					if ( _x_center_distorts_y )
						zhy = cosf( th / _y_focal_length[index] );
					else
						zhy = cosf( (th -  _bottom_texcoord[index].x() - 0.5f) / _y_focal_length[index] );

					osg::Vec2 texcoordh( _x_focal_length[index] * xh / zhx - _bottom_texcoord[index].x(),
					                     _y_focal_length[index] * yh / zhy - _bottom_texcoord[index].y() );

					xy_min.x() = min( xy_min.x(), texcoordh.x() );
					xy_max.x() = max( xy_max.x(), texcoordh.x() );
					xy_min.y() = min( xy_min.y(), texcoordh.y() );
					xy_max.y() = max( xy_max.y(), texcoordh.y() );

					texcoords->push_back( texcoordh );
				}
				else
					texcoords->push_back( osg::Vec2( texcoord.x() - _bottom_texcoord[index].x(),
					                                 texcoord.y() - _bottom_texcoord[index].y() ) );
				colors->push_back( _globalBrightness[ index ] );

//				cursor += dx;
				texcoord += dx_texcoord;
			}
		}

		if ( _use_distortion[index] )
		{
			osg::Vec2 scale( 1.f/(xy_max.x()-xy_min.x()),  1.f/(xy_max.y()-xy_min.y()));

			for ( osg::Vec2Array::iterator it = texcoords->begin(); it < texcoords->end(); it++ )
			{
				it->x() = ( it->x() - xy_min.x() ) * scale.x();
				it->y() = ( it->y() - xy_min.y() ) * scale.y();
			}
		}

		// pass the created vertex array to the points geometry object.
		polyGeom->setVertexArray(vertices);

		polyGeom->setColorArray(colors);
		polyGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		polyGeom->setTexCoordArray(0,texcoords);


		for(i=0;i<noSteps-1;++i)
		{
			osg::DrawElementsUShort* elements = new osg::DrawElementsUShort(osg::PrimitiveSet::QUAD_STRIP);
			for(j=0;j<noSteps;++j)
			{
				elements->push_back(j+(i+1)*noSteps);
				elements->push_back(j+(i)*noSteps);
			}
			polyGeom->addPrimitiveSet(elements);
		}


		// new we need to add the texture to the Drawable, we do so by creating a
		// StateSet to contain the Texture StateAttribute.
		osg::StateSet* stateset = polyGeom->getOrCreateStateSet();
		for ( int i = 0; i < 3; i++ )
			stateset->setTextureAttributeAndModes( i, _texture[ index*3 + i ],
			                                       osg::StateAttribute::ON );
        stateset->setTextureAttributeAndModes(3, _brightness[index], osg::StateAttribute::ON);
		stateset->addUniform( new osg::Uniform( "texture1", 0 ) );
		stateset->addUniform( new osg::Uniform( "texture2", 1 ) );
		stateset->addUniform( new osg::Uniform( "texture3", 2 ) );
        stateset->addUniform( new osg::Uniform("brightness", 3));
		stateset->setAttributeAndModes( program,
		                                osg::StateAttribute::ON );
		stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

		osg::Geode* geode = new osg::Geode();
		geode->addDrawable(polyGeom);

		// set up the camera to render the textured quad
		osg::Camera* camera = new osg::Camera;

		// just inherit the main cameras view
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setViewMatrix(osg::Matrix::identity());
		camera->setProjectionMatrixAsOrtho2D( 0, _traits->width, 0, _traits->height );

		// set the camera to render before the main camera.
		camera->setRenderOrder(osg::Camera::NESTED_RENDER);

		// add subgraph to render
		camera->addChild(geode);
		if ( addHud )
			camera->addChild( _hud );

		distortionNode->addChild(camera);
	}
	return distortionNode;
}

osg::Camera*
ViewingWindowQt::multipleWindowWithDistortion( int i, int numCameras, int num_displays,
                                               int starting_display )
{
   osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();

   _multipleScreens = num_displays > 1 && numCameras >= 1;

   osg::Node* sceneData = getSceneData();

   // osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();

   osg::GraphicsContext::ScreenIdentifier si;
   si.readDISPLAY();

   // displayNum has not been set so reset it to 0.
   if ( si.displayNum < 0 ) si.displayNum = 0;
   si.screenNum = starting_display;

   unsigned int w, h;
   wsi->getScreenResolution( si, w, h );

   if ( _multipleScreens )
	   _width[i] = w;
   else
	   _width[i] = w / numCameras;

   _height[i] = h;

   double aspectRatioScale = (float)_width[i] / (float)_height[i];
   double translate_x = double(numCameras)-1 + i * -2.0;
   double theta = 0.0;

   if ( numCameras > 1 )
   {
	   // Even number of cameras
	   if ( numCameras % 2 == 0 )
		   theta = osg::inDegrees(-_fov_offset / 2. ) * numCameras / 2 + i*osg::inDegrees(_fov_offset);
	   else
		   theta = osg::inDegrees(-_fov_offset) * ( numCameras - 1 )/ 2 + i*osg::inDegrees(_fov_offset   );
   }

   _traits = new osg::GraphicsContext::Traits;
   _traits->hostName = si.hostName;
   _traits->displayNum = si.displayNum;

   if ( _multipleScreens )
   {
	   _traits->screenNum = si.screenNum+i;
		if ( _custom_widget_enabled )
			_traits->x = 0;
		else
		{
			QRect screenres = QApplication::desktop()->screenGeometry( _traits->screenNum );
			_traits->x = screenres.x();
		}
	   _traits->width = _width[i];
   }
   else
   {
	   _traits->screenNum = si.screenNum;
		if ( _custom_widget_enabled )
			_traits->x = i*_width[i];
		else
		{
			QRect screenres = QApplication::desktop()->screenGeometry( _traits->screenNum );
			_traits->x = screenres.x() + (i*_width[i]);
		}
	   _traits->width = _width[i];
   }
   _traits->y = 0;
   _traits->height = _height[i];
   _traits->windowDecoration = false;
   _traits->doubleBuffer = true;
   _traits->sharedContext = 0;
   _traits->vsync = 1;

   osg::ref_ptr<osg::Camera> camera = new osg::Camera;
   if ( _custom_widget_enabled )
   {
	   MyGraphicsWindowQt* gwqt = new MyGraphicsWindowQt( _traits.get() );
	   camera->setGraphicsContext( gwqt );
	   gwqt->setClearColor( _clearColor );
	   gwqt->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }
   else
   {
	   osgQt::GraphicsWindowQt* gwqt = new osgQt::GraphicsWindowQt( _traits.get(), 0, 0, Qt::FramelessWindowHint );
	   camera->setGraphicsContext( gwqt );
      gwqt->setClearColor( _clearColor );
      gwqt->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   camera->setClearColor( _clearColor );

   camera->setViewport(new osg::Viewport(0,0, _width[i], _height[i]));
   GLenum buffer = _traits->doubleBuffer ? GL_BACK : GL_FRONT;
   camera->setDrawBuffer(buffer);
   camera->setReadBuffer(buffer);

   osg::Node* data;

   if ( _packFrames )
   {
	   _frameSwitch.push_back( new osg::Switch );
	   for ( int idx = 0; idx < 3; idx++ )
		   _frameSwitch.back()->addChild( createDistortionSubgraphWithPacking( i, idx, sceneData,
		                                                               _clearColor,
		                                                               theta,
		                                                               i == _whichScreen ));
	   _frameSwitch.back()->setAllChildrenOff();
	   data = _frameSwitch.back();
   }
   else
	   data = createDistortionSubgraph( i, sceneData, _clearColor,
	                                    theta, i == _whichScreen );

   camera->addChild( data );

   if ( _rotateCamera )
	   addSlave(camera.get(), osg::Matrix::scale( aspectRatioScale, 1.0, 1.0 ),
	            osg::Matrix::rotate( theta, 0.0, 1.0, 0.0) * osg::Matrixd::rotate(osg::inDegrees(-90.0f), 0.0,0.0,1.0), false );
   else
	   addSlave(camera.get(), osg::Matrix::scale(aspectRatioScale, 1.0, 1.0)*
	            osg::Matrix::translate(translate_x, 0.0, 0.0), osg::Matrix(), false );

   return camera.release();
}

