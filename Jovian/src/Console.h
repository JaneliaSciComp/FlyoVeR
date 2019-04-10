/* -*-c++-*- */

#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <QMainWindow>

#include <btBulletDynamicsCommon.h>

#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/ArgumentParser>

#include <ui_console.h>

#include <Communicator.h>
#include <CameraUpdateCallback.h>
#include <Config_Memento.h>
#include <FileStatusWidget.h>
#include <Graph_Evaluator.h>
#include <graphwidget.h>
#include <QCustomPlot/qcustomplot.h>
#include <scene_model.h>
#include <ViewingWindowQt.h>
#include <QString>

class Console: public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

public:
	Console( osg::ArgumentParser& args, QWidget *parent = 0 );
	~Console() { };

	void setViewer( ViewingWindowQt* v ) { viewer = v; };

	int retries( void ) const;
	std::string port( void ) const;
	std::string hostname( void ) const;
	Communicator* getCommunicator( void ) { return comm; }

	void resetPosition( );
	void select_scene();
	void manualSpeedUp();
	void manualSpeedDown();
	void disableMotion();
	void blankDisplay();
	bool motionAndDisplayGanged() { return gangMotionCheckBox->isChecked(); }
	void load_config( Config_Memento const* cm );
	void save_config(Config_Memento* cm);
	Scene_Model* model() { return _model; }
	void setVersionString(std::string& s) {_versionString = s;}

protected:
	void keyPressEvent( QKeyEvent *event );
	void move_to_start();

	private slots:

	void about();
	void doOpen();
	void doClose();
	void doOpenConfig();
	void doOpenConfigXml();
	void doSaveConfig();
	void doSaveConfigXml();
	void doImageOpen();
	void doOSGOpen();
	void resetSliders( int value );
	void setCameraMaximums( int newMax );
	void startViewer();
	void updateViewportLeft( int value );
	void updateViewportRight( int value );
	void setXFocalLength( int value );
	void setYFocalLength( int value );
	void setGangMotion( bool value );
	void setDistortionEnabled( bool value );
	void setCenter();
	void setupConnection( bool reset = true );
	void motionAndBlankingCallback();
	void disableMotionCallback();
	
	// Display Callbacks
	void resetDisplay();
	void distributeHorizontally();
	void distributeVertically();
	void smoothDisplay();
	void linearizeEdges();
	void setFramePacking( bool value );
	
	// Lighting Callbacks
	void setAmbientColorCallback();
	void setGlobalBrightnessCallback();
	void setDisplayBlankingCallback();
	void setGlobalBrightnessTarget( int value );
	void setBackgroundColorCallback();
	void setDiffuseColorCallback();
	void setDiffusePower( int power );
	void enableFog( bool enable );
	void changeFogMode( int mode );
	void setFogColorCallback();
	void setFogColor( QColor color );
	void setFogDensity( int density );
	void setFogDensity( double density );
	void setFogStart( int start );
	void setFogEnd( int end_loc );

	void setIndicatorSize( int new_size );
	void setIndicatorBorderSize( int new_size );
	void setIndicatorPosition( int ignored );
	void setIndicatorScreen( int screen );
	void setIndicatorMode( int index );

	void setFieldOfView( int fov );
	void setFieldOfView( double fov );
	void setFieldOfViewOffset( int fov_offset );
	void setFieldOfViewOffset( double fov_offset );
	void setRotatedCameras( bool yes_or_no );

	// Calibration Slots
	void calibrateX();
	void calibrateY();
	void calibrateZ();
	void enableEditing( bool value );
	void updateCalibrationValuesCallback();
	
	// Configuration Widgets
    void enableBrightnessAdjustment(bool onOrOff);
	void enableVelocitySmoothing( bool yes_or_no );
	void setIntervalForVelocitySmoothing( int value );
	void enablePreBlendHeadingSmoothing( bool yes_or_no );
	void enableIntermediaryHeadingSmoothing( bool yes_or_no );
	void enablePostBlendHeadingSmoothing( bool yes_or_no );
	void setIntervalForPreBlendingSmoothing( int value );
	void setIntervalForIntermediarySmoothing( int value );
	void setIntervalForPostBlendingSmoothing( int value );
	void setSerialPortName();
	void enableFileExportCallback( bool yes_or_no );
	void doSetExportFileName();
	void enableTreadmillOutputCallback( bool yes_or_no ) { viewer->set_export_treadmill( yes_or_no ); };
	void enableReducedOutputCallback( bool yes_or_no ) { viewer->set_reduced_output( yes_or_no ); };
	void autoHeadingCallback();
	void setCrossbarWidth( double value );
	void restrict_vertical_motion( bool yes_or_no );
	void setMinimumVelocityThresold( double value );
	void setOutputRate( int value );
	void togglePhysicsDebugging( bool yes_or_no ) { _model->debug_physics( yes_or_no ); };
	void showInvisibleObjects( bool yes_or_no );
	void changeShader( int value );
	void update_frustum( bool val );
	void update_frustum( );
    void update_screen_geometry( );
    void setCylindricalScreen();
    void setFlatScreen();
    void update_screen_geometry(GeometryType geom);
	void use_matrix_not_frustum( bool val );
	void copy_matrix();
	void update_reinforcement();
	void update_respawn();

	// Note use of auto-connected Qt method name CMB
	void on_yawOnlyCheckBox_stateChanged();
	void update_yawOnly();
	void on_stripeDistanceBox_valueChanged(double value);
	void on_radialFogCheckBox_toggled(bool isChecked);
	void on_exportFileNameStyle_currentIndexChanged(int index);
	
	// Turning Widgets
	void enableThresholdTurning( bool value );
	void updatePlot( int row, int column );
	void updateVelocityPlot( int row, int column );
	void addRow();
	void deleteRow();
	void addRowVelocity();
	void deleteRowVelocity();
	void updateThresholdWeight( int val );
	void updateThresholdScale( double val );
	void updateAutoHeadingTurnRate( double val );

	// File Widget Slot
	void switchScene( int id );
	void update_timer();
	void update_ratio_line( );
	
private:
	QString getExportFileBaseName();
	void setup_slaves() { viewer->setUpSlaves(); }
	void initializePlotWidgets( std::vector< double > x, std::vector< double > y, 
		std::vector< double > vx, std::vector< double > vy, 
		bool create_plot = true );
	void updatePlot( );
	void updateVelocityPlot();
	void loadFile(const QString &fileName);
	void enableBlankingWidgets( bool onOrOff );
	void updateViewport( int value, int lower, int upper );
	void doCalibration( QPushButton* button, float* rx, float* ry, float* rz );
	void setAmbientColor( QColor color );
	void setGlobalBrightness( int i, QColor color );
	void setBackgroundColor( QColor color );
	void setDiffuseColor( QColor color );
	void setExportFilename( QString const& fileName );
	float compute_turning_mixture( float val );
	void set_frustum_parameters();
	void update_frustum_gui();
    void update_brightness_gui();
	
	osg::ArgumentParser& arguments;
	Scene_Model* _model;
	ViewingWindowQt* viewer;
	Communicator* comm;
	CameraUpdateCallback* callback;
	bool _firstTime;
	bool connected;
	int _connectionCounter;
	bool useImage;
	int lastCamera;
	bool _calibrating;
	int _activeScene;
	std::string imageFile;
	std::string osgFile;
	std::vector< std::pair< int, int > > _frame;
	std::vector< triple< bool, int, int > > _distort;
	std::vector< std::pair< int, int > > _center;
	float _Vfwdf;
	float _Vssf;
	float _Omegaf;
	int _calibrationRuns[3];
	osg::Vec4 _clearColor;
    bool _brightnessAdjustment;
	std::vector< QColor > _brightness;
	std::vector< GraphWidget* > _graphicsView;
	bool _blankDisplay;
	bool _useAutoHeading;
	bool _motionDisabled;
	bool _custom_matrix;
	bool _doYawOnly;
	float _stripe_distance;
	FileStatusWidget* _fileWidget;
	osg::Vec3d _manual_speed;
	QCustomPlot* _plot_ratio_widget;
	QCustomPlot* _plot_velocity_widget;
	double _plot_ratio_max_y, _plot_velocity_max_y;
	QCPItemLine* _ratio_line;
	QCPItemLine* _smoothed_ratio_line;
	QCPItemLine* _velocity_ratio_line;
	QCPItemLine* _smoothed_velocity_ratio_line;
	Graph_Evaluator _plot, _v_plot;
	QTimer* _clock_timer;
	QTimer* _plot_timer;
	QTime _time;
	float _fog_density_scale;
	std::string _versionString;
	// bool _use_radial_fog;
};

#endif
