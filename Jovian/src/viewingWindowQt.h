/* -*-c++-*- */

#include <iostream>
#include <vector>

#include <boost/asio.hpp> // include boost
#include <boost/circular_buffer.hpp>
#include <boost/shared_ptr.hpp>

#include <btBulletDynamicsCommon.h>

#include <QtCore/QTimer>
#include <QApplication>
#include <QMainWindow>

#include <osg/Node>
#include <osg/BoundingBox>
#include <osg/StateSet>
#include <osg/Switch>
#include <osg/Texture2D>
#include <osgViewer/Viewer>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/NodeTrackerManipulator>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgQt/GraphicsWindowQt>

// For GLSL Shading
#include <osg/Program>
#include <osg/Shader>
#include <osg/Uniform>

#include <Globals.h>
#include <graphwidget.h>
#include <scene_model.h>

#ifndef VIEWINGWINDOWQT
#define VIEWINGWINDOWQT

typedef boost::shared_ptr< bool > bool_ptr;


class ViewingWindowQt : public QObject, public osgViewer::Viewer
{
	Q_OBJECT

public:
	// Initialization
	ViewingWindowQt( int nCameras, std::vector< GraphWidget* > g,
	                 osg::ArgumentParser& arguments, Scene_Model* model );

	// Duplication
	// Destruction
	~ViewingWindowQt();

	// Access
	osg::Node* getSceneRoot();
	osg::BoundingBox const* const getBound() const { return _model->scene_bound(); }
	int width( int i ) {return _width[i]; }
	int total_width() {return _total_width; }
	int height( int i ) { return _height[i]; }
	int numberOfCameras() { return numCameras; }
	osg::Switch* indicator() { return _indicator; }
	Frustum& frustum( int i ) { return _frustum[ i ]; }
    ScreenGeometry& screen_geometry(int i) { return _screen_geometry[i]; }

	// Do the division here in case the call is made before averaging is complete
	// If the average hasn't even started we return 1 to guarantee a valid sample
	double averageFrameTime() { return ( ( _avgFrameCount == 0 ) ? 1 : _avgFrameTime / (double)_avgFrameCount ); }

	// Measurement
	// Comparison
	// Status report
	// Status setting
	void setXFocalLength( int i, float focal_length ) { _x_focal_length[i] = focal_length; setUpSlaves();};
	void setYFocalLength( int i, float focal_length ) { _y_focal_length[i] = focal_length; setUpSlaves();};
	void setCenter( int i, bool distorts, float x, float y ) { _bottom_texcoord[i] = osg::Vec2( x, y ); _x_center_distorts_y = distorts; setUpSlaves();};
	void setClearColor( osg::Vec4 color ) { 
		_clearColor = color; 
		//setUpSlaves(); 
	}
	void setGlobalBrightness( int i, osg::Vec4 color ) { _globalBrightness[i] = color; setUpSlaves(); }
    void setBrightnessAdjustment(bool yes_or_no) { _brightness_adjustment_enabled = yes_or_no; setUpSlaves(); }
    void setScreenRadius(float radius) { _screen_radius = radius; setUpSlaves(); }
	void setTrackNode();
	void setFieldOfView( float fov ) { _fov = fov; setUpSlaves(); }
	void setFieldOfViewOffset( float fov_offset ) { _fov_offset = fov_offset; setUpSlaves(); }
	void setRotatedCameras( bool yes_or_no ) { _rotateCamera = yes_or_no; setUpSlaves(); }

	void setIndicatorSize( int new_size ) { _indicatorSize = new_size; setUpSlaves(); }
	void setIndicatorBorderSize( int new_border ) { _indicatorBorderSize = new_border; setUpSlaves(); }
	void setIndicatorPosition( osg::Vec2 new_pos ) { _indicatorPos = new_pos; setUpSlaves(); }
	void setIndicatorScreen( int screen ) { _whichScreen = screen; setUpSlaves(); }
	void setIndicatorMode( int index ) { _indicatorOn = index; }

	void setUpdateCallback( osg::NodeCallback* callback, bool reset = true );
	void setSerialPort( std::string port );

	void set_output_rate( int value ) { _output_rate = 1.f / (float)value; }
	void use_custom_widget( bool yes_or_no ) { _custom_widget_enabled = yes_or_no; }

	void setManipulatorToTrackingMode() { _keyswitchManipulator->selectMatrixManipulator( 2 ); }

	void set_frame_packing( bool value ) { _packFrames = value; _frameSwitch.clear(); _texture.clear(); setUpSlaves(); averageFrames(); }
	bool get_frame_packing() const { return _packFrames; }

	void setDiffusePower( float dPower );
	void setDiffuseColor( osg::Vec4 diff );
	void setAmbientColor( osg::Vec4 amb );
	void set_export_treadmill( bool yes_or_no ) { _output_treadmill_data = yes_or_no; }
	void set_reduced_output( bool yes_or_no ) { _use_reduced_output = yes_or_no; }
	void use_custom_frustum( bool yes_or_no ) { _use_custom_frustum = yes_or_no; }
	std::ostream& reportCameraConfiguration( std::ostream& os );

	// Cursor movement
	// Element change
	void loadOSG( std::string filename );
	void loadModel( osg::ArgumentParser& arguments );
	void loadImage( std::string filename, bool flip = false, bool useTextureRectangle = true );
	void loadData( );
	void addReportableObject( btCollisionObject* colObj ) { _model->add_reportable_object( colObj ); }
	void resetManipulatorMode() { _keyswitchManipulator->selectMatrixManipulator( 0 ); }

	// Removal
	void clearSerialPort() { if ( _port ) delete _port; _port = 0; }

	// Resizing
	// Transformation
	// Conversion
	// Basic operations
	void initializeCameras( int nCameras, int num_displays, int starting_display );
	void selectScene( int id );
	void updateViewport( int i, int leftOffset, int rightOffset );
	void setUpSlaves();
	void averageFrames() { _averageFrames = true; _avgFrameCount = 0; _avgFrameTime = 0.0; _ignoredFramesCount = 50; _avg_frame_buffer.clear(); }
	void enableDistortion( int i, bool on_or_off ) { _use_distortion[i] = on_or_off; };
	// Outputs an event string. Prepends a time stamp, appends newline
	void outputEvent( std::ostringstream const& eventString,
	                  std::ostringstream const& floatEventString,
	                  bool force_write = false,
	                  double time_stamp = -1 ) const;

	// Miscellaneous
	// Obsolete
	// Inapplicable

protected:

	QTimer _timer;

	QWidget* addViewWidget( osg::Camera* camera );

	osg::Node* createDistortionSubgraph( int index, osg::Node* subgraph,
	                                     const osg::Vec4& clearColour,
	                                     double theta, bool addHud );

	osg::Node* createDistortionSubgraphWithPacking( int index, int idx,
	                                                osg::Node* subgraph,
	                                                const osg::Vec4& clearColour,
	                                                double theta, bool addHud );

	osg::Camera* multipleWindowWithDistortion( int i, int numCameras, int num_displays,
	                                           int starting_display );

	void myRenderingTraversals();

private slots:

	void paintEvent( );

private:

	QWidget** widgets;
	Scene_Model* _model;
	osg::ref_ptr<osg::GraphicsContext::Traits> _traits;
	bool_ptr _swapBuffers;
	int numCameras;
	bool _multipleScreens;
	std::vector< unsigned int > _width, _height;
	unsigned int _total_width;
	float _fov, _fov_offset;
	bool _rotateCamera, _initiated;
	bool _x_center_distorts_y;
	std::vector< float > _x_focal_length;
	std::vector< float > _y_focal_length;
	std::vector< bool > _use_distortion;
	osg::Vec4 _clearColor;
	std::vector< osg::Vec4 > _globalBrightness;
	std::vector< osg::Vec2 > _bottom_texcoord;
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> _keyswitchManipulator;
	int _childNum;
	osg::Switch* _indicator;
	int _indicatorSize;
	int _indicatorBorderSize;
	int _indicatorOn;
	int _whichScreen;
	int _screenNum;
	osg::Vec2 _indicatorPos;
	osg::Camera* _hud;
	btVector3 _lastPosition;
	int _buffer_samples;
	boost::circular_buffer< float > _avg_frame_buffer;
	double _lastRefTime, _startRefTime, _avgFrameTime;
	bool _averageFrames;
	int _avgFrameCount, _ignoredFramesCount;
	boost::asio::io_service _io;
	boost::asio::serial_port* _port;
	std::vector< GraphWidget* > _graph;
	double _currSimTime, _prevSimTime;
	bool _packFrames;
	std::vector< osg::Texture2D* > _texture;
    std::vector<osg::Texture1D* > _brightness;
	std::vector< osg::Switch* > _frameSwitch;
	mutable double _last_output_time, _output_rate;
	bool _output_treadmill_data, _use_reduced_output;
	bool _custom_widget_enabled, _use_custom_frustum;
	std::vector< Frustum > _frustum;
    std::vector< ScreenGeometry > _screen_geometry;
    GeometryType _screen_type; 
    bool _brightness_adjustment_enabled;
    double _screen_radius; // indicates the radius of the screen curvature for cylindrical screens 
};

#endif

