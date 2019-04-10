/* -*-c++-*- */

#ifndef SCENE_MODEL_H
#define SCENE_MODEL_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <QtCore/QString>

#include <btBulletDynamicsCommon.h>

#include <osg/AnimationPath>
#include <osg/ArgumentParser>
#include <osg/BoundingBox>
#include <osg/LightSource>
#include <osg/Node>
#include <osg/StateSet>
#include <osg/Switch>
#include <osg/Texture2D>
#include <osg/Vec3>
#include <osg/Vec4>

#include <osg/io_utils>

#include <osgDB/ReadFile>

#include <osgGA/NodeTrackerManipulator>

#include <CameraUpdateCallback.h>
#include <ColladaVisitor>
#include <Config_Memento.h>
#include <Globals.h>
#include <MotionCallback.h>

struct Physics_State
{
	bool movable;
	bool reportable;
	bool convex;
	bool camera;

	osg::Node* node;
	
	Physics_State(): movable( false ), reportable( false ), convex( true ), camera( false ), 
	                node( 0 ) {}
};

/**
 * Scene_Model.
 **/

class Scene_Model
{
	friend std::ostream& operator<<( std::ostream& os, Scene_Model const& f );
	
public:
	
	// Initialization
	Scene_Model();
	
	// Duplication
	Scene_Model( Scene_Model const& c );
	Scene_Model& operator=( Scene_Model const& c );
	
	// Destruction
	~Scene_Model();
	
	// Access
	std::vector< osg::Node* > const& contact_nodes();
	osg::Switch* current_scene();
	float average_framerate() const { return _frame_rate; }
	std::vector< osg::LightSource* > get_lights() { return _lights; }
	osg::StateSet* per_pixel_state_set() { return _per_pixel_state_set; }
	osg::StateSet* per_vertex_state_set() { return _per_vertex_state_set; }
	osg::BoundingBox* scene_bound() const { return _scene_bound[ _active_model ]; }
	osg::Switch* scene_root() { return _root; }
	osg::ref_ptr<osgGA::NodeTrackerManipulator> tracker() { return _tracker[ _active_model ]; }
	float camera_mount_radius() const { return _camera_mount_radius; }
	osg::Vec3f start_center( int i ) const { return _start_center[ _active_model ][i]; }
	osg::Vec3f start_direction( int i ) const { return _start_center[ _active_model ][i] + _start_cross[ _active_model ][i]; }
	osg::Vec3f eye_point( int i ) const { return _eye_pt[ _active_model ][i]; }
	osg::Vec3f center_point( int i ) const { return _center_pt[ _active_model ][i]; }
	osg::Vec3f up_direction( int i ) const { return _up[ _active_model ][i]; }
	bool eye_point_set() const { return _eye_pt_set[ _active_model ]; }
	float crossbar_width() const { return  _dynamics_world[ _active_model]->crossbar_width; }
	osg::ref_ptr< CameraMotionUpdateCallback > camera_callback() const { return _camera_callback[ _active_model ]; }
	Config_Memento* current_configuration() { return _configuration[ _active_model ]; }

	// Measurement
	// Comparison
	// Status report
	int active_scene() const { return _active_model; }
	int contact_count() const;
	bool debug_physics( ) const { return _debug_physics; }
	bool is_tracking( ) const { return _tracking_initialized; }
	bool motion_disabled() const { return _disable_motion; }
	bool physics_enabled() const;
	bool show_invisible_objects( ) const { return _show_invisibles; }
	btVector3 camera_position() const { return _camera_body[ _active_model ]->getWorldTransform().getOrigin(); }
	bool has_path() const { return _path_set[ _active_model ]; }
	
	// Status setting
	void debug_physics( bool on_or_off ) { _debug_physics = on_or_off; _dynamics_world[ _active_model]->debug_drawer->setEnabled( on_or_off ); }
	void toggle_motion_disabled( ) { _disable_motion = !_disable_motion; }
	void engage_physics_engine( bool on_or_off ) { _engage_physics = on_or_off; }
	void use_rotations( bool use_auto_heading) { }
	void set_export_status( bool on_or_off ) { _export_file = on_or_off; }
	QString const& scene_file_name( ) const { return _scene_file_name[ _active_model ]; }
	void use_dynamics( bool on_or_off )  { _camera_callback[ _active_model ]->use_dynamics( on_or_off ); }

	// Cursor movement
	// Element change
	void add_reportable_object( btCollisionObject* colObj ) { _reportable_objects.push_back( colObj ); }
	void add_rigid_body( btRigidBody* body, collision_types col, int interaction,
	                     bool camera_body = false );
	void change_shader( int value );
	void change_start_location( int new_start_id ) { _start_id = new_start_id; }
	void clear_tracker_data();
	void set_ambient_color( osg::Vec4 amb );
	void set_diffuse_color( osg::Vec4 diff );
	void set_diffuse_power( float d_power );
	void set_average_framerate( float rate );
	void set_eye_position( osg::Vec3d center );
	osg::ref_ptr<osgGA::NodeTrackerManipulator> set_track_node( osg::Node* node );
	void update_model_index();
	void set_export_filename( QString& filename );
	void set_eye_point( osg::Vec3f v, int i ) { _eye_pt[ _active_model ][i] = v; _eye_pt_set[ _active_model ] = true; }
	void set_center_point( osg::Vec3f v, int i ) { _center_pt[ _active_model ][i] = v; _eye_pt_set[ _active_model ] = true; }
	void set_up_direction( osg::Vec3f v, int i ) { _up[ _active_model ][i] = v; _eye_pt_set[ _active_model ] = true; }
	void set_crossbar_width( float value );
	void restrict_vertical_motion( bool yes_or_no );
	void set_minimum_velocity_thresold( float value );
	void set_trace_path( osg::AnimationPath* trace_path ) { _trace_path = trace_path; }
	void set_play_forward( bool on_or_off, int stride ) { _play_forward = on_or_off; _play_backward = false; _stride = stride; }
	void set_play_backward( bool on_or_off, int stride ) { _play_backward = on_or_off; _play_forward = false; _stride = stride; }
	void update_time_index();
	
	// Removal
	// Resizing
	// Transformation
	void show_invisible_objects( bool yes_or_no );
	
	// Conversion
	// Basic operations
	void load_collada( QString& filename );
	void load_data( osg::Group* group );
	void load_image( std::string filename, bool flip = false, bool useTextureRectangle = true );
	void load_model( osg::ArgumentParser& arguments );
	void load_osg( std::string filename );
	void move_offset( float x, float y, float z );
	void output_event( std::ostringstream const& eventString ) { if ( _export_file )_out_file[ _active_model ]->write( eventString.str().c_str(), eventString.str().size() ); }
	void reset_traversal() { _camera_callback[ _active_model ]->reset(); }
	void reset_motion();
	void select_scene( int id );
	void step_simulation( btScalar time_step, int max_sub_steps=1, 
	                      btScalar fixed_time_step=btScalar(1.)/btScalar(60.) );
	void switch_scene( int id );
	
	// Miscellaneous
	// Obsolete
	// Inapplicable
	
protected:
	
	void create_scene_graph( osg::Group* root );
	void create_physics_world( bool enable_physics );
	btDiscreteDynamicsWorld* initialize_physics();
	osg::Group* create_lights( osg::BoundingBox const* bb, osg::StateSet* root_state_set );
	void process_channel( std::vector< NodeOffsetPair >::iterator& itn );
	void process_start( std::vector< NodeOffsetPair >::iterator& itn );
	void process_crossbar( std::vector< NodeOffsetPair >::iterator& itn );
	void process_animation_path( std::vector< NodeOffsetPair >::iterator& itn );
	btRigidBody* process_physics( std::vector< NodeOffsetPair >::iterator& itn,
	                              Physics_State& pState, bool substitute_sphere = false );
	void process_reportable( std::vector< NodeOffsetPair >::iterator itn );
	void setup_tracking( int index );
	
	btRigidBody* create_bullet_box( osg::MatrixTransform* box,
	                                osg::Vec3 center );
	void move_to_new_start();

private:

	osg::ref_ptr<osg::Switch> _root;
	int _active_model;
	int _current_shader;
	int _start_id;
	osg::Program* _per_vertex_program;
	osg::Program* _per_pixel_program;
	osg::Shader* _per_vertex_vert_shader;
	osg::Shader* _per_vertex_frag_shader;
	osg::Shader* _per_pixel_vert_shader;
	osg::Shader* _per_pixel_frag_shader;
	osg::StateSet* _per_vertex_state_set;
	osg::StateSet* _per_pixel_state_set;
	std::vector< osg::LightSource* > _lights;
	std::vector< osg::BoundingBox* > _scene_bound;
	std::vector< btDefaultCollisionConfiguration* > _collision_configuration;
	std::vector< btCollisionDispatcher* > _dispatcher;
	std::vector< btSequentialImpulseConstraintSolver* > _solver;
	std::vector< btBroadphaseInterface* > _inter;
	
	std::vector< ColladaVisitor* > _cv;
	std::vector< std::vector< NodeOffsetPair > > _start_nodes;
	std::vector< Collision_World* > _dynamics_world;
	std::vector< osg::AnimationPath* > _animationPath;
	std::vector< osg::ref_ptr<osgGA::NodeTrackerManipulator> > _tracker;
	std::vector< btCollisionObject* > _reportable_objects;
	std::vector< btRigidBody* > _camera_body;
	std::vector< btRigidBody* > _sliding_body;
	std::vector< osg::ref_ptr< osg::MatrixTransform > > _homeOffset;
	std::vector< osg::ref_ptr< osg::MatrixTransform > > _xformer;
	std::vector< osg::ref_ptr< osg::MatrixTransform > > _positioned;
	std::vector< btTransform* >_camera_xform;

	bool _first, _tracking_initialized;
	osgbDynamics::MotionStateCallback* _motion_callback;
	std::vector< std::vector< osg::Vec3f > > _start_center;
	std::vector< std::vector< osg::Vec3f > > _start_cross;
	std::vector< std::vector< std::string > > _start_label;
	std::vector< std::vector< osg::Vec3d > > _eye_pt, _center_pt, _up;
	std::vector< bool > _eye_pt_set, _path_set;
	osg::Vec3f _min_point;
	float _min_dist, _max_dist, _bound_width;
	float _camera_mount_radius;
	std::vector< osg::ref_ptr< CameraMotionUpdateCallback > > _camera_callback;
	float _frame_rate;
	
	std::vector< osg::MatrixTransform* > _invisible_objects;
	bool _debug_physics, _show_invisibles, _disable_motion, _engage_physics;
	int _camera_collides_with, _channel_collides_with, 
		_reportable_collides_with, _ee_collides_with;
	bool _has_channel;
	array_type* _tangent_grid;

	bool _export_file;
	QString _export_file_name;
	std::vector< QString > _scene_file_name;
	std::vector< std::ofstream* > _out_file;
	bool _initial_load;
	std::vector< Config_Memento* > _configuration;
	osg::AnimationPath* _trace_path;
	bool _play_forward, _play_backward;
	int _stride;
};	// end of class Scene_Model

#endif
