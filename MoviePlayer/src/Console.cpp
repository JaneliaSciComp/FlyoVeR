#include <cmath>
#include <cstring>
#include <cstdlib>
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
#include <sstream>
#include <algorithm>
using std::min;
using std::max;

#include <boost/regex.hpp>

#include <QtCore/QFileInfo>
#include <QColorDialog>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Material>
#include <osg/Node>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osg/ShapeDrawable>
#include <osgUtil/SmoothingVisitor>

#include <osgbDynamics/MotionState.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbCollision/RefBulletObject.h>
#include <osgbDynamics/RigidBodyAnimation.h>

#include <ColladaVisitor>
#include <Console>
#include <Globals.h>

#define radians( _x_ ) _x_ * M_PI / 180.f
#define degrees( _x_ ) _x_ * 180.f / M_PI

const float scaling_val = 10000.f;

class SmoothNodeVisitor : public osg::NodeVisitor
{
public:

	SmoothNodeVisitor()
		: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{
		_smoother = new osgUtil::SmoothingVisitor;
	}

	~SmoothNodeVisitor( )
	{
		delete _smoother;
	}

	void apply(osg::Node& node)
	{
		traverse(node);
	}

	void apply( osg::Geode& node )
	{
		_smoother->apply( node );
	}

private:
	osgUtil::SmoothingVisitor* _smoother;
};

class KeyboardEventHandler : public osgGA::GUIEventHandler
{
public:
    
	KeyboardEventHandler( Console* console ): _console( console ) {}
    
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
	{
		switch(ea.getEventType())
		{
			case(osgGA::GUIEventAdapter::KEYDOWN):
			{
				if ( ea.getKey()==' ' )
				{
					_console->resetPosition();
					return true;
				}
				if ( ea.getKey()=='d' )
				{
					_console->disableMotion();
					if ( _console->motionAndDisplayGanged() )
						_console->blankDisplay();
					return true;
				}
				if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Up )
				{
					_console->manualSpeedUp();
					return true;
				}
				if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Down )
				{
					_console->manualSpeedDown();
					return true;
				}
				
				break;
			}
			default:
				break;
		}
		return false;
	}
        
	Console* _console;
        
};

Console::Console( osg::ArgumentParser& args, QWidget *parent ): 
	QMainWindow( parent ), Ui::MainWindow(), _model( new Scene_Model ),
	arguments( args ), viewer( 0 ), callback( 0 ), _firstTime( true ), 
	connected( false ), _connectionCounter( 1 ), useImage( false ), lastCamera( 1 ),
	_calibrating( false ), _activeScene( 0 ), osgFile( "cow.osg" ), _Vfwdf( 1.f ), _Vssf( 1.f ), 
	_Omegaf( 1.f ), _blankDisplay( false ), _useAutoHeading( true ), _motionDisabled( false ),
	_manual_speed( 0.f, 0.f, 0.f ), 
	_plot_ratio_widget( 0 ), _plot_velocity_widget(0),
	_plot_ratio_max_y(0), _plot_velocity_max_y(0), _plot_timer( 0 ), 
    _trace_path( 0 ), _playForward( false ), _playBackward( false ),
	_recording( false )

{
	setupUi( this );

	// Menu Actions
	connect( actionOpen, SIGNAL( triggered() ), this, SLOT( doOpen() ) );
	connect( actionOpenConfig, SIGNAL( triggered() ), this, SLOT( doOpenConfig() ) );
	connect( actionSave, SIGNAL( triggered() ), this, SLOT( doSaveConfig() ) );
	connect( actionOpen_TraceFile, SIGNAL( triggered() ), this, SLOT( doOpenTraceFile() ) );
	connect( actionQuit, SIGNAL( triggered() ), this, SLOT( doClose() ) );
	connect( actionAbout, SIGNAL( triggered() ), this, SLOT( about() ) );
	connect( imageCalibrationButton, SIGNAL( clicked() ), this, SLOT( doImageOpen() ) );
	connect( osgDataButton, SIGNAL( clicked() ), this, SLOT( doOSGOpen() ) );

	connect( cameraSelectSpinBox, SIGNAL( valueChanged(int) ), this, 
	         SLOT( resetSliders(int) ) );
	connect( cameraSelectSpinBox, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setGlobalBrightnessTarget(int) ) );

	connect( numCamerasSpinBox, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setCameraMaximums(int) ) );
	connect( initializeCameraButton, SIGNAL( clicked() ), this, SLOT( startViewer() ) );
	connect( horizontalBlankingSlider, SIGNAL( valueChanged(int) ), this, 
	         SLOT( updateViewportLeft(int) ) );
	connect( horizontalBlankingSlider_2, SIGNAL( valueChanged(int) ), this, 
	         SLOT( updateViewportRight(int) ) );
	connect( networkConnectButton, SIGNAL( clicked() ), this, SLOT( setupConnection() ) );
	connect( disableMotionButton, SIGNAL( clicked() ), this, SLOT( disableMotionCallback() ) );
	connect( displayBlankingButton, SIGNAL( clicked() ), this, 
	         SLOT( setDisplayBlankingCallback() ) );
	connect( gangMotionCheckBox, SIGNAL( toggled(bool) ), this, 
	         SLOT( setGangMotion(bool) ) );
	connect( rewindButton, SIGNAL( clicked() ), this, SLOT( rewindCallback() ) );
	connect( playForwardButton, SIGNAL( clicked() ), this, SLOT( playForwardCallback() ) );
	connect( playBackwardsButton, SIGNAL( clicked() ), this, SLOT( playBackwardsCallback() ) );
	connect( recordButton, SIGNAL( clicked() ), this, SLOT( recordCallback() ) );
	connect( timeSlider, SIGNAL( valueChanged(int) ), this, SLOT( timeSliderCallback( int ) ) );

	// Display Callbacks
	connect( resetButton, SIGNAL( clicked() ), this, SLOT( resetDisplay() ) );
	connect( distributeHoriz, SIGNAL( clicked() ), this, SLOT( distributeHorizontally() ) );
	connect( distributeVert, SIGNAL( clicked() ), this, SLOT( distributeVertically() ) );
	connect( smoothButton, SIGNAL( clicked() ), this, SLOT( smoothDisplay() ) );
	connect( linearizeEdgesButton, SIGNAL( clicked() ), this, SLOT( linearizeEdges() ) );
	connect( framePackingEnabled, SIGNAL( toggled(bool) ), 
	         this, SLOT( setFramePacking(bool) ) );

	// Lighting Callbacks
	connect( ambientLightButton, SIGNAL( clicked() ), this, 
	         SLOT( setAmbientColorCallback() ) );
	connect( backgroundColorButton, SIGNAL( clicked() ), this, 
	         SLOT( setBackgroundColorCallback() ) );
	connect( diffuseLightButton, SIGNAL( clicked() ), this, 
	         SLOT( setDiffuseColorCallback() ) );
	connect( diffusePowerLevel, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setDiffusePower(int) ) );
	connect( globalBrightnessButton, SIGNAL( clicked() ), this, 
	         SLOT( setGlobalBrightnessCallback() ) );

	connect( fovSlider, SIGNAL( valueChanged(int) ), this, SLOT( setFieldOfView(int) ) );
	connect( fovOffsetSlider, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setFieldOfViewOffset(int) ) );
	connect( fovOffsetSpinBox, SIGNAL( valueChanged(double) ), this, 
	         SLOT( setFieldOfViewOffset(double) ) );
	connect( rotatedCamerasCheckBox, SIGNAL( toggled(bool) ), this, 
	         SLOT( setRotatedCameras(bool) ) );

	// Indicator Widgets
	connect( indicatorSizeSlider, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setIndicatorSize(int) ) );
	connect( borderSizeSlider, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setIndicatorBorderSize(int) ) );
	connect( indicatorHorizontalPositionSlider, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setIndicatorPosition(int) ) );
	connect( indicatorVerticalPositionSlider, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setIndicatorPosition(int) ) );
	connect( screenSelectSpinBox, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setIndicatorScreen(int) ) );
	connect( frameModeComboBox, SIGNAL( currentIndexChanged(int) ), this, 
	         SLOT( setIndicatorMode(int) ) );

	connect( focalLengthSlider_x, SIGNAL( valueChanged(int)), this, 
	         SLOT( setXFocalLength(int) ) );
	connect( focalLengthSlider_y, SIGNAL( valueChanged(int)), this, 
	         SLOT( setYFocalLength(int) ) );
	connect( xCenterSlider, SIGNAL( valueChanged(int) ), this, SLOT( setCenter( ) ) );
	connect( yCenterSlider, SIGNAL( valueChanged(int) ), this, SLOT( setCenter( ) ) );
	connect( enableDistortion, SIGNAL( toggled(bool) ), this, 
	         SLOT( setDistortionEnabled(bool) ) );

	// Calibration Widgets
	connect( calibrateXButton, SIGNAL( clicked() ), this, SLOT( calibrateX( ) ) );
	connect( calibrateYButton, SIGNAL( clicked() ), this, SLOT( calibrateY( ) ) );
	connect( calibrateZButton, SIGNAL( clicked() ), this, SLOT( calibrateZ( ) ) );
	connect( editValues, SIGNAL( toggled(bool) ), this, SLOT( enableEditing(bool) ) );
	connect( updateCalibrationValuesButton, SIGNAL( clicked() ), this, SLOT( updateCalibrationValuesCallback( ) ) );

	// Configuration Widgets
	connect( velocitySmoothingEnabled, SIGNAL( toggled(bool) ), this, 
	         SLOT( enableVelocitySmoothing(bool) ) );
	connect( velocitySmoothingInterval, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setIntervalForVelocitySmoothing(int) ) );
	connect( inputSmoothingEnabled, SIGNAL( toggled(bool) ), this, 
	         SLOT( enablePreBlendHeadingSmoothing(bool) ) );
	connect( intermediarySmoothingEnabled, SIGNAL( toggled(bool) ), this, 
	         SLOT( enableIntermediaryHeadingSmoothing(bool) ) );
	connect( outputSmoothingEnabled, SIGNAL( toggled(bool) ), this, 
	         SLOT( enablePostBlendHeadingSmoothing(bool) ) );
	connect( inputSmoothingInterval, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setIntervalForPreBlendingSmoothing(int) ) );
	connect( intermediarySmoothingInterval, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setIntervalForIntermediarySmoothing(int) ) );
	connect( outputSmoothingInterval, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setIntervalForPostBlendingSmoothing(int) ) );
	connect( portNameControl, SIGNAL( returnPressed() ), this, 
	         SLOT( setSerialPortName()) );
	connect( fileExportToggle, SIGNAL( toggled(bool) ), this, 
	         SLOT( enableFileExportCallback(bool) ) );
	connect( exportFileNameSelector, SIGNAL( clicked() ), this, SLOT( doSetExportFileName() ) );

	connect( treadmillOutputToggle, SIGNAL( toggled(bool) ), this, 
	         SLOT( enableTreadmillOutputCallback(bool) ) );
	connect( reducedOutputToggle, SIGNAL( toggled(bool) ), this, 
	         SLOT( enableReducedOutputCallback(bool) ) );
	connect( headingStatusButton, SIGNAL( clicked() ), this, 
	         SLOT( autoHeadingCallback() ) );
	connect( crossbarWidthSpinBox, SIGNAL( valueChanged(double) ), this, 
	         SLOT( setCrossbarWidth(double) ) );
	connect( minimumVelocityThreshold, SIGNAL( valueChanged(double) ), this, 
	         SLOT( setMinimumVelocityThresold(double) ) );
	connect( restrictVerticalMotion, SIGNAL( toggled(bool) ), this, 
	         SLOT( restrict_vertical_motion(bool) ) );
	connect( outputRateSpinBox, SIGNAL( valueChanged(int) ), this, 
	         SLOT( setOutputRate(int) ) );

	// Debugging Widgets
	connect( debugPhysicsCheckBox, SIGNAL( toggled(bool) ), this, 
	         SLOT( togglePhysicsDebugging(bool) ) );
	connect( showInvisibleObjectsCheckBox, SIGNAL( toggled(bool) ), this, 
	         SLOT( showInvisibleObjects(bool) ) );
	connect( shaderComboBox, SIGNAL(currentIndexChanged(int)), this, 
	         SLOT( changeShader(int) ) );

	// Turning Widgets
	connect( thresholdTurningEnabled, SIGNAL( toggled(bool) ), this, 
	         SLOT( enableThresholdTurning(bool) ) );
	connect( plotTableWidget, SIGNAL( cellChanged ( int, int ) ), this, 
	         SLOT( updatePlot( int, int ) ) );
	connect( plotVelocityTableWidget, SIGNAL( cellChanged ( int, int ) ), this, 
	         SLOT( updateVelocityPlot( int, int ) ) );
	connect( addRowButton, SIGNAL( clicked() ), this, 
	         SLOT( addRow() ) );
	connect( deleteRowButton, SIGNAL( clicked() ), this, 
	         SLOT( deleteRow() ) );
	connect( addRowButton_2, SIGNAL( clicked() ), this, 
	         SLOT( addRowVelocity() ) );
	connect( deleteRowButton_2, SIGNAL( clicked() ), this, 
	         SLOT( deleteRowVelocity() ) );
	connect( thresholdTurningSlider, SIGNAL( valueChanged(int) ), this, 
	         SLOT( updateThresholdWeight(int) ) );
	connect( minThresholdScale, SIGNAL( valueChanged(double) ), this, 
	         SLOT( updateThresholdScale(double) ) );
	connect( maxThresholdScale, SIGNAL( valueChanged(double) ), this, 
	         SLOT( updateThresholdScale(double) ) );

	// Setup a null communicator
	comm = new VoidCommunicator( hostname(), port(), retries() );
	enableBlankingWidgets( false );
	actionOpen->setEnabled( false );
	headingStatusButton->setEnabled( false );

	for ( int i = 0; i < 3; i++ )
		_calibrationRuns[i] = 0;

	// Set the color labels
	_clearColor = osg::Vec4( 0.f, 0.f, 0.f, 1.0 );

	QColor color( 25, 25, 25 );
	ambientLabel->setText( "#191919" );
	ambientLabel->setPalette( QPalette( color ) );
	ambientLabel->setAutoFillBackground( true );

	diffuseLabel->setText( "#191919" );
	diffuseLabel->setPalette( QPalette( color ) );
	diffuseLabel->setAutoFillBackground( true );

	color.setRgb( 0, 0, 0 );
	backgroundLabel->setText( "#000000" );
	backgroundLabel->setPalette( QPalette( color ) );
	backgroundLabel->setAutoFillBackground( true );

	color.setRgb( 255, 255, 255 );
	brightnessLabel->setText( "#ffffff" );
	brightnessLabel->setPalette( QPalette( color ) );
	brightnessLabel->setAutoFillBackground( true );

	for ( int i = 0; i < numCamerasSpinBox->value(); i++ )
		_brightness.push_back( color );
   
	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
    
	// Ensure there is one display widget before we add new ones. The camera
	// selection widget is 1-based so we need the extra padding
	if ( displayWidget->count() == 0 )
	{
		QWidget* widget = new QWidget();
		displayWidget->addWidget( widget );
	}
	else
		// Remove all current layout items in `displayWidget'
		while ( displayWidget->count() > 1 )
		{
			QWidget* widget = displayWidget->currentWidget();
			displayWidget->removeWidget( widget );
		}
	
   int num_screens = wsi->getNumScreens();
   char s[256];
   sprintf( s, "Number of Displays(%d)", num_screens );
   displayCountLabel->setText( s );
   startingDisplaySpinBox->setValue( num_screens == 1 ? 1 : 2 );
   num_screens = num_screens > 1 ? num_screens - 1 : num_screens;
   numCamerasSpinBox->setValue( num_screens );
   numDisplaysSpinBox->setValue( num_screens );
   
   // In case setting `numCamerasSpinBox' above didn't trigger the callback 
   setCameraMaximums( num_screens );
   
	QDesktopWidget desktopWidget;
	QRect r( desktopWidget.screenGeometry( 0 ) );
	indicatorHorizontalPositionSlider->setMaximum( r.right() );
	indicatorHorizontalPositionSpinBox->setMaximum( r.right() );
	indicatorVerticalPositionSlider->setMaximum( r.bottom() );
	indicatorVerticalPositionSpinBox->setMaximum( r.bottom() );

	screenSelectSpinBox->setValue( numCamerasSpinBox->value() );

	displayWidget->setCurrentIndex( 1 );
   
	// The file status widget
	_fileWidget = new FileStatusWidget( fileGroupBox, fileGroupBoxLayout, verticalSpacer_4 );
	QObject::connect( _fileWidget, SIGNAL( fileActivated(int) ), 
	                  this, SLOT( switchScene(int) ) );

	this->setFocusPolicy( Qt::StrongFocus );

	startingLocationsBox->hide();
	headingStatusButton->hide();
	
	std::vector< double > x, y, vx, vy;
	x.push_back( 0. ); y.push_back( radians( 60. ) );
	x.push_back( 1. ); y.push_back( radians(60.) );
	x.push_back( 3. ); y.push_back( radians(30.) );
	x.push_back( 5. ); y.push_back( radians(6.) );
	x.push_back( 10. ); y.push_back( radians(3.) );
	x.push_back( 100. ); y.push_back( radians(0.) );
	x.push_back( 101. ); y.push_back( 0. );

	vx.push_back( 0. );  vy.push_back( 0. );
	vx.push_back( 0.1 ); vy.push_back( 0.01 );
	vx.push_back( 1. );  vy.push_back( 0.1 );
	vx.push_back( 10. ); vy.push_back( 0.5 );
	vx.push_back( 50. ); vy.push_back( 1. );
	vx.push_back( 100. );vy.push_back( 1. );

	initializePlotWidgets( x, y, vx, vy, true );

	// Magic values to align home position with tracker
	debugPanel->hide();
	xoffset->setValue( 0.0 );
	yoffset->setValue( 0.0 );
	zoffset->setValue( 0.0 );

	timeLabel->findChild<QLineEdit*>()->setReadOnly(true);

// Qt 4.8.5 will crash on OS X if we don't use a custom OpenGL widget
#ifdef __APPLE__
	useCustomGLWidget->setChecked( true );
#endif
}

void
Console::setupConnection( bool reset )
{
	Communicator* new_comm;

	if ( !connected ) 
	{
		if ( TCP_radioBtn->isChecked() )
		{
			new_comm = new TCPCommunicator( hostname(), port(), retries() );
		}
		else if ( UDP_radioBtn->isChecked() )
		{
			new_comm = new UDPCommunicator( hostname(), port(), retries() );
		}

		if ( new_comm->success )
		{
			delete comm;      
			comm = new_comm;
			comm->reset();
      
			// Clear out old data
			comm->read();
			connected = true;
      
			if ( exportFileNameStyle->currentIndex() == 1 )
			{
				char s[40];
				sprintf( s, "Disconnect (%d)", _connectionCounter );
				networkConnectButton->setText( s );
			}
			else
				networkConnectButton->setText( "Disconnect" );

			if ( reset )
				if ( fileExportToggle->isChecked() )
					enableFileExportCallback( true );
			
			from_string<float>(_Vfwdf, std::string( calibrateXValue->text().toStdString() ), std::dec);
			from_string<float>(_Omegaf, std::string( calibrateYValue->text().toStdString() ), std::dec);
			from_string<float>(_Vssf, std::string( calibrateZValue->text().toStdString() ), std::dec);

			_Vfwdf /= calibrateXGain->value();
			_Omegaf /= calibrateYGain->value();
			_Vssf /= calibrateZGain->value();

			// Potential problem here:
			// It is unlikely, but it is remotely possible that a call to connect
			// to the server is made before the viewer has finished averaging the
			// frames. This will occur if we switch to scripting, but for now the
			// scenario is unlikely.
			if ( _model->tracker() == 0 )
				callback = new RemoteCameraUpdateCallback( viewer->getCameraManipulator(), 
				                                           comm, _Vfwdf, _Omegaf, _Vssf, 
				                                           ballRadiusBox->value(), 
				                                           1, _model->has_path() );
			else
				callback = new RemoteCameraUpdateCallback( _model->tracker(), 
				                                           comm, _Vfwdf, _Omegaf, _Vssf, 
				                                           ballRadiusBox->value(), 
				                                           1, _model->has_path() );

			if ( _model->motion_disabled() )
				callback->toggleMotion();

			callback->addNestedCallback( _model->camera_callback() );
			
			viewer->setUpdateCallback( callback, reset );
      
			_model->move_offset( xoffset->value(), yoffset->value(), zoffset->value() );

			if ( _model->is_tracking() )
			{
				_model->camera_callback()->interval_for_velocity_smoothing( velocitySmoothingInterval->value() );
				_model->camera_callback()->enable_velocity_smoothing( velocitySmoothingEnabled->isChecked() );
				_model->camera_callback()->set_plot_data( _plot, _v_plot );
			}
			
			_model->engage_physics_engine( true );
			
			if ( enableTimedTrialCheckBox->isChecked() )
			{
				_time = timeTrialEditor->time();
				int ms = _time.hour() * 3600000 + _time.minute() * 60000 + _time.second() * 1000;
				QTimer::singleShot( ms, this, SLOT( setupConnection() ) );
				_clock_timer = new QTimer( this );
				connect( _clock_timer, SIGNAL( timeout() ), this, SLOT( update_timer() ) );
				_clock_timer->start(1000);
				enableTimedTrialCheckBox->setEnabled( false );
				timeTrialEditor->setEnabled( false );
				// Connection button needs to be disabled during a time trial
				networkConnectButton->setEnabled( false );
			}
		}
		else
		{
			viewer->setUpdateCallback( 0, reset );

			callback = 0;
			_model->engage_physics_engine( false );
		}
	}
	else
	{
		if ( enableTimedTrialCheckBox->isChecked() )
		{
			_clock_timer->stop();
			timeTrialEditor->setTime( _time );
			delete _clock_timer;
			enableTimedTrialCheckBox->setEnabled( true );
			timeTrialEditor->setEnabled( true );
			networkConnectButton->setEnabled( true );

			std::ostringstream command;
			command << "Trial_Ended";
			viewer->outputEvent( command, command, true );
			viewer->resetManipulatorMode(); // Not sure if this is necessary
			move_to_start();
		}

		callback->removeNestedCallback( _model->camera_callback() );
		_model->clear_tracker_data();
		
		connected = false;
		callback = 0;
		viewer->setUpdateCallback( 0, reset );
		networkConnectButton->setText( "Connect" );
		_model->engage_physics_engine( false );
		if ( reset )
			if ( exportFileNameStyle->currentIndex() == 1 )
				_connectionCounter++;

	}
}

void
Console::motionAndBlankingCallback()
{
	disableMotion();
	setDisplayBlankingCallback();
}

void
Console::disableMotionCallback()
{
	disableMotion();
}

std::string
Console::port( void ) const
{
   return std::string( portCtrl->text().toStdString() );
}

int
Console::retries( void ) const
{
   int numRetries = 3;
   
   numRetries = retriesCtrl->text().toInt( );
   
   return numRetries;
}

std::string
Console::hostname( void ) const
{
   return std::string( hostnameCtrl->text().toStdString() );
}

void
Console::resetPosition()
{
	_model->move_offset( xoffset->value(), yoffset->value(), zoffset->value() );
	_model->reset_motion();
	if ( _model->eye_point_set() )
	{
		osg::Matrixd mat;
		mat.makeLookAt( _model->eye_point( 0 ), _model->center_point( 0 ), _model->up_direction( 0 ) );

//		viewer->getCameraManipulator()->setByMatrix( mat );
	}
	else
		if ( viewer ) viewer->home();
}

void 
Console::manualSpeedUp()
{
	if ( _motionDisabled && callback ) 
	{
		osg::Vec3d vec;
		// 1.414 is the smallest unit of forward motion based on cameras
		// with a 45 degree offset
		vec.set( ( ( 1.414 / (float)_Vfwdf )  * M_PI ) * ballRadiusBox->value(), 0.f, 0.f );
		_manual_speed += vec;
		callback->set_tracking_vector( _manual_speed );
	}
}

void
Console::manualSpeedDown()
{
	if ( _motionDisabled && callback ) 
	{
		osg::Vec3d vec;
		vec.set( ( ( -1.414 / (float)_Vfwdf )  * M_PI ) * ballRadiusBox->value(), 0.f, 0.f );
		_manual_speed += vec;
		callback->set_tracking_vector( _manual_speed );
	}
}

void
Console::disableMotion()
{
	std::ostringstream command;

	_model->toggle_motion_disabled();

	if ( callback ) callback->toggleMotion();
		
	if ( _model->motion_disabled() )
	{
		disableMotionButton->setText( "Enable Motion" );
		command << "Enable_Motion_Off";
	}
	else
	{
		disableMotionButton->setText( "Disable Motion" );
		command << "Enable_Motion_On";
	}

	viewer->outputEvent( command, command, true );
}

void
Console::blankDisplay()
{
	std::ostringstream command;

	if ( _blankDisplay )
	{
		_blankDisplay = false;
		displayBlankingButton->setText( "Enable Display Blanking" );
		command << "Display_Blanking_Off";
		for ( int i = 0; i < numCamerasSpinBox->value(); i++ )
			setGlobalBrightnessTarget( i + 1 );
	}
	else
	{
		_blankDisplay = true;
		command << "Display_Blanking_On";
		displayBlankingButton->setText( "Disable Display Blanking" );
		osg::Vec4 col = osg::Vec4( 0.f, 0.f, 0.f, 1.0 );
		
		for ( int i = 0; i < numCamerasSpinBox->value(); i++ )
			viewer->setGlobalBrightness( i, col );
	}

	viewer->outputEvent( command, command, true );
}

void
Console::keyPressEvent( QKeyEvent *event )
{
	switch ( event->key() ) 
	{
		case Qt::Key_Space:
			resetPosition();
			break;
		case Qt::Key_D:
		{
			disableMotion();
			if ( motionAndDisplayGanged() )
				blankDisplay();
		}
		break;
		case Qt::Key_S:
		{
			SmoothNodeVisitor smoother;
			smoother.apply( *( _model->scene_root() ) );
		}
		
		break;

		// These two cases are for when the user wants to manually drive through
		// the world without using the ball as input
		case Qt::Key_Up:
			manualSpeedUp();
			break;
		case Qt::Key_Down:
			manualSpeedDown();
			break;
		default:
			return;
	}
	
}

void 
Console::move_to_start()
{
	_model->switch_scene( _model->active_scene() );

	viewer->setTrackNode();

	headingStatusButton->setEnabled( true );

	viewer->getCameraManipulator()->setHomePosition( _model->start_center( 0 ), 
	                                                 _model->start_direction( 0 ), 
	                                                 osg::Vec3f( 0, 0, 1 ));

	viewer->setManipulatorToTrackingMode();
	resetPosition();

}

void
Console::doClose()
{
	if ( thresholdTurningEnabled->isChecked() )
		enableThresholdTurning( false ); //Force it to be off so that certain widgets
										 //aren't updated after we delete the model.
	
	if ( viewer )
		delete viewer;

	delete _model;
	
	close();
}

void
Console::about()
{
	QMessageBox::about( this, "About Jovian", "Jovian version: $Rev:: 534          $\n$Date:: 2014-02-06 08:58:1#$" );
}

void
Console::doOpen()
{
	bool reset_connection = false;
	
	viewer->resetManipulatorMode();

	if ( _model->is_tracking() ) 
	{
		// Turn off connection to remoteDataServer if active
		if ( connected )
		{
			reset_connection = true;
			setupConnection();
		}
	}
	
	Config_Memento* cm = _model->current_configuration();
	if ( !cm->initialized() )
		cm->initialize( this );
	
	save_config( cm );

	QString fileName = QFileDialog::getOpenFileName( this, tr("Open File"),
	                                                 "", tr("Collada (*.dae);; All Files (*.*)"));
	if (!fileName.isEmpty())
	{
		_fileWidget->loadFile( fileName );
		
		_model->load_collada( fileName );
		
		if ( viewer->movie_mode() )
		{
			CameraTraceUpdateCallback* ct = dynamic_cast< CameraTraceUpdateCallback* >( _model->camera_callback().get() );
			ct->setConsole( this );
		}
		
		if ( exportFileNameBox->text().count() == 0 )
		{
			QString s( fileName );
			s.replace( ".dae", "" );
			setExportFilename( s );
		}
		
		zoffset->setValue( _model->camera_mount_radius() );
		
		headingStatusButton->setEnabled( true );

		viewer->getCameraManipulator()->setHomePosition( _model->start_center( 0 ), 
		                                                 _model->start_direction( 0 ), 
		                                                 osg::Vec3f( 0, 0, 1 ));

		viewer->setTrackNode();
		viewer->setManipulatorToTrackingMode();
		resetPosition();
			
		// Force a reset of the diffuse lighting values on a load as lights are
		// created anew each read.
		QColor color( ambientLabel->text() );
		setAmbientColor( color );
		color = QColor( diffuseLabel->text() );
		setDiffuseColor( color );
		setDiffusePower( diffusePowerLevel->value() );
		
		// Reenable connection to remoteDataServer if it was active
		if ( reset_connection )
			setupConnection();

		viewer->loadData();

		if ( _trace_path != 0 )
		{
			_model->engage_physics_engine( true );
			viewer->setUpdateCallback( _model->camera_callback(), true );
		}
		
		cm = _model->current_configuration();
		if ( !cm->initialized() )
			cm->initialize( this );
		
		load_config( cm );
		
		// Set this after we reload the config file so that it's not overwritten
		crossbarWidthSpinBox->setValue( _model->crossbar_width() );
	}
}

void
Console::doOpenConfig()
{
	QString fileName = QFileDialog::getOpenFileName( this, tr("Open Config File"),
	                                                 "", tr("MouseOver Config (*.mvc);; All Files (*.*)"));
	if (!fileName.isEmpty())
	{
		QByteArray ba = fileName.toLatin1();
		ifstream config( ba.data() );
		Config_Memento* cm = _model->current_configuration();
		cm->initialize( this );
		cm->load( config );

		load_config( cm );

		config.close();
	}
}

void
Console::doSaveConfig()
{
	QString fileName = QFileDialog::getSaveFileName( this, tr("Save Config File"),
	                                                 "", tr("Collada (*.mvc);; All Files (*.*)"));
	if (!fileName.isEmpty())
	{
		QByteArray ba = fileName.toLatin1();
		ofstream config( ba.data() );
		Config_Memento* cm = _model->current_configuration();
		if ( !cm->initialized() )
			cm->initialize( this );
		
		save_config( cm );
		
		cm->save( config );

		config.close();
	}
}

void
Console::doOpenTraceFile()
{
	QString fileName = QFileDialog::getOpenFileName( this, tr("Open Trace File"),
	                                                 "", tr("Text File (*.txt);; All Files (*.*)"));
	if (!fileName.isEmpty())
	{
		QByteArray ba = fileName.toLatin1();
		loadTraceFile( ba.data() );
	}
}


void
Console::setXFocalLength( int value )
{
	if ( viewer )
	{
		if ( gangedCheckBox->isChecked() )
		{
			int old_last = lastCamera;
			
			for ( int i = 0; i < numCamerasSpinBox->value(); i++ )
			{
				lastCamera = i + 1;
				resetSliders( lastCamera );
				viewer->setXFocalLength( i, value / 100.f );
			}
			lastCamera = old_last;
		}
		else
			viewer->setXFocalLength( cameraSelectSpinBox->value() - 1, value / 100.f );
	}
}


void
Console::setYFocalLength( int value )
{
	if ( viewer )
	{
		if ( gangedCheckBox->isChecked() )
		{
			int old_last = lastCamera;
			
			for ( int i = 0; i < numCamerasSpinBox->value(); i++ )
			{
				lastCamera = i + 1;
				resetSliders( lastCamera );
				viewer->setYFocalLength( i, value / 100.f );
			}
			lastCamera = old_last;
		}
		else
			viewer->setYFocalLength( cameraSelectSpinBox->value() - 1, value / 100.f );
	}
}


void 
Console::setDistortionEnabled( bool value )
{
	if ( viewer )
	{
		if ( gangedCheckBox->isChecked() )
		{
			int old_last = lastCamera;
			
			for ( int i = 0; i < numCamerasSpinBox->value(); i++ )
			{
				lastCamera = i + 1;
				resetSliders( lastCamera );
				viewer->enableDistortion( i, value );
			}
			lastCamera = old_last;
		}
		else
			viewer->enableDistortion( cameraSelectSpinBox->value() - 1, value );

		viewer->setUpSlaves();
	}
}

void 
Console::setGangMotion( bool value )
{
	if ( viewer )
	{
		disableMotionButton->disconnect();
		displayBlankingButton->disconnect();

		if ( value )
		{
			connect( disableMotionButton, SIGNAL( clicked() ), this, SLOT( motionAndBlankingCallback() ) );
			connect( displayBlankingButton, SIGNAL( clicked() ), this, SLOT( motionAndBlankingCallback() ) );
			// If we have a callback it mean we are hooked to the ball
			// so, motion status sets the value
			if ( callback )
			{
				// If motion is enabled and the screen is blank or motion is disabled
				// and the screen is active, flip the display blanking
				if ( (!callback->motion_state() && _blankDisplay) || (callback->motion_state() && !_blankDisplay) )
					setDisplayBlankingCallback();
			}
		}
		else
		{
			connect( disableMotionButton, SIGNAL( clicked() ), this, SLOT( disableMotionCallback() ) );
			connect( displayBlankingButton, SIGNAL( clicked() ), this, 
						SLOT( setDisplayBlankingCallback() ) );
		}
	}
}

void
Console::playForwardCallback()
{
	if ( !_playForward || _playBackward )
	{
		if ( _playBackward )
		{
			QIcon icon2;
			icon2.addFile(QString::fromUtf8(":/images/Back.png"), QSize(), QIcon::Normal, QIcon::Off);
			playBackwardsButton->setIcon(icon2);
			_playBackward = false;
		}
		
		_playForward = true;
		QIcon icon2;
		icon2.addFile(QString::fromUtf8(":/images/Play_Active.png"), QSize(), QIcon::Normal, QIcon::Off);
		playForwardButton->setIcon(icon2);
	}
	else if ( _playForward )
	{
		_playForward = false;
		QIcon icon2;
		icon2.addFile(QString::fromUtf8(":/images/Play.png"), QSize(), QIcon::Normal, QIcon::Off);
		playForwardButton->setIcon(icon2);
	}
	
	_model->set_play_forward( _playForward, strideLength->value() );
}

void
Console::playBackwardsCallback()
{
	if ( !_playBackward || _playForward )
	{
		if ( _playForward )
		{
			QIcon icon2;
			icon2.addFile(QString::fromUtf8(":/images/Play.png"), QSize(), QIcon::Normal, QIcon::Off);
			playForwardButton->setIcon(icon2);
			_playForward = false;
		}
		
		_playBackward = true;
		QIcon icon2;
		icon2.addFile(QString::fromUtf8(":/images/Back_Active.png"), QSize(), QIcon::Normal, QIcon::Off);
		playBackwardsButton->setIcon(icon2);
	}
	else if ( _playBackward )
	{
		_playBackward = false;
		QIcon icon2;
		icon2.addFile(QString::fromUtf8(":/images/Back.png"), QSize(), QIcon::Normal, QIcon::Off);
		playBackwardsButton->setIcon(icon2);
	}
	
	_model->set_play_backward( _playBackward, strideLength->value() );
}

void
Console::rewindCallback()
{
	timeSlider->setValue( 0 );
}

void
Console::recordCallback()
{
	if ( _recording )
	{
		QIcon icon2;
		icon2.addFile(QString::fromUtf8(":/images/Record.png"), QSize(), QIcon::Normal, QIcon::Off);
		recordButton->setIcon( icon2 );
		_recording = false;
		viewer->record( false );
	}
	else
	{
		QIcon icon2;
		icon2.addFile(QString::fromUtf8(":/images/Record_Active.png"), QSize(), QIcon::Normal, QIcon::Off);
		recordButton->setIcon( icon2 );
		_recording = true;
		QString dir_name = QFileDialog::getExistingDirectory( this, tr("Set Directory for Recorded Files"),
		                                                 "", QFileDialog::ShowDirsOnly );
		if ( !dir_name.isEmpty() )
		{
			CameraTraceUpdateCallback* ct = dynamic_cast< CameraTraceUpdateCallback* >( _model->camera_callback().get() );
			viewer->set_recording_directory( dir_name.toStdString(), "foo" );
			viewer->record( true, ct );
		}

		_last_frame_index = timeSlider->value();
	}

	playForwardCallback();
}

void
Console::timeSliderCallback( int value )
{
	CameraTraceUpdateCallback* ct = dynamic_cast< CameraTraceUpdateCallback* >( _model->camera_callback().get() );
	ct->setTimeIndex( value );
	timeLabel->setValue( ct->time() );
}

void
Console::setCenter( )
{
	if ( viewer )
	{
		if ( gangedCheckBox->isChecked() )
		{
			int old_last = lastCamera;
			
			for ( int i = 0; i < numCamerasSpinBox->value(); i++ )
			{
				lastCamera = i + 1;
				resetSliders( lastCamera );
				viewer->setCenter( i, xCenterDistortsBox->isChecked(),
				                   -xCenterSlider->value() / 100.f, -yCenterSlider->value()/ 100.f );
			}
			lastCamera = old_last;
		}
		else
			viewer->setCenter( cameraSelectSpinBox->value() - 1, xCenterDistortsBox->isChecked(),
			                   -xCenterSlider->value() / 100.f, -yCenterSlider->value() / 100.f );

		viewer->setUpSlaves();
	}
}

void
Console::doImageOpen()
{
	QString fileName = QFileDialog::getOpenFileName( this, tr("Open Image File"),
	                                                 "", tr("Image Files (*.jpg *.png *.tif *.bmp);; All Files (*.*)"));
	if (!fileName.isEmpty())
	{
		QFileInfo info( fileName );
		
		imageCalibrationButton->setText( info.fileName() );
		imageFile = fileName.toStdString();
		useImage = true;
	}
}

void
Console::doOSGOpen()
{
	QString directory( "" );
   char* osg_path = getenv( "OSG_FILE_PATH" );
   if ( osg_path != NULL )
	   directory = osg_path;
   
	QString fileName = QFileDialog::getOpenFileName( this, tr("Open OSG File"),
	                                                 directory, tr("OSG Files (*.osg);; All Files (*.*)"));
	if (!fileName.isEmpty())
	{
		QFileInfo info( fileName );
		
		osgDataButton->setText( info.fileName() );
		osgFile = fileName.toStdString();
	}
}

void
Console::enableFileExportCallback( bool yes_or_no )
{ 
	_model->set_export_status( yes_or_no );
	if ( yes_or_no )
	{
		if ( exportFileNameBox->text().count() == 0 )
		{
			if ( viewer ) 
			{
				QString s( _model->scene_file_name() );
				s.replace( ".dae", "" );
				setExportFilename( s );
			}
			else
				setExportFilename( QString( "foo" ) );
		}
		else
			setExportFilename( exportFileNameBox->text() );
	}
}

void
Console::doSetExportFileName()
{
	QString directory( "" );
   
	QString fileName = QFileDialog::getSaveFileName( this, tr("Open Export File"),
	                                                 directory, tr("All Files (*.*)"));
	if (!fileName.isEmpty())
	{
		QFileInfo info( fileName );
		
		exportFileNameBox->setText( info.filePath() );
//		osgFile = fileName.toStdString();
	}
}

void
Console::resetDisplay()
{
	_graphicsView[ cameraSelectSpinBox->value() - 1 ]->reset();
}

void
Console::distributeHorizontally()
{
	_graphicsView[ cameraSelectSpinBox->value() - 1 ]->distributeHorizontally();
}

void
Console::distributeVertically()
{
	_graphicsView[ cameraSelectSpinBox->value() - 1 ]->distributeVertically();
}

void
Console::smoothDisplay()
{
	_graphicsView[ cameraSelectSpinBox->value() - 1 ]->smooth();
}

void
Console::linearizeEdges()
{
	_graphicsView[ cameraSelectSpinBox->value() - 1 ]->linearizeEdges();
}

void
Console::setFramePacking( bool value )
{
	viewer->set_frame_packing( value );
}

void
Console::setAmbientColorCallback()
{
	QColor color = QColorDialog::getColor( QColor( ambientLabel->text() ), this );
	if (color.isValid()) 
		setAmbientColor( color );
}

void
Console::setAmbientColor( QColor color )
{
	ambientLabel->setText(color.name());
	ambientLabel->setPalette(QPalette(color));
	ambientLabel->setAutoFillBackground(true);
	osg::Vec4 amb = osg::Vec4( color.red()/255.0, color.green()/255.0, color.blue()/255.0, 1.0 );
  
	if ( viewer )
		viewer->setAmbientColor( amb );
}

void
Console::setBackgroundColorCallback()
{
	QColor color = QColorDialog::getColor( QColor( backgroundLabel->text() ), this );
	if (color.isValid())
		setBackgroundColor( color );
}

void
Console::setBackgroundColor( QColor color )
{
	backgroundLabel->setText(color.name());
	backgroundLabel->setPalette(QPalette(color));
	backgroundLabel->setAutoFillBackground(true);
	_clearColor = osg::Vec4( color.red()/255.0, color.green()/255.0, color.blue()/255.0, 1.0 );
  
	if ( viewer )
		viewer->setClearColor( _clearColor );
}

void
Console::setDiffuseColorCallback()
{
	QColor color = QColorDialog::getColor( QColor( diffuseLabel->text() ), this );
	if (color.isValid())
		setDiffuseColor( color );
}

void
Console::setDiffuseColor( QColor color )
{
	diffuseLabel->setText(color.name());
	diffuseLabel->setPalette(QPalette(color));
	diffuseLabel->setAutoFillBackground(true);
	osg::Vec4 diff = osg::Vec4( color.red()/255.0, color.green()/255.0, color.blue()/255.0, 1.0 );
  
	if ( viewer )
		viewer->setDiffuseColor( diff );
}

void
Console::setDiffusePower( int power )
{
	if ( diffusePowerLevel->value() != power )
		diffusePowerLevel->setValue( power );

	if ( viewer )
		viewer->setDiffusePower( diffusePowerLevel->value() );
}

void
Console::setGlobalBrightnessCallback()
{
	QColor color = QColorDialog::getColor( QColor( brightnessLabel->text() ), this );
	if (color.isValid())
	{
		_brightness[ cameraSelectSpinBox->value()-1 ] = color;
		setGlobalBrightness( cameraSelectSpinBox->value() - 1, color );
	}
}

void
Console::setDisplayBlankingCallback()
{
	blankDisplay();
}

void
Console::setGlobalBrightnessTarget( int value )
{
	setGlobalBrightness( value-1, _brightness[ value-1 ] );
}

void
Console::setGlobalBrightness( int i, QColor color )
{
	brightnessLabel->setText(color.name());
	brightnessLabel->setPalette(QPalette(color));
	brightnessLabel->setAutoFillBackground(true);
	osg::Vec4 col = osg::Vec4( color.red()/255.0, color.green()/255.0, color.blue()/255.0, 1.0 );
  
	if ( viewer )
		viewer->setGlobalBrightness( i, col );
}

// The sense of the FOV and the FOV offset are backwards from the widget names
// and controls. In this case, the FOV slider sets the rotated camera offset
// which in turn sets the actual FOV. Confusing!

void
Console::setFieldOfView( int fov )
{
	if ( viewer )
	{
	   if ( rotatedCamerasCheckBox->isChecked() )
		   viewer->setFieldOfViewOffset( (float)fov / viewer->numberOfCameras() );
	   else
		   viewer->setFieldOfView( (float)fov );
	}
}

void
Console::setFieldOfViewOffset( int fov_offset )
{
	fovOffsetSpinBox->setValue( fov_offset / 10.f );
}

void
Console::setFieldOfViewOffset( double fov_offset )
{
	fovOffsetSlider->setValue( (int)( fov_offset * 10.f ) );
   if ( viewer )
	   viewer->setFieldOfView( (float)fov_offset );
}

void
Console::setRotatedCameras( bool yes_or_no )
{
   if ( viewer )
	   viewer->setRotatedCameras( yes_or_no );
	setFieldOfView( (float)fovSlider->value() );
	if ( yes_or_no )
		setFieldOfViewOffset( fovOffsetSpinBox->value() );
}

void
Console::setIndicatorSize( int new_size )
{
   if ( viewer )
	   viewer->setIndicatorSize( new_size );
}

void
Console::setIndicatorBorderSize( int new_size )
{
   if ( viewer )
	   viewer->setIndicatorBorderSize( new_size );
}

void
Console::setIndicatorPosition( int ignored )
{
	osg::Vec2 v( indicatorHorizontalPositionSlider->value(), 
			indicatorVerticalPositionSlider->value());
   if ( viewer )
	viewer->setIndicatorPosition( v );
}

void
Console::setIndicatorScreen( int screen )
{
   if ( viewer )
	   viewer->setIndicatorScreen( screen - 1 );
}

void
Console::setIndicatorMode( int index )
{
   if ( viewer )
	   viewer->setIndicatorMode( index );
}

void
Console::setCameraMaximums( int newMax )
{
   cameraSelectSpinBox->setMaximum( newMax );
   screenSelectSpinBox->setMaximum( newMax );
   if ( newMax > _brightness.size() )
	   for ( int i = _brightness.size(); i < newMax; i++ )
		   _brightness.push_back( _brightness.back() );
   
   enableBlankingWidgets( false );

   if ( viewer )
	   if ( viewer->numberOfCameras() == newMax )
		   enableBlankingWidgets( true );

   if ( newMax > _graphicsView.size() )
   {
	   for ( int i = _graphicsView.size(); i < newMax; i++ )
	   {
		   QWidget* widget = new QWidget();
		   displayWidget->addWidget( widget );
	   }
   
	   for ( int i = _graphicsView.size(); i < newMax; i++ )
	   {
		   // Offset the displayWidget by 1 since the camera index is 1-based
		   QVBoxLayout* layout = new QVBoxLayout( displayWidget->widget( i+1 ) );
		   GraphWidget* gw = new GraphWidget( displayWidget->widget( i+1 ) );
		   _graphicsView.push_back( gw );
		   layout->addWidget( gw );
	   }
   }
   
   for ( int i = _frame.size(); i < newMax; i++ )
	{
		_frame.push_back( std::pair< int, int >( 0, 0 ) );
		_distort.push_back( triple< bool, int, int >( true, 100, 100 ) );
		_center.push_back( std::pair< int, int >( 50, 50 ) );
	}

}

void
Console::resetSliders( int value )
{
	_frame[ lastCamera - 1 ].first = horizontalBlankingSlider->value();
	_frame[ lastCamera - 1 ].second = horizontalBlankingSlider_2->value();
	
	_distort[ lastCamera - 1 ].first = enableDistortion->isChecked();
	_distort[ lastCamera - 1 ].second = focalLengthSlider_y->value();
	_distort[ lastCamera - 1 ].third = focalLengthSlider_x->value();
	
	_center[ lastCamera - 1 ].first = xCenterSlider->value();
	_center[ lastCamera - 1 ].second = yCenterSlider->value();
	
	horizontalBlankingSlider->setValue( _frame[ value - 1 ].first );
	horizontalBlankingSlider_2->setValue( _frame[ value - 1 ].second );

	enableDistortion->setChecked( _distort[ value - 1 ].first );
	focalLengthSlider_y->setValue( _distort[ value - 1 ].second );
	focalLengthSlider_x->setValue( _distort[ value - 1 ].third );

	xCenterSlider->setValue( _center[ value - 1 ].first );
	yCenterSlider->setValue( _center[ value - 1 ].second );

	updateViewportLeft( horizontalBlankingSlider->value() );
	updateViewportRight( horizontalBlankingSlider_2->value() );
	
	lastCamera = value;
}

void
Console::startViewer()
{
	if ( viewer )
		delete viewer;

	_frame.erase( _frame.begin(), _frame.end() );
	_distort.erase( _distort.begin(), _distort.end() );
	_center.erase( _center.begin(), _center.end() );

	for ( int i = 0; i < numCamerasSpinBox->value(); i++ )
	{
		_frame.push_back( std::pair< int, int >( 0, 0 ) );
		_distort.push_back( triple< bool, int, int >( true, 100, 100 ) );
		_center.push_back( std::pair< int, int >( 50, 50 ) );
	}

	viewer = new ViewingWindowQt( numCamerasSpinBox->value(), _graphicsView, arguments, _model );
	viewer->use_custom_widget( useCustomGLWidget->isChecked() );
	
	viewer->initializeCameras( numCamerasSpinBox->value(), 
	                           numDisplaysSpinBox->value(),
	                           startingDisplaySpinBox->value() - 1,
	                           _trace_path != 0 );
	viewer->addEventHandler( new KeyboardEventHandler( this ) );

	viewer->setClearColor( _clearColor );
	viewer->setFieldOfViewOffset( (float)fovSlider->value() / numCamerasSpinBox->value() );
	viewer->setFieldOfView( (float)fovOffsetSpinBox->value() );

	for ( int i = 0; i < numCamerasSpinBox->value(); i++ )
		_frame[i].second = viewer->width();

	horizontalBlankingSlider->setMaximum( viewer->width() );
	horizontalBlankingSpinBox->setMaximum( viewer->width() );
	horizontalBlankingSlider_2->setMaximum( viewer->width() );
	horizontalBlankingSpinBox_2->setMaximum( viewer->width() );
	horizontalBlankingSlider_2->setValue( viewer->width() );

	enableBlankingWidgets( true );

	int screen = cameraSelectSpinBox->value();

	for ( int i = 1; i <= numCamerasSpinBox->value(); i++ )
		setGlobalBrightnessTarget( i );

	setGlobalBrightnessTarget( screen );

	viewer->setXFocalLength( cameraSelectSpinBox->value() - 1, focalLengthSlider_x->value() / 100.f );
	viewer->setYFocalLength( cameraSelectSpinBox->value() - 1, focalLengthSlider_y->value() / 100.f );
	viewer->setCenter( cameraSelectSpinBox->value() - 1, xCenterDistortsBox->isChecked(),
		-xCenterSlider->value() / 100.f, -yCenterSlider->value() / 100.f );

	indicatorHorizontalPositionSlider->setValue( ( numCamerasSpinBox->value() * viewer->width() ) - 
		numCamerasSpinBox->value() * ( indicatorSizeSlider->value() + borderSizeSlider->value() ) );
	indicatorVerticalPositionSlider->setValue( borderSizeSlider->value() );

	if ( imageRadioButton->isChecked() && useImage )
	{
		viewer->loadImage( imageFile );
		viewer->home();
	}
	else if ( osgRadioButton->isChecked() )
	{
		viewer->loadOSG( osgFile );
		viewer->home();
	}

	networkConnectButton->setEnabled( true );
	actionOpen->setEnabled( true );
	displayBlankingButton->setEnabled( true );
	gangMotionCheckBox->setEnabled( true );
	disableMotionButton->setEnabled( true );
	
	viewer->set_output_rate( outputRateSpinBox->value() );

	if ( _trace_path != 0 )
		viewer->set_movie_mode( true );
}

void
Console::updateViewportLeft( int value )
{
	if ( horizontalBlankingSlider->value() <= horizontalBlankingSlider_2->value() )
		updateViewport( cameraSelectSpinBox->value(),
		                horizontalBlankingSlider->value(), 
		                horizontalBlankingSlider_2->value() );
	else
		horizontalBlankingSlider_2->setValue( horizontalBlankingSlider->value() );
}

void
Console::updateViewportRight( int value )
{
	if ( horizontalBlankingSlider->value() <= horizontalBlankingSlider_2->value() )
		updateViewport( cameraSelectSpinBox->value(),
		                horizontalBlankingSlider->value(), 
		                horizontalBlankingSlider_2->value() );
	else
		horizontalBlankingSlider->setValue( horizontalBlankingSlider_2->value() );
}

void
Console::enableBlankingWidgets( bool onOrOff )
{
	cameraSelectSpinBox->setEnabled( onOrOff );
   horizontalBlankingSlider->setEnabled( onOrOff );
   horizontalBlankingSpinBox->setEnabled( onOrOff );
   horizontalBlankingSlider_2->setEnabled( onOrOff );
   horizontalBlankingSpinBox_2->setEnabled( onOrOff );
   horizontalBlankingSlider_2->setEnabled( onOrOff );
}

void
Console::calibrateX()
{
	float rx, ry, rz;

	if ( !_calibrating && !computeAverage->isChecked() )
	{
		_calibrationRuns[0] = 0;
		_Vfwdf = 0.f;
	}

	doCalibration( calibrateXButton, &rx, &ry, &rz );
	if ( !_calibrating )
	{
		char val[256];
		_calibrationRuns[0] += 1;
		float count = (float)_calibrationRuns[0];

		_Vfwdf = _Vfwdf * (count-1.f)/count + rx/count;
		if ( abs( _Vfwdf ) < 1.f )
			_Vfwdf = 1.f;
		sprintf( val, "%f", fabs(_Vfwdf ) );
		calibrateXValue->setText( val );
		calibrateYButton->setEnabled( true );
		calibrateZButton->setEnabled( true );
	}
	else
	{
		calibrateYButton->setEnabled( false );
		calibrateZButton->setEnabled( false );
	}
}

void
Console::calibrateY()
{
	float rx, ry, rz;

	if ( !_calibrating && !computeAverage->isChecked() )
	{
		_calibrationRuns[1] = 0;
		_Omegaf = 0.f;
	}

	doCalibration( calibrateYButton, &rx, &ry, &rz );
	if ( !_calibrating )
	{
		char val[256];
		_calibrationRuns[1] += 1;
		float count = (float)_calibrationRuns[1];

		_Omegaf = _Omegaf * (count-1.f)/count + rz/count;
		if ( abs( _Omegaf ) < 1.f )
			_Omegaf = 1.f;
		sprintf( val, "%f", fabs( _Omegaf ) );
		calibrateYValue->setText( val );
		calibrateXButton->setEnabled( true );
		calibrateZButton->setEnabled( true );
	}
	else
	{
		calibrateXButton->setEnabled( false );
		calibrateZButton->setEnabled( false );
	}
}


void
Console::calibrateZ()
{
	float rx, ry, rz;

	if ( !_calibrating && !computeAverage->isChecked() )
	{
		_calibrationRuns[2] = 0;
		_Vssf = 0.f;
	}

	doCalibration( calibrateZButton, &rx, &ry, &rz );
	if ( !_calibrating )
	{
		char val[256];
		_calibrationRuns[2] += 1;
		float count = (float)_calibrationRuns[2];

		_Vssf = _Vssf * (count-1.f)/count + ry/count;
		if ( abs( _Vssf ) < 1.f )
			_Vssf = 1.f;
		sprintf( val, "%f", fabs( _Vssf )  );
		calibrateZValue->setText( val );
		calibrateYButton->setEnabled( true );
		calibrateXButton->setEnabled( true );
	}
	else
	{
		calibrateYButton->setEnabled( false );
		calibrateXButton->setEnabled( false );
	}
}

void 
Console::doCalibration( QPushButton* button, float* rx, float* ry, float* rz )
{
	Communicator* new_comm;

	if ( connected ) 
	{
		connected = false;
	}
	
	if( !_calibrating )
	{
		if ( TCP_radioBtn->isChecked() )
		{
			new_comm = new TCPCommunicator( hostname(), port(), retries() );
		}
		else if ( UDP_radioBtn->isChecked() )
		{
			new_comm = new UDPCommunicator( hostname(), port(), retries() );
		}

		if ( new_comm->success )
		{
			delete comm;      
			comm = new_comm;
			comm->reset();
      
			connected = true;
		}

		comm->read();
		_calibrating = true;
		button->setText( "Stop Calibration ..." );
	}
	else
	{
		_calibrating = false;
		button->setText( "Start Calibration ..." );
		comm->read();

		if ( comm->message_length() > 0 )
		{
			try
			{
				float Vfwd, Vss, Omega;
				sscanf( comm->data(), "%f,%f,%f", &Vfwd, &Vss, &Omega );
				*rx = Vfwd;
				*ry = Vss;
				*rz = Omega;

				comm->close();
			}
			catch (std::exception& e)
			{
				std::cout << "Caught exception\n";
				// Reset the connection
				comm->close();
				comm->reset();
			}
		}
	}
}

void
Console::enableEditing( bool onOrOff )
{
	// Invert, for when we want to edit, we need to turn read only off
	calibrateXValue->setReadOnly( !onOrOff );
	calibrateYValue->setReadOnly( !onOrOff );
	calibrateZValue->setReadOnly( !onOrOff );
}

void
Console::updateCalibrationValuesCallback()
{
	from_string<float>(_Vfwdf, std::string( calibrateXValue->text().toStdString() ), std::dec);
	from_string<float>(_Omegaf, std::string( calibrateYValue->text().toStdString() ), std::dec);
	from_string<float>(_Vssf, std::string( calibrateZValue->text().toStdString() ), std::dec);

	_Vfwdf /= calibrateXGain->value();
	_Omegaf /= calibrateYGain->value();
	_Vssf /= calibrateZGain->value();

	if ( callback != 0 )
		callback->updateScaleFactors( _Vfwdf, _Omegaf, _Vssf );
}

void
Console::enableVelocitySmoothing( bool yes_or_no )
{
	if ( _model->is_tracking() )
	{
		_model->camera_callback()->enable_velocity_smoothing( yes_or_no );
		_model->camera_callback()->interval_for_velocity_smoothing( velocitySmoothingInterval->value() );
	}
}

void
Console::setIntervalForVelocitySmoothing( int value )
{
	if ( _model->is_tracking() )
		_model->camera_callback()->interval_for_velocity_smoothing( value );
}

void
Console::enablePreBlendHeadingSmoothing( bool yes_or_no )
{
	if ( _model->camera_callback() )
	{
		_model->camera_callback()->enable_input_heading_smoothing( yes_or_no );
		if ( _model->is_tracking() )
			_model->camera_callback()->interval_for_heading_smoothing( inputSmoothingInterval->value(),
			                                                           intermediarySmoothingInterval->value(),
			                                                           outputSmoothingInterval->value() );
	}
}

void
Console::enableIntermediaryHeadingSmoothing( bool yes_or_no )
{
	if ( _model->camera_callback() )
	{
		_model->camera_callback()->enable_intermediary_heading_smoothing( yes_or_no );
		if ( _model->is_tracking() )
			_model->camera_callback()->interval_for_heading_smoothing( inputSmoothingInterval->value(),
			                                                           intermediarySmoothingInterval->value(),
			                                                           outputSmoothingInterval->value() );
	}
}

void
Console::enablePostBlendHeadingSmoothing( bool yes_or_no )
{
	if ( _model->camera_callback() )
	{
		_model->camera_callback()->enable_output_heading_smoothing( yes_or_no );
		if ( _model->is_tracking() )
			_model->camera_callback()->interval_for_heading_smoothing( inputSmoothingInterval->value(),
			                                                           intermediarySmoothingInterval->value(),
			                                                           outputSmoothingInterval->value() );
	}
}

void
Console::setIntervalForPreBlendingSmoothing ( int value )
{
	if ( _model->is_tracking() )
		_model->camera_callback()->interval_for_heading_smoothing( value,
		                                                           intermediarySmoothingInterval->value(),
		                                                           outputSmoothingInterval->value() );
}

void
Console::setIntervalForIntermediarySmoothing ( int value )
{
	if ( _model->is_tracking() )
		_model->camera_callback()->interval_for_heading_smoothing( inputSmoothingInterval->value(),
		                                                           value,
		                                                           outputSmoothingInterval->value() );
}

void
Console::setIntervalForPostBlendingSmoothing( int value )
{
	if ( _model->is_tracking() )
		_model->camera_callback()->interval_for_heading_smoothing( inputSmoothingInterval->value(),
		                                                           intermediarySmoothingInterval->value(),
		                                                           value );
}

void 
Console::setSerialPortName()
{
	if ( viewer )
	{
		viewer->clearSerialPort();
		if ( portNameControl->text().count() > 0 )
			viewer->setSerialPort( portNameControl->text().toStdString() );
	}
}

void 
Console::switchScene( int id )
{
	QColor color;
	bool resetConnection = connected;
	
	viewer->resetManipulatorMode();

	if ( _model->is_tracking() ) 
	{
		// Turn off connection to remoteDataServer if active
		if ( connected )
			setupConnection();
		
	}

	Config_Memento* cm = _model->current_configuration();
	if ( !cm->initialized() )
		cm->initialize( this );
	
	save_config( cm );

	_model->switch_scene( id );

	cm = _model->current_configuration();
	if ( !cm->initialized() )
		cm->initialize( this );
	
	load_config( cm );

	viewer->setTrackNode();

	headingStatusButton->setEnabled( true );

	viewer->getCameraManipulator()->setHomePosition( _model->start_center( 0 ), 
	                                                 _model->start_direction( 0 ), 
	                                                 osg::Vec3f( 0, 0, 1 ));

	viewer->setManipulatorToTrackingMode();
	resetPosition();

/*
	color = QColor( ambientLabel->text() );
	setAmbientColor( color );

	color = QColor( diffuseLabel->text() );
	setDiffuseColor( color );
	setDiffusePower( diffusePowerLevel->value() );

	color = QColor( backgroundLabel->text() );
	setBackgroundColor( color );
*/
	// Reenable connection to remoteDataServer if it was active
	if ( resetConnection )
		setupConnection();
}

void 
Console::select_scene()
{
	bool resetConnection = connected;
	
	viewer->resetManipulatorMode();

	if ( _model->is_tracking() ) 
	{
		// Turn off connection to remoteDataServer if active
		if ( connected )
			setupConnection( false );
		
	}

	move_to_start();
	
	// Reenable connection to remoteDataServer if it was active
	if ( resetConnection )
	{
		setupConnection( false );
		if ( thresholdTurningEnabled->isChecked() )
		{
			float scale_fac = compute_turning_mixture( thresholdTurningSlider->value() );
			_model->camera_callback()->set_turning_mixture( scale_fac );
		}
		else
		{
			_model->camera_callback()->set_turning_mixture( 0.f );
		}

		_model->camera_callback()->enable_velocity_smoothing( velocitySmoothingEnabled->isChecked() );
		_model->camera_callback()->enable_input_heading_smoothing( inputSmoothingEnabled->isChecked() );
		_model->camera_callback()->enable_output_heading_smoothing( outputSmoothingEnabled->isChecked() );
		if ( _model->is_tracking() )
		{
			_model->camera_callback()->interval_for_heading_smoothing( inputSmoothingInterval->value(),
			                                                           intermediarySmoothingInterval->value(),
			                                                           outputSmoothingInterval->value() );
			_model->camera_callback()->interval_for_velocity_smoothing( velocitySmoothingInterval->value() );
		}
	}
}

void
Console::autoHeadingCallback()
{
	if ( _useAutoHeading )
		headingStatusButton->setText( "Manual Heading Enabled" );
	else
		headingStatusButton->setText( "Auto Heading Enabled" );

	_useAutoHeading = !_useAutoHeading;
	_model->use_rotations( _useAutoHeading );
}

/******************************************************************************
PURPOSE: enableThresholdTurning - Callback for enabling threshold based turning
INPUTS:  bool value - passed from Qt callback on thresholdTurningEnabled
OUTPUTS:  None
RETURNS: None
******************************************************************************/
void 
Console::enableThresholdTurning( bool value )
{	
	if ( _model->is_tracking() )
	{
		if ( value  )
		{
			float scale_fac = compute_turning_mixture( thresholdTurningSlider->value() );
			_model->camera_callback()->set_turning_mixture( scale_fac );
			_ratio_line = new QCPItemLine( _plot_ratio_widget );
			_plot_ratio_widget->addItem( _ratio_line );
			_smoothed_ratio_line = new QCPItemLine( _plot_ratio_widget );
			_plot_ratio_widget->addItem( _smoothed_ratio_line );
			_velocity_ratio_line = new QCPItemLine( _plot_velocity_widget );
			_plot_velocity_widget->addItem( _velocity_ratio_line );
			_smoothed_velocity_ratio_line = new QCPItemLine( _plot_velocity_widget );
			QPen pen( Qt::green );
			pen.setWidth( 2 );
			_smoothed_velocity_ratio_line->setPen( pen );
			_smoothed_velocity_ratio_line->start->setCoords( 0.f, 0.f );
			_smoothed_velocity_ratio_line->end->setCoords( 0.f, _plot_velocity_max_y );
			_plot_velocity_widget->addItem( _smoothed_velocity_ratio_line );
			_smoothed_ratio_line->setPen( pen );
			_smoothed_ratio_line->start->setCoords( 0.f, 0.f );
			_smoothed_ratio_line->end->setCoords( 0.f, _plot_ratio_max_y );
			_plot_ratio_widget->addItem( _smoothed_ratio_line );

			_plot_timer = new QTimer( this );
			connect( _plot_timer, SIGNAL( timeout() ), this, SLOT( update_ratio_line() ) );
			_plot_timer->start( 10 );
		}
		else
		{
			_model->camera_callback()->set_turning_mixture( 0.f );
			// It appears that _ratio_line is deleted when removed.
			if ( _plot_timer )
			{
				_plot_ratio_widget->removeItem( _ratio_line );
				_plot_ratio_widget->removeItem( _smoothed_ratio_line );
				_plot_velocity_widget->removeItem( _velocity_ratio_line );
				_plot_velocity_widget->removeItem( _smoothed_velocity_ratio_line );
				
				_plot_timer->stop();
				delete _plot_timer;
				_plot_timer = 0;
				
				_plot_ratio_widget->replot();
				_plot_velocity_widget->replot();
			}
		}
	}
}

void
Console::updatePlot( int row, int column )
{
	updatePlot();
}

void
Console::updateVelocityPlot( int row, int column )
{
	updateVelocityPlot();
}

void
Console::initializePlotWidgets( std::vector< double > x, std::vector< double > y,
                                std::vector< double > vx, std::vector< double > vy, 
                                bool create_plot )
{
	// Threshold Turning widget
	// Ensure there is one threshold display widget.
	
	if ( ratioDisplayWidget->count() == 0 )
	{
		QWidget* widget = new QWidget();
		ratioDisplayWidget->addWidget( widget );
	}
	else
		// Remove all current layout items in `ratioDisplayWidget'
		while ( ratioDisplayWidget->count() > 1 )
		{
			QWidget* widget = ratioDisplayWidget->currentWidget();
			ratioDisplayWidget->removeWidget( widget );
		}

	if ( velocityDisplayWidget->count() == 0 )
	{
		QWidget* widget = new QWidget();
		velocityDisplayWidget->addWidget( widget );
	}
	else
		// Remove all current layout items in `ratioDisplayWidget'
		while ( velocityDisplayWidget->count() > 1 )
		{
			QWidget* widget = velocityDisplayWidget->currentWidget();
			velocityDisplayWidget->removeWidget( widget );
		}

	// Initialize plot values
	plotTableWidget->setRowCount( x.size() );
	for( int i = 0; i < x.size(); i++ )
	{
		QTableWidgetItem *item = new QTableWidgetItem;
		item->setData( Qt::EditRole, x[i] );    
		plotTableWidget->setItem( i, 0, item );

		item = new QTableWidgetItem;
		item->setData( Qt::EditRole, degrees( y[i] ) );    
		plotTableWidget->setItem( i, 1, item );	
	}
	
	plotVelocityTableWidget->setRowCount( vx.size() );
	for( int i = 0; i < vx.size(); i++ )
	{
		QTableWidgetItem *item = new QTableWidgetItem;
		item->setData( Qt::EditRole, vx[i] );    
		plotVelocityTableWidget->setItem( i, 0, item );

		item = new QTableWidgetItem;
		item->setData( Qt::EditRole, vy[i] );    
		plotVelocityTableWidget->setItem( i, 1, item );	
	}
	
	// Create the plot widgets
	if ( create_plot )
	{
		_plot_ratio_widget = new QCustomPlot( ratioDisplayWidget->widget( 0 ) );
		QVBoxLayout* layout = new QVBoxLayout( ratioDisplayWidget->widget( 0 ) );
		layout->addWidget( _plot_ratio_widget );

		_plot_velocity_widget = new QCustomPlot( velocityDisplayWidget->widget( 0 ) );
		QVBoxLayout* layout2 = new QVBoxLayout( velocityDisplayWidget->widget( 0 ) );
		layout2->addWidget( _plot_velocity_widget );
	}
	
	updateVelocityPlot();
}

void
Console::updatePlot()
{
	bool valid = _plot_ratio_widget && plotTableWidget->rowCount() > 0;
	for( int i = 0; i < plotTableWidget->rowCount() && valid; i++ )
	{
		if ( plotTableWidget->item( i, 0 ) == 0 || plotTableWidget->item( i, 1 ) == 0 )
			valid = false;
		else
		{
			bool ok1, ok2;
			double d = plotTableWidget->item( i, 0 )->data( Qt::EditRole ).toDouble( &ok1 );
			d = plotTableWidget->item( i, 1 )->data( Qt::EditRole ).toDouble( &ok2 );
			if ( !( ok1 && ok2 ) )
				valid = false;
		}
	}
	
	if ( valid )
	{
		if ( _plot_ratio_widget->graphCount() > 0 )
			_plot_ratio_widget->clearGraphs();
	
		_plot_ratio_widget->addGraph();
		_plot_ratio_widget->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
		_plot_ratio_widget->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); 

		int rows = plotTableWidget->rowCount();
		int v_rows = plotVelocityTableWidget->rowCount();
		QVector<double> x, y, rad_y;
		x.resize( rows );
		y.resize( rows );
		rad_y.resize( rows );

		_plot_ratio_max_y = -100000.f;
		_plot_velocity_max_y = -100000.f;
		
		for ( int i = 0; i < rows; ++i )
		{
			x[i] = plotTableWidget->item( i, 0 )->data( Qt::EditRole ).toDouble();
			y[i] = plotTableWidget->item( i, 1 )->data( Qt::EditRole ).toDouble();
			rad_y[i] = radians( y[i] );
			
			_plot_ratio_max_y = std::max( _plot_ratio_max_y, y[i] );			
		}

		_plot = Graph_Evaluator( x.toStdVector(), rad_y.toStdVector() );

		if ( _model->is_tracking() )
		{
			_model->camera_callback()->set_plot_data( _plot, _v_plot );
		}

		// configure right and top axis to show ticks but no labels (could've also just called _plot_ratio_widget->setupFullAxesBox):
		_plot_ratio_widget->xAxis->setScaleType( QCPAxis::stLogarithmic );
		_plot_ratio_widget->xAxis2->setVisible(true);
		_plot_ratio_widget->xAxis2->setTickLabels(false);
		_plot_ratio_widget->yAxis2->setVisible(true);
		_plot_ratio_widget->yAxis2->setTickLabels(false);
		// pass data points to graphs:
		_plot_ratio_widget->graph(0)->setData(x, y);
		// let the ranges scale themselves so graph 0 fits perfectly in the visible area:
		_plot_ratio_widget->graph(0)->rescaleAxes();
		// Note: we could have also just called _plot_ratio_widget->rescaleAxes(); instead
		// make range moveable by mouse interaction (click and drag):
		_plot_ratio_widget->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
		_plot_ratio_widget->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
		_plot_ratio_widget->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
		                                     QCP::iSelectLegend | QCP::iSelectPlottables);

		_plot_ratio_widget->replot();
	}
}

void
Console::updateVelocityPlot()
{
	bool valid = _plot_velocity_widget && plotVelocityTableWidget->rowCount() > 0;
	for( int i = 0; i < plotVelocityTableWidget->rowCount() && valid; i++ )
	{
		if ( plotVelocityTableWidget->item( i, 0 ) == 0 || plotVelocityTableWidget->item( i, 1 ) == 0 )
			valid = false;
		else
		{
			bool ok1, ok2;
			double d = plotVelocityTableWidget->item( i, 0 )->data( Qt::EditRole ).toDouble( &ok1 );
			d = plotVelocityTableWidget->item( i, 1 )->data( Qt::EditRole ).toDouble( &ok2 );
			if ( !( ok1 && ok2 ) )
				valid = false;
		}
	}
	
	if ( valid )
	{
		if ( _plot_velocity_widget->graphCount() > 0 )
			_plot_velocity_widget->clearGraphs();
	
		_plot_velocity_widget->addGraph();
		_plot_velocity_widget->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
		_plot_velocity_widget->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); 

		int rows = plotVelocityTableWidget->rowCount();
		QVector<double> x, y;
		double min_y = 10000, max_y = -10000;
		x.resize( rows );
		y.resize( rows );
		for ( int i = 0; i < rows; ++i )
		{
			x[i] = plotVelocityTableWidget->item( i, 0 )->data( Qt::EditRole ).toDouble();
			y[i] = plotVelocityTableWidget->item( i, 1 )->data( Qt::EditRole ).toDouble();
			min_y = std::min( min_y, y[i] );
			max_y = std::max( max_y, y[i] );			
		}

		if ( max_y - min_y < 0.00001 )
		{
			max_y *= 1.01;
			min_y *= 0.99;
		}
		_plot_velocity_max_y = max_y;			
			
		_v_plot = Graph_Evaluator( x.toStdVector(), y.toStdVector() );

		// configure right and top axis to show ticks but no labels (could've also just called _plot_velocity_widget->setupFullAxesBox):
		_plot_velocity_widget->xAxis->setScaleType( QCPAxis::stLogarithmic );
		_plot_velocity_widget->xAxis2->setVisible(true);
		_plot_velocity_widget->xAxis2->setTickLabels(false);
		_plot_velocity_widget->yAxis2->setVisible(true);
		_plot_velocity_widget->yAxis2->setTickLabels(false);
		// pass data points to graphs:
		_plot_velocity_widget->graph(0)->setData(x, y);
		_plot_velocity_widget->rescaleAxes();
//		_plot_velocity_widget->xAxis->setRange( x.first(), x.last() );
//		_plot_velocity_widget->yAxis->setRange( min_y, max_y );
		
		// make range moveable by mouse interaction (click and drag):
		_plot_velocity_widget->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
		_plot_velocity_widget->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
		_plot_velocity_widget->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
		                                        QCP::iSelectLegend | QCP::iSelectPlottables);


		_plot_velocity_widget->replot();
	}
}

void
Console::addRow()
{
	if ( plotTableWidget->currentRow() == -1 )
		plotTableWidget->insertRow( plotTableWidget->rowCount() );
	else
		plotTableWidget->insertRow( plotTableWidget->currentRow() + 1 );

	updatePlot();
}

void
Console::deleteRow()
{
	plotTableWidget->removeRow( plotTableWidget->currentRow() );

	updatePlot();
}

void
Console::addRowVelocity()
{
	if ( plotVelocityTableWidget->currentRow() == -1 )
		plotVelocityTableWidget->insertRow( plotVelocityTableWidget->rowCount() );
	else
		plotVelocityTableWidget->insertRow( plotVelocityTableWidget->currentRow() + 1 );

	updateVelocityPlot();
}

void
Console::deleteRowVelocity()
{
	plotVelocityTableWidget->removeRow( plotVelocityTableWidget->currentRow() );

	updateVelocityPlot();
}

void
Console::updateThresholdWeight( int val )
{
	float scale_fac = compute_turning_mixture( val );

	thresholdWeight->setValue( ( (int)( roundf( scaling_val* scale_fac ) ) ) / scaling_val );
	
	if ( _model->is_tracking() )
	{
	   _model->camera_callback()->set_turning_mixture( scale_fac );
	}
}

void
Console::updateThresholdScale( double val )
{
	float range = roundf( scaling_val * ( maxThresholdScale->value() - minThresholdScale->value() ) ) / scaling_val;
	thresholdTurningSlider->setValue( (int) roundf( ( ( thresholdWeight->value() - minThresholdScale->value() ) / range ) * 100.f ) );
	
	float scale_fac = compute_turning_mixture( thresholdTurningSlider->value() );

	thresholdWeight->setValue( ( (int)( roundf( scaling_val * scale_fac ) ) ) / scaling_val );
	
	if ( _model->is_tracking() )
	{
	   _model->camera_callback()->set_turning_mixture( scale_fac );
	}
}

void 
Console::setCrossbarWidth( double value )
{
	if ( viewer && _model->physics_enabled() )
		_model->set_crossbar_width( value );
}

void
Console::restrict_vertical_motion( bool yes_or_no )
{
	_model->restrict_vertical_motion( yes_or_no );
}

void
Console::setMinimumVelocityThresold( double value )
{
	_model->set_minimum_velocity_thresold( value );
}

void
Console::setOutputRate( int value )
{
	if ( viewer )
		viewer->set_output_rate( value );
}

void
Console::showInvisibleObjects( bool yes_or_no )
{
	_model->show_invisible_objects( yes_or_no );
}

void
Console::changeShader( int value )
{
	_model->change_shader( value );
}

void
Console::updateViewport( int value, int lower, int upper )
{
	viewer->updateViewport( value - 1, lower, upper );
}

void
Console::load_config( Config_Memento const* cm )
{
	int num_cameras = cm->_numCameras;
	if ( viewer )
		num_cameras = viewer->numberOfCameras();

	if ( cm->_numCameras == num_cameras )
	{
		numCamerasSpinBox->setValue( cm->_numCameras );
		numDisplaysSpinBox->setValue( cm->_num_displays );
		startingDisplaySpinBox->setValue( cm->_starting_display );
		
		gangedCheckBox->setChecked( cm->_gangCameras );

		xCenterDistortsBox->setChecked( cm->_xCenterDistorts );
			
		for ( int i = 0; i < num_cameras; i++ )
		{
			_frame[ i ] = cm->_frame[i];
			_distort[ i ] = cm->_distort[i];
			_center[ i ] = cm->_center[i];
			
			if ( viewer )
			{
				updateViewport( i + 1, _frame[ i ].first, _frame[ i ].second );
				viewer->setXFocalLength( i, _distort[ i ].second / 100.f );
				viewer->setYFocalLength( i, _distort[ i ].third / 100.f );
				viewer->setCenter( i, xCenterDistortsBox->isChecked(),
				                   -_center[ i ].first / 100.f, -_center[ i ].second / 100.f );
			}
		}
		
		enableDistortion->setChecked( _distort[ cameraSelectSpinBox->value() - 1 ].first );
		focalLengthSlider_y->setValue( _distort[ cameraSelectSpinBox->value() - 1 ].second );
		focalLengthSlider_x->setValue( _distort[ cameraSelectSpinBox->value() - 1 ].third );
		xCenterSlider->setValue( _center[ cameraSelectSpinBox->value() - 1 ].first );
		yCenterSlider->setValue( _center[ cameraSelectSpinBox->value() - 1 ].second );
			
		if ( viewer )
			viewer->setUpSlaves();
		
		char val[256];
			
		sprintf( val, "%d", cm->_retries );
		retriesCtrl->setText( val );
		sprintf( val, "%d", cm->_port );
		portCtrl->setText( val );
		hostnameCtrl->setText( cm->_host.c_str() );

		gangMotionCheckBox->setChecked( cm->_gangMotion );

		// GraphWidget
		for ( int i = 0; i < num_cameras; i++ )
		{
			_graphicsView[i]->load( cm->_graph[i] );
		}
		
		horizontalBlankingSlider->setValue( _frame[ lastCamera - 1 ].first );
		horizontalBlankingSlider_2->setValue( _frame[ lastCamera - 1 ].second );

		setAmbientColor( cm->_ambient );
		setDiffuseColor( cm->_diffuse );
		setDiffusePower( cm->_power );
		setBackgroundColor( cm->_background );
			
		_brightness = cm->_global_brightness;
		if ( !_blankDisplay )
		{
			for ( int i = 0; i < num_cameras; i++ )
				setGlobalBrightness( i, cm->_global_brightness[i] );

			// Sets to 0-offset in call
			setGlobalBrightnessTarget( 1 );
		}
		
		shaderComboBox->setCurrentIndex( cm->_shader_choice );
			
		// Calibration Parameters
		calibrateXValue->setText( cm->_calib_x.c_str() );
		calibrateXGain->setValue( cm->_gain_x );

		calibrateYValue->setText( cm->_calib_y.c_str() );
		calibrateYGain->setValue( cm->_gain_y );

		calibrateZValue->setText( cm->_calib_z.c_str() );
		calibrateZGain->setValue( cm->_gain_z );

		velocitySmoothingEnabled->setChecked( !cm->_enable_vel_smoothing );
		velocitySmoothingEnabled->setChecked( cm->_enable_vel_smoothing );
		inputSmoothingEnabled->setChecked( !cm->_enable_input_smoothing );
		inputSmoothingEnabled->setChecked( cm->_enable_input_smoothing );
		intermediarySmoothingEnabled->setChecked( !cm->_enable_intermediary_smoothing );
		intermediarySmoothingEnabled->setChecked( cm->_enable_intermediary_smoothing );
		outputSmoothingEnabled->setChecked( !cm->_enable_output_smoothing );
		outputSmoothingEnabled->setChecked( cm->_enable_output_smoothing );
		velocitySmoothingInterval->setValue( cm->_vel_smoothing_interval );
		inputSmoothingInterval->setValue( cm->_input_interval );
		intermediarySmoothingInterval->setValue( cm->_intermediary_interval );
		outputSmoothingInterval->setValue( cm->_output_interval );
		
		ballRadiusBox->setValue( cm->_ball_radius );
		crossbarEnabledCheckBox->setChecked( cm->_crossbar_enabled );
		crossbarWidthSpinBox->setValue( cm->_crossbar_width );
		minimumVelocityThreshold->setValue( cm->_minimum_velocity_thresold );
		restrictVerticalMotion->setChecked( cm->_restrict_vertical_motion );

		if ( cm->_port_name[0] == ',' )
			portNameControl->setText( "" );
		else
			portNameControl->setText( cm->_port_name.c_str() );
		setSerialPortName();
		outputRateSpinBox->setValue( cm->_output_rate );

		rotatedCamerasCheckBox->setChecked( cm->_rotatedCameras );
		fovSlider->setValue( cm->_fov );
		fovOffsetSpinBox->setValue( cm->_fov_offset );
		setRotatedCameras( cm->_rotatedCameras );
			
		frameModeComboBox->setCurrentIndex( cm->_frame_mode );
		indicatorSizeSlider->setValue( cm->_indicator_size );
		borderSizeSlider->setValue( cm->_border_size );
		screenSelectSpinBox->setValue( cm->_screen_select );
		indicatorHorizontalPositionSlider->setValue( cm->_indicator_horizontal_pos );
		indicatorVerticalPositionSlider->setValue( cm->_indicator_vertical_pos );

		// Flip the sense of the widget so that the real callback occurs
		thresholdTurningEnabled->setChecked( !cm->_enable_threshold_turning );
		thresholdTurningEnabled->setChecked( cm->_enable_threshold_turning );
		minThresholdScale->setValue( cm->_min_threshold_scale );
		maxThresholdScale->setValue( cm->_max_threshold_scale );
		thresholdTurningSlider->setValue( cm->_current_threshold_scale );
		
		initializePlotWidgets( cm->_plot_x, cm->_plot_y, cm->_plot_vel_x, cm->_plot_vel_y, false );
	}
}

void
Console::save_config( Config_Memento* cm )
{
	if ( viewer )
		cm->_numCameras = viewer->numberOfCameras();
	else
		cm->_numCameras = numCamerasSpinBox->value();
		
	cm->_num_displays = numDisplaysSpinBox->value();
	cm->_starting_display = startingDisplaySpinBox->value();
	cm->_gangCameras = gangedCheckBox->isChecked();
	cm->_xCenterDistorts = xCenterDistortsBox->isChecked();

	if ( viewer )
		resetSliders( lastCamera );

	cm->_frame = _frame;
	cm->_distort = _distort;
	cm->_center = _center;

	cm->_retries = retries();
	from_string<int>( cm->_port, port(), std::dec );
	cm->_host = hostname();

	cm->_gangMotion = gangMotionCheckBox->isChecked();
		
	cm->_frame_packing = framePackingEnabled->isChecked();
	cm->_graph.resize( cm->_numCameras );
	for ( int i = 0; i < cm->_numCameras; i++ )
		_graphicsView[i]->save( cm->_graph[i] );
		
	cm->_ambient = QColor( ambientLabel->text() );
	cm->_diffuse = QColor( diffuseLabel->text());
	cm->_power = diffusePowerLevel->value();
	cm->_background = QColor( backgroundLabel->text() );
	cm->_global_brightness = _brightness;
	cm->_shader_choice = shaderComboBox->currentIndex();
		
	cm->_calib_x = calibrateXValue->text().toStdString();
	cm->_gain_x = calibrateXGain->value();
	cm->_calib_y = calibrateYValue->text().toStdString();
	cm->_gain_y = calibrateYGain->value();
	cm->_calib_z = calibrateZValue->text().toStdString();
	cm->_gain_z = calibrateZGain->value();

	cm->_enable_vel_smoothing = velocitySmoothingEnabled->isChecked();
	cm->_vel_smoothing_interval = velocitySmoothingInterval->value();
	cm->_enable_input_smoothing = inputSmoothingEnabled->isChecked();
	cm->_input_interval = inputSmoothingInterval->value();
	cm->_enable_output_smoothing = outputSmoothingEnabled->isChecked();
	cm->_output_interval = outputSmoothingInterval->value();
	cm->_enable_intermediary_smoothing = intermediarySmoothingEnabled->isChecked();
	cm->_intermediary_interval = intermediarySmoothingInterval->value();

	cm->_ball_radius = ballRadiusBox->value();
	cm->_crossbar_enabled = crossbarEnabledCheckBox->isChecked();
	cm->_crossbar_width = crossbarWidthSpinBox->value();
	cm->_minimum_velocity_thresold = minimumVelocityThreshold->value();
	cm->_restrict_vertical_motion = restrictVerticalMotion->isChecked();
	
	if ( portNameControl->text().count() > 0 )
		cm->_port_name = portNameControl->text().toStdString();
	else
		cm->_port_name = ",";
	cm->_output_rate = outputRateSpinBox->value();
		
	cm->_rotatedCameras = rotatedCamerasCheckBox->isChecked();
	cm->_fov = fovSlider->value();
	cm->_fov_offset = fovOffsetSpinBox->value();

	cm->_frame_mode = frameModeComboBox->currentIndex();
	cm->_indicator_size = indicatorSizeSlider->value();
	cm->_border_size = borderSizeSlider->value();
	cm->_screen_select = screenSelectSpinBox->value();
	cm->_indicator_horizontal_pos = indicatorHorizontalPositionSlider->value();
	cm->_indicator_vertical_pos = indicatorVerticalPositionSlider->value();
		
	cm->_enable_threshold_turning = thresholdTurningEnabled->isChecked();
	cm->_min_threshold_scale = minThresholdScale->value();
	cm->_max_threshold_scale = maxThresholdScale->value();
	cm->_current_threshold_scale = thresholdTurningSlider->value();	

	int rows = plotTableWidget->rowCount();
	cm->_plot_x.resize( rows );
	cm->_plot_y.resize( rows );

	for ( int i = 0; i < rows; ++i )
	{
		cm->_plot_x[i] = plotTableWidget->item( i, 0 )->data( Qt::EditRole ).toDouble();
		cm->_plot_y[i] = radians( plotTableWidget->item( i, 1 )->data( Qt::EditRole ).toDouble() );
	}

	rows = plotVelocityTableWidget->rowCount();
	cm->_plot_vel_x.resize( rows );
	cm->_plot_vel_y.resize( rows );

	for ( int i = 0; i < rows; ++i )
	{
		cm->_plot_vel_x[i] = plotVelocityTableWidget->item( i, 0 )->data( Qt::EditRole ).toDouble();
		cm->_plot_vel_y[i] = plotVelocityTableWidget->item( i, 1 )->data( Qt::EditRole ).toDouble();
	}
}

void
Console::loadTraceFile( char const* fileName )
{
	bool first = true;
	ifstream trace_file( fileName );
	std::string junk;
	_trace_path = new osg::AnimationPath;
	_trace_path->setLoopMode( osg::AnimationPath::LOOP );
	
	float time;
	while ( !trace_file.eof() )
	{
		float x, y, z, vel, yaw, p_yaw;
		char ch, str[1024];
	
		trace_file.getline( str, 1024 );
		std::string s( str );
		size_t n = std::count( s.begin(), s.end(), ',' );

		if ( n >= 6 )
		{
			std::stringstream ss( str );
			ss >> time >> ch >> x >> ch >> y >> ch >> z >> ch >> vel >> ch >> yaw >> ch >> junk;
//		std::cout << time << ", " << x << ", " << y << ", " <<  z << ", " << yaw;

			osg::Vec3f pt( x, y, z );

			yaw = WrapTwoPI(radians( yaw ));

			p_yaw = yaw;
		
//		std::cout << ", " << yaw << std::endl;
			osg::Quat rotation( osg::Quat( yaw, osg::Vec3( 0.0, 1.0, 0.0 ) ) );

			_trace_path->insert( time, osg::AnimationPath::ControlPoint( pt, rotation ) );
		}
	}
	
	trace_file.close();
	_model->set_trace_path( _trace_path );

	// Update the time slider widget
	timeSlider->setMaximum( _trace_path->getTimeControlPointMap().size()-1 );
	timeLabel->setMaximum( time );
	if ( viewer )
		viewer->set_movie_mode( true );
}

void
Console::update_time_slider( int value )
{
	timeSlider->setValue( value );
	
	if ( _recording )
		if ( _playForward || _playBackward )
			if ( abs( value - _last_frame_index ) > strideLength->value() )
				// If we're recording and playing the movie, and we've hit the end,
				// then stop the recording.
				recordCallback();

	_last_frame_index = value;
}
	
void
Console::update_ratio_line()
{
	std::vector< float > const& vals = _model->camera_callback()->ratio();
	float ratio = fabs( vals[0] );
	float vel = vals[1];
	float smoothed_ratio = fabs( vals[2] ); 
	float smoothed_vel = vals[3];
	
	//std::cout << ratio << ", " << _plot_ratio_max_y << std::endl;
	
	_ratio_line->start->setCoords( ratio, 0.f );
	_ratio_line->end->setCoords( ratio, _plot_ratio_max_y );
	_velocity_ratio_line->start->setCoords( vel, 0.f );
	_velocity_ratio_line->end->setCoords( vel, _plot_velocity_max_y );

	if ( inputSmoothingEnabled->isChecked() )
	{
		_smoothed_ratio_line->start->setCoords( smoothed_ratio, 0.f );
		_smoothed_ratio_line->end->setCoords( smoothed_ratio, _plot_ratio_max_y );
	}
	
	if ( velocitySmoothingEnabled->isChecked() )
	{
		_smoothed_velocity_ratio_line->start->setCoords( smoothed_vel, 0.f );
		_smoothed_velocity_ratio_line->end->setCoords( smoothed_vel, _plot_velocity_max_y );
	}
	
	_plot_ratio_widget->replot();
	_plot_velocity_widget->replot();
}

void 
Console::setExportFilename( QString const& fileName )
{
	QString temp( fileName );
	switch( exportFileNameStyle->currentIndex() )
	{
		case 0:
			temp += "_out.txt";
			break;
		case 1:
			char s[40];
			sprintf( s, "_out_%d.txt", _connectionCounter );
			temp += s;
			break;
		default:	
			temp += "_out.txt";
			break;
	}

	_model->set_export_filename( temp );
}

void 
Console::update_timer()
{
	QTime t = timeTrialEditor->time();
	int s = t.second();
	int m = t.minute();
	int h = t.hour();
	s--;
	if ( s < 0 )
	{
		s = 59;
		m--;
		if ( m < 0 )
		{
			m = 59;
			h--;
		}
	}
	
	t.setHMS( h, m, s );
	timeTrialEditor->setTime( t );
}

float
Console::compute_turning_mixture( float val )
{
	float range = roundf( scaling_val * ( maxThresholdScale->value() - minThresholdScale->value() ) ) / scaling_val;
	float fac = val / 100.f;
	float scale_fac = range * fac + minThresholdScale->value();

	return scale_fac;	
}

