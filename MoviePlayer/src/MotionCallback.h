/* -*-c++-*- */

#ifndef MOTIONCALLBACK_H
#define MOTIONCALLBACK_H

#include <utility>

#include <boost/circular_buffer.hpp>
#include <boost/multi_array.hpp>

#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osgGA/NodeTrackerManipulator>

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>

#include <osgbDynamics/MotionState.h>

#include <CameraUpdateCallback.h>
#include <Globals.h>
#include <Graph_Evaluator.h>

typedef boost::multi_array< osg::Vec3f*, 2 > array_type;

#define BIT(x) (1<<(x))
enum collision_types {
    COL_NOTHING = 0, //<Collide with camera
    COL_CHANNEL = BIT(0), //<Collide with channel
    COL_REPORTABLE = BIT(1), //<Collide with reportable object
    COL_EE = BIT(2), //<Collides with Everybody Else
    COL_CAMERA = BIT(3) //<Collide with camera
};

struct PathMovementCallback : public osgbDynamics::MotionStateCallback
{
	osg::MatrixTransform* _track_node;
	
	PathMovementCallback( osg::MatrixTransform* trackNode );

	virtual void operator()( const btTransform& worldTrans );
};

struct ChannelMovementCallback : public osgbDynamics::MotionStateCallback
{
	osg::MatrixTransform* _track_node;
	osg::Vec3f _axis;
	array_type* _tangent_grid;
	osg::Vec3f _min_point;
	float _min_dist;
	
	ChannelMovementCallback( osg::MatrixTransform* trackNode,
	                         array_type* tangent_grid,
	                         osg::Vec3f min_point,
	                         float min_dist );

	virtual void operator()( const btTransform& worldTrans );
};

struct MovementCallback : public osgbDynamics::MotionStateCallback
{
	osg::MatrixTransform* _track_node;
	osg::Vec3f _axis;
	
	MovementCallback( osg::MatrixTransform* trackNode );

	virtual void operator()( const btTransform& worldTrans );
};

class CameraMotionUpdateCallback : public osg::NodeCallback
{
public:
	CameraMotionUpdateCallback( btRigidBody* body,
	                            Collision_World* dynamics_world,
	                            osg::ref_ptr< osgGA::NodeTrackerManipulator > tracker );
	
	virtual void clear() {}
	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv ) = 0;
	void set_average_framerate( float rate ) { _frame_rate = rate; interval_for_velocity_smoothing( _interval_for_vel_smoothing ); interval_for_heading_smoothing( _interval_for_input_smoothing, _interval_for_intermediary_smoothing, _interval_for_output_smoothing ); }
	void enable_velocity_smoothing( bool on_or_off ) { _use_vel_smoothing = on_or_off; _vel_frames.clear(); _tt_vel_frames.clear(); _smoothing_vel = osg::Vec3d(0.f, 0.f, 0.f ); _smoothing_tt_vel = osg::Vec3d(0.f, 0.f, 0.f );}
	void enable_input_heading_smoothing( bool on_or_off ) { _use_input_heading_smoothing = on_or_off; _input_ratio.clear(); _input_smoothing_val = osg::Vec3d(0.f, 0.f, 0.f );}
	void enable_intermediary_heading_smoothing( bool on_or_off ) { _use_intermediary_heading_smoothing = on_or_off; _intermediary_angle.clear(); _intermediary_smoothing_val = 0.f;}
	void enable_output_heading_smoothing( bool on_or_off ) { _use_output_heading_smoothing = on_or_off; _output_angle.clear(); _output_smoothing_val = 0.f;}
	void interval_for_velocity_smoothing( int interval );
	void interval_for_heading_smoothing( int input_interval, 
	                                     int intermediary_interval, 
	                                     int output_interval );
	void set_plot_data( Graph_Evaluator vals, Graph_Evaluator vels ) { _plot = vals; _v_plot = vels; }
	// Could make virtual as is only used by the path version of the callback,
	// but we'd have to implement empty versions, so why bother
	void set_turning_mixture( float val ) { _turning_mixture = val; }
	void restrict_vertical_motion( bool yes_or_no )  { _restrict_vertical_motion = yes_or_no; }
	void set_minimum_velocity_thresold( float val ) { _minimum_velocity_thresold = val; }
	virtual void set_auto_heading_turn_rate( float value ) {}
	
	virtual void reset() { _traversed = false; }
	virtual void home() { _initializing = true; _traversed = false; _body->setWorldTransform( _startPos ); }
	void use_dynamics( bool on_or_off )  { _use_dynamics = on_or_off; }
	std::vector< float > const & ratio() const { return _ratio_values; }

protected:
	
	btRigidBody* _body;
	Collision_World* _dynamics_world;
	osg::ref_ptr< osgGA::NodeTrackerManipulator > _tracker;
	btTransform   _startPos;
	int         _punchCount;
	double     _basetime;
	float       _frame_rate;
	bool       _traversed;
	bool       _first;
	bool _initializing;
	float      _theta;
	bool      _use_dynamics, _use_vel_smoothing, _use_input_heading_smoothing, _use_intermediary_heading_smoothing, _use_output_heading_smoothing;
	int        _vel_frame_index, _head_frame_index, _interval_for_vel_smoothing, _interval_for_input_smoothing, _interval_for_intermediary_smoothing, _interval_for_output_smoothing;
	boost::circular_buffer< osg::Vec3d > _input_ratio, _vel_frames, _tt_vel_frames;
	boost::circular_buffer< float > _intermediary_angle, _output_angle;
	osg::Vec3d _input_smoothing_val, _smoothing_vel, _smoothing_tt_vel, _euler_vector;
	float _intermediary_smoothing_val, _output_smoothing_val;
	Graph_Evaluator _plot, _v_plot;
	float _turning_mixture;
	bool _restrict_vertical_motion;
	float _minimum_velocity_thresold;
	osg::Vec3d _y_axis;
	std::vector< float > _ratio_values;
};

class CameraBlockUpdateCallback : public CameraMotionUpdateCallback
{
public:
	CameraBlockUpdateCallback( btRigidBody* body,
	                           Collision_World* dynamics_world,
	                           osg::ref_ptr< osgGA::NodeTrackerManipulator > tracker,
	                           osg::Vec3f& start_dir );
	
	void operator()( osg::Node* node, osg::NodeVisitor* nv );
	void update( osg::Node* node );
};

class CameraPathUpdateCallback : public CameraMotionUpdateCallback
{
public:

	CameraPathUpdateCallback( btRigidBody* body, btRigidBody* sliding_body,
	                          Collision_World* dynamics_world,
	                          osg::ref_ptr< osgGA::NodeTrackerManipulator > tracker,
	                          osg::AnimationPath* ap, double start_time, 
	                          osg::Vec3d start_dir );
	~CameraPathUpdateCallback();
	
	void clear();
	void operator()( osg::Node* node, osg::NodeVisitor* nv );
	void useRotations( bool on_or_off );
	void setDisableCameraPathUpdateState( bool on_or_off );
//	void home() { CameraMotionUpdateCallback::home(); _initializing = true; }
	bool motionState() const { return _disableCameraPathUpdate; }
	void update( osg::Node* node );
	void update_crossbar();
	float getClosetPoint(osg::Vec3d const& A, osg::Vec3d const& B, 
	                     osg::Vec3f& P, bool segmentClamp = false ) const;

private:
	void getInterpolatedControlPoint( double& time, 
	                                  osg::Vec3f pos,
	                                  osg::AnimationPath::ControlPoint& cp ) const;
	osg::Vec3d computeAutoHeadingVector( osg::Vec3d const& vec, osg::Matrixd const& mr, osg::Vec3d const& up, Motion_Data const* md );
	osg::Vec3d computeThresholdVector( float ratio, osg::Vec3d const& vec, osg::Matrixd const& mr, osg::Vec3d const& up, Motion_Data const* md, double* d_angle );
	
	void smooth_velocities( osg::Vec3d& ttv, osg::Vec3d& ahv );
protected:
 
	btRigidBody* _sliding_body;
	btGeneric6DofConstraint* _constraint;
	double _firstTime, _start_time, _deltaTime, _timeIncrement, _minimum_distance;
	bool _averaging, _flip_tangent, _disableCameraPathUpdate;
	osg::AnimationPath* _orig_ap;
	osg::ref_ptr< osg::AnimationPath > _ap;
	osg::AnimationPath::ControlPoint _cp_last, _cp_now;
	osg::Vec3d _pt_last, _pt_now, _tangent, _start_dir;
	double _last_unwrapped_angle, _last_ah_angle;
	float _auto_heading_turn_rate;
};

class Console;

class CameraTraceUpdateCallback : public CameraMotionUpdateCallback
{
public:

	CameraTraceUpdateCallback( btRigidBody* body, Collision_World* dynamics_world,
	                          osg::ref_ptr< osgGA::NodeTrackerManipulator > tracker,
	                           osg::ref_ptr< osg::MatrixTransform > track_node,
	                          osg::AnimationPath* ap );
	~CameraTraceUpdateCallback();
	
	void setTimeIndex( int time_index ) { _time_index = time_index; }
	int time_index( ) { return _time_index; }
	double time( ) { return _time_map[ _time_index ]; }
	double time( int time_index ) { return _time_map[ time_index ]; }
	void setConsole( Console* c ) { _console = c; }
	void increment_time_index( int stride );
	void decrement_time_index( int stride );
	void operator()( osg::Node* node, osg::NodeVisitor* nv );
	void update( osg::Node* node );
	
private:
	
protected:
	int _time_index;
	osg::ref_ptr< osg::MatrixTransform > _track_node;
	osg::ref_ptr< osg::AnimationPath > _ap;
	std::vector< double > _time_map;
	std::vector< double >::iterator _time_map_iterator;
	Console* _console;
};


#endif
