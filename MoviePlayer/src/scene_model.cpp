
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
#include <algorithm>
using std::min;
using std::max;

#include <boost/regex.hpp>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Material>
#include <osg/Node>
#include <osg/PolygonMode>
#include <osg/PolygonOffset>
#include <osg/ShapeDrawable>
#include <osg/TextureRectangle>
#include <osgUtil/SmoothingVisitor>

#include <osgbDynamics/MotionState.h>
#include <osgbCollision/CollisionShapes.h>
#include <osgbCollision/RefBulletObject.h>
#include <osgbDynamics/RigidBodyAnimation.h>

// Need Globals.h to be the first of my #includes on windows 
// otherwise, _USE_MATH_DEFINES is not picked up by the compiler
#include <Globals.h> 
#include <CameraUpdateCallback.h>
#include <Scene_Model.h>
#include <shader_defs.h>

extern ContactAddedCallback gContactAddedCallback;

enum ShaderTypes { OSG_DEFAULT, PER_VERTEX, PER_PIXEL };

class LightNodeVisitor : public osg::NodeVisitor
{
  public:

  LightNodeVisitor()
	  : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
	{}

	void apply(osg::Node& node)
	{
		traverse(node);
	}

	void apply( osg::LightSource& node )
	{
		lights.push_back( &node );
	}

	std::vector< osg::LightSource* > lights;
};

struct ContactNodes
{
	int count;
	std::vector< osg::Node* > nodes;
	
	void addNode( osg::Node* node )
	{
		if ( count == 0 || ( node->getName() != nodes.back()->getName() ) )
		{
			nodes.push_back( node );

			count++;
		}
	}

	void reset()
	{
		count = 0;
		nodes.clear();
	}

	ContactNodes(): count( 0 ) {}
	
};
static ContactNodes contactCallback;

bool ContactCallback( btManifoldPoint& cp,
                      const btCollisionObjectWrapper* colObj0,
                      int partId0,
                      int index0,
                      const btCollisionObjectWrapper* colObj1,
                      int partId1,
                      int index1)
{
	Physics_State* ps0 = (Physics_State *)( colObj0->getCollisionObject()->getUserPointer() );
	Physics_State* ps1 = (Physics_State *)( colObj1->getCollisionObject()->getUserPointer() );
	if ( ps0 && ps1 && ( ps0->camera || ps1->camera ) && ( ps0->reportable && ps1->reportable ) )
	{
//		std::cout << "Contact! " << ps0->node->getName() << ", " << ps1->node->getName() << std::endl;
		if ( !ps0->camera )
			contactCallback.addNode( ps0->node );
		else
			contactCallback.addNode( ps1->node );
	}
	

	return true;
}

osg::Geometry* create_textured_quad(const osg::Vec3& pos, float width, float height, osg::Image* image,
                                    bool useTextureRectangle, bool option_flip );

static std::vector< btCollisionObject* >::iterator bco_it;

// Initialization
Scene_Model::Scene_Model(): _root( new osg::Switch ), _active_model( -1 ), _current_shader( 0 ),
                            _start_id( 0 ), _first( false ), _tracking_initialized( false ),
                            _motion_callback( 0 ), _min_point( 99999.f, 99999.f, 99999.f ),
                            _min_dist( 999999.f ), _max_dist( -999999.f ),
                            _camera_mount_radius( 0.f ),
                            _frame_rate( .0167f ),
                            _debug_physics( false ), 
                            _show_invisibles( false ), _disable_motion( false ), 
	_engage_physics( false ),
                            _camera_collides_with( COL_EE | COL_REPORTABLE | COL_CHANNEL ), 
                            _channel_collides_with( COL_CAMERA ), 
                            _reportable_collides_with( COL_CAMERA ), 
                            _ee_collides_with( COL_EE | COL_CAMERA ),
                            _has_channel( false ), _export_file( false ), _initial_load( true ),
							_trace_path( 0 ),
                            _play_forward( false ), _play_backward( false )

{
	// Setup shader definitions
   _per_vertex_program = new osg::Program;
   _per_vertex_program->setName( "per_vertex_lighting_shader" );
   _per_vertex_vert_shader = new osg::Shader( osg::Shader::VERTEX, lighting_vert_source );
   _per_vertex_frag_shader = new osg::Shader( osg::Shader::FRAGMENT, lighting_frag_source );
   _per_vertex_program->addShader( _per_vertex_vert_shader );
   _per_vertex_program->addShader( _per_vertex_frag_shader );

   _per_vertex_state_set = new osg::StateSet;
   _per_vertex_state_set->addUniform( new osg::Uniform( "numLights", 4 ) );
   _per_vertex_state_set->addUniform( new osg::Uniform( "attenuation", 0.1f ) );
   _per_vertex_state_set->addUniform( new osg::Uniform( "diffuse_color", osg::Vec4( 1.f, 1.f, 1.f, 1.f ) ) );
   _per_vertex_state_set->addUniform( new osg::Uniform( "ambient_color", osg::Vec4( 1.f, 1.f, 1.f, 1.f ) ) );
   _per_vertex_state_set->addUniform( new osg::Uniform( "eye_pos", osg::Vec4( 0.f, 0.f, 0.f, 1.f ) ) );
   _per_vertex_state_set->setAttributeAndModes( _per_vertex_program, 
                                                osg::StateAttribute::ON | 
                                                osg::StateAttribute::OVERRIDE );

   _per_pixel_program = new osg::Program;
   _per_pixel_program->setName( "per_vertex_lighting_shader" );
   _per_pixel_vert_shader = new osg::Shader( osg::Shader::VERTEX, lighting_vert_source_pixel );
   _per_pixel_frag_shader = new osg::Shader( osg::Shader::FRAGMENT, lighting_frag_source_pixel );
   _per_pixel_program->addShader( _per_pixel_vert_shader );
   _per_pixel_program->addShader( _per_pixel_frag_shader );

   _per_pixel_state_set = new osg::StateSet;
   _per_pixel_state_set->addUniform( new osg::Uniform( "numLights", 4 ) );
   _per_pixel_state_set->addUniform( new osg::Uniform( "attenuation", 0.1f ) );
   _per_pixel_state_set->addUniform( new osg::Uniform( "diffuse_color", osg::Vec4( 1.f, 1.f, 1.f, 1.f ) ) );
   _per_pixel_state_set->addUniform( new osg::Uniform( "ambient_color", osg::Vec4( 1.f, 1.f, 1.f, 1.f ) ) );
   _per_pixel_state_set->addUniform( new osg::Uniform( "eye_pos", osg::Vec4( 0.f, 0.f, 0.f, 1.f ) ) );
   _per_pixel_state_set->setAttributeAndModes( _per_pixel_program, 
                                               osg::StateAttribute::ON | 
                                               osg::StateAttribute::OVERRIDE );

   gContactAddedCallback = &ContactCallback;

   update_model_index();
}

// Duplication
Scene_Model::Scene_Model( Scene_Model const& c )
{
}

Scene_Model&
Scene_Model::operator=( Scene_Model const& c )
{
	return *this;
}

// Destruction
Scene_Model::~Scene_Model()
{
	for ( uint32_t i = 0; i < _camera_callback.size(); i++ )
	{
		if ( _camera_callback[i] )
			_camera_callback[i]->clear();
	}
	
   if ( _root )
	   _root.release();

   for ( uint32_t i = 0; i < _dynamics_world.size(); i++ )
   {
	   if ( _dynamics_world[i] != 0 )
	   {
		   delete _dynamics_world[i]->world;
		   delete _dynamics_world[i]->concave;
		   delete _dynamics_world[i];
		   delete _inter[i];
		   delete _solver[i];
		   delete _dispatcher[i];
		   delete _collision_configuration[i];
		   delete _camera_xform[i];
	   }
	   delete _camera_body[i];
	   
	   if ( _out_file[i] != 0 )
	   {
		   _out_file[i]->flush();
		   _out_file[i]->close();
		   delete _out_file[i];
	   }
   }
}

// Access

std::vector< osg::Node* > const&
Scene_Model::contact_nodes()
{
	return contactCallback.nodes;
}

osg::Switch* 
Scene_Model::current_scene()
{
	osg::Switch* result = 0;
	
	// _active_model is non-negative if scene has been loaded
	if ( _active_model >= 0 )
		result = dynamic_cast<osg::Switch*>( _root->getChild( _active_model ) );

	return result;
}

// Measurement
// Comparison
// Status report

int
Scene_Model::contact_count() const 
{
	return contactCallback.count; 
}

bool 
Scene_Model::physics_enabled() const 
{
	if ( _dynamics_world.size() == 0 )
		return false;
	else if ( _dynamics_world[ _active_model ] == 0 )
		return false;
	else
		return _dynamics_world[ _active_model ]->world != 0; 
}

// Status setting
// Cursor movement
// Element change

void
Scene_Model::add_rigid_body( btRigidBody* body, collision_types col, int interaction,
                             bool camera_body ) 
{
	Physics_State* ps = (Physics_State *)( body->getUserPointer() );

	// If `body' is concave and reportable then handle differently. Concave
	// objects do not report if they are "inside"
	if ( !ps->convex && ps->reportable )
		_dynamics_world[ _active_model ]->concave->addRigidBody( body, col, interaction );
	else
		_dynamics_world[ _active_model ]->world->addRigidBody( body, col, interaction );
	if ( camera_body )
		_camera_body.back( ) = body;
}

void
Scene_Model::change_shader( int value )
{
	// _active_model is non-negative if scene has been loaded
	if ( _active_model >= 0 )
		current_scene()->setSingleChildOn( value );

	_current_shader = value;
}

void 
Scene_Model::clear_tracker_data()
{
	// If the tracker is valid, set the velocity vector to 0 to stop any
	// current movement
	if ( _tracker.size() > 0 && _tracker[ _active_model ] != 0 )
	{
		Motion_Data* md = (Motion_Data*)_tracker[ _active_model ]->getUserData();
		md->motion.set( 0.f, 0.f, 0.f );
	}
}

void 
Scene_Model::set_ambient_color( osg::Vec4 amb )	
{ 
	// case OSG_DEFAULT:
	for ( uint32_t i = 0; i < get_lights().size(); i++ )
		get_lights()[ i ]->getLight()->setAmbient( amb );

	// _active_model is non-negative if scene has been loaded
	if ( _active_model >= 0 )
	{
		// case PER_VERTEX:
		current_scene()->getChild( PER_VERTEX )->getOrCreateStateSet()->getUniform( "ambient_color" )->set( amb ); 
		
		// case PER_PIXEL:
		current_scene()->getChild( PER_PIXEL )->getOrCreateStateSet()->getUniform( "ambient_color" )->set( amb );
	}
}
   
void 
Scene_Model::set_diffuse_color( osg::Vec4 diff )	
{ 
	// case OSG_DEFAULT:
	for ( uint32_t i = 0; i < get_lights().size(); i++ )
		get_lights()[ i ]->getLight()->setDiffuse( diff );

	// _active_model is non-negative if scene has been loaded
	if ( _active_model >= 0 )
	{
		// case PER_VERTEX:
		current_scene()->getChild( PER_VERTEX )->getOrCreateStateSet()->getUniform( "diffuse_color" )->set( diff ); 
		
		// case PER_PIXEL:
		current_scene()->getChild( PER_PIXEL )->getOrCreateStateSet()->getUniform( "diffuse_color" )->set( diff );
	}
}
   
void 
Scene_Model::set_diffuse_power( float d_power )	
{
	// case OSG_DEFAULT:
	for ( uint32_t i = 0; i < get_lights().size(); i++ )
		get_lights()[ i ]->getLight()->setConstantAttenuation( 1.0f / d_power );

	// _active_model is non-negative if scene has been loaded
	if ( _active_model >= 0 )
	{
		// case PER_VERTEX:
		current_scene()->getChild( PER_VERTEX )->getOrCreateStateSet()->getUniform( "attenuation" )->set( d_power ); 

		// case PER_PIXEL:
		current_scene()->getChild( PER_PIXEL )->getOrCreateStateSet()->getUniform( "attenuation" )->set( d_power );
	}
}

void
Scene_Model::set_average_framerate( float rate )
{ 
	_frame_rate = rate; 
	if ( _camera_callback.size() > 0 && _camera_callback[ _active_model ] ) 
		_camera_callback[ _active_model ]->set_average_framerate( rate );
}

void 
Scene_Model::set_eye_position( osg::Vec3d center )
{
	osg::Uniform* uniform;

	switch( _current_shader )
	{
		case 0: // OSG Default
			break;
		case 1: // Per Vertex
			uniform = _per_vertex_state_set->getUniform( "eye_pos" );
			uniform->set( osg::Vec4f( center, 1.f ) );
			break;
		case 2: // Per Pixel
			uniform = _per_pixel_state_set->getUniform( "eye_pos" );
			uniform->set( osg::Vec4f( center, 1.f ) );
			break;
	}
}

void
Scene_Model::update_model_index() 
{
	_active_model = _root->getNumChildren();
	_path_set.push_back( false );
	_animationPath.push_back( 0 );

	_camera_body.push_back( 0 );
	_sliding_body.push_back( 0 );
	_camera_xform.push_back( new btTransform() );
	_camera_callback.push_back( 0 );

	_scene_file_name.push_back( "" );
	
	_out_file.push_back( 0 );
	ColladaVisitor* cv = new ColladaVisitor;
	_cv.push_back( cv );

	if ( _initial_load )
		_configuration.push_back( new Config_Memento );
	else
		// If not the first world, we duplicate the last Config_Memento
		_configuration.push_back( new Config_Memento( *( _configuration.back() ) ) );
	
	std::vector< NodeOffsetPair > it;
	_start_nodes.push_back( it );
	
	std::vector< osg::Vec3f > center;
	_start_center.push_back( center );

	std::vector< osg::Vec3f > cross;
	_start_cross.push_back( cross );

	std::vector< std::string > label;
	_start_label.push_back( label );

	std::vector< osg::Vec3d > eye;
	_eye_pt.push_back( eye );

	std::vector< osg::Vec3d > cpt;
	_center_pt.push_back( cpt );

	std::vector< osg::Vec3d > up;
	_up.push_back( up );

	_eye_pt_set.push_back( false );

	_dynamics_world.push_back( 0 );
	_collision_configuration.push_back( 0 );
	_dispatcher.push_back( 0 );
	_solver.push_back( 0 );
	_inter.push_back( 0 );

	_scene_bound.push_back( 0 );
	_homeOffset.push_back( 0 );
	_positioned.push_back( 0 );
	_xformer.push_back( 0 );
	_tracker.push_back( 0 );
}

void
Scene_Model::set_export_filename( QString& filename )
{
	_export_file_name = filename;
	std::ofstream* out_file = new std::ofstream( _export_file_name.toLatin1().data() );
	if ( _out_file.back() != 0 )
		_out_file.back()->close();
	
	_out_file.back() = out_file;
}


osg::ref_ptr<osgGA::NodeTrackerManipulator> 
Scene_Model::set_track_node( osg::Node* node )
{
	osg::ref_ptr<osgGA::NodeTrackerManipulator> tracker;

	tracker = new osgGA::NodeTrackerManipulator;
	tracker->setTrackerMode( osgGA::NodeTrackerManipulator::NODE_CENTER_AND_ROTATION );
	tracker->setRotationMode( osgGA::NodeTrackerManipulator::TRACKBALL );
	tracker->setTrackNode( node );
	Motion_Data* md = new Motion_Data;
	tracker->setUserData( (osg::Referenced*) md );

	return tracker;
}

void 
Scene_Model::set_crossbar_width( float value ) 
{ 
	_dynamics_world[ _active_model ]->crossbar_width = value;
	// Only set if we have a crossbar, and we're not in movie mode
	if ( _dynamics_world[ _active_model ]->has_crossbar && _trace_path == 0 )
	{
		CameraPathUpdateCallback* cb = dynamic_cast< CameraPathUpdateCallback* >( _camera_callback[ _active_model].get() );
		cb->update_crossbar();
	}
}

void
Scene_Model::restrict_vertical_motion( bool yes_or_no )
{
	if ( _camera_callback.size() > 0 && _camera_callback[ _active_model ] )
		_camera_callback[ _active_model ]->restrict_vertical_motion( yes_or_no );
}

void 
Scene_Model::set_minimum_velocity_thresold( float value ) 
{ 
	// Need to check if the callback is valid, because we may be called before
	// things are setup
	if ( _camera_callback.size() > 0 && _camera_callback[ _active_model ] )
		_camera_callback[ _active_model ]->set_minimum_velocity_thresold( value );
}

void 
Scene_Model::update_time_index()
{ 
	if ( _camera_callback.size() > 0 && _camera_callback[ _active_model ] )
	{
		CameraTraceUpdateCallback* cb = dynamic_cast< CameraTraceUpdateCallback* >( _camera_callback[ _active_model].get() );
		if ( _play_forward )
		{
			cb->increment_time_index( _stride );
}
		else if ( _play_backward )
		{
			cb->decrement_time_index( _stride );
		}
	}
}

// Removal
// Resizing
// Transformation
void
Scene_Model::show_invisible_objects( bool yes_or_no )
{
	_show_invisibles = yes_or_no;
	
	std::vector< osg::MatrixTransform* >::iterator it;
	
	if ( yes_or_no )
	{
		for ( it = _invisible_objects.begin(); it != _invisible_objects.end(); it++ )
			(*it)->setNodeMask( ~INVISIBLE_MASK );
	}
	else
	{
		for ( it = _invisible_objects.begin(); it != _invisible_objects.end(); it++ )
			(*it)->setNodeMask( INVISIBLE_MASK );
	}
}

// Conversion
// Basic operations

void
Scene_Model::load_collada( QString& filename )
{
	if ( _tracking_initialized ) 
	{
		_xformer[ _active_model ]->setUpdateCallback( 0 );
		_camera_callback[ _active_model ] = 0;
//		current_scene()->removeChild( _xformer[ _active_model ] );
	}
	
	osg::ref_ptr< osg::Group > root = new osg::Group();
	osg::ref_ptr< osg::Node > nodeDB = osgDB::readNodeFile( filename.toStdString() );
	if( !nodeDB.valid() )
	{
		osg::notify( osg::FATAL ) << "Can't find \"" << filename.toStdString() << std::endl;
	}
	else
	{
		bool has_start = false;
		bool start_set_to_camera = false;
	
		if ( !_initial_load )
			update_model_index();
		else
			_initial_load = false;
		
		create_physics_world( true );
		
		// Initialize the cross width so there is always a value
		_cv.back()->apply( *(osg::Group*)(nodeDB.get()) );

		btRigidBody* body;
		std::vector< NodeOffsetPair >::iterator itn, path_itn;
		std::ofstream coord_file;
		_scene_file_name.back() = filename;
		coord_file.open( filename.replace( ".dae", ".coords" ).toLatin1().data() );			
		
		for ( itn = _cv.back()->others.begin() ; itn < _cv.back()->others.end(); itn++ )
		{
			Physics_State pState;
		
			coord_file << itn->second->getName() << " - " << itn->first.x() << " " << itn->first.y() << " " << itn->first.z() << std::endl;
			body = process_physics( itn, pState );
			itn->second->setMatrix( osg::Matrix::translate( itn->first ) );
			root->addChild( itn->second );
			if ( body )
				add_rigid_body( body, COL_EE, _ee_collides_with );
		}

		NodeOffsetPair camera_node;

		// Process invisible items 
		for ( itn = _cv.back()->nodes.begin() ; itn < _cv.back()->nodes.end(); itn++ )
		{
			Physics_State pState;
			std::string name = itn->second->getName();

			coord_file << itn->second->getName() << " - " << itn->first.x() << " " << itn->first.y() << " " << itn->first.z() << std::endl;
					
			if( name.find( "channel" ) != std::string::npos )
			{
				process_channel( itn );
				body = process_physics( itn, pState );
				add_rigid_body( body, COL_CHANNEL, _channel_collides_with );
				itn->second->setMatrix( osg::Matrix::translate( itn->first ) );
				root->addChild( itn->second );
				_has_channel = true;
			}
			else if( name.find( "_start_" ) != std::string::npos )
			{
				process_start( itn );
				itn->second->setMatrix( osg::Matrix::translate( itn->first ) );
				_start_nodes.back().push_back( *itn );
				root->addChild( itn->second );
				has_start = true;
			}
			else if( name == "_path_" )
			{
				// Delay processing until we know start_center is set.
				path_itn = itn;
				// Set in update_model_index
				_path_set.back() = true;
			}
			else if( name == "_crossbar_pic_" )
			{
				process_crossbar( itn );
			}
			else
			{
				body = process_physics( itn, pState, name == "_camera_block_pm_" );
				itn->second->setMatrix( osg::Matrix::translate( itn->first ) );
				root->addChild( itn->second );

				if ( pState.reportable )
				{
					add_rigid_body( body, COL_REPORTABLE, _reportable_collides_with );
				}
				else if ( name == "_camera_block_pm_" )
				{
					Physics_State* ps = (Physics_State *)( body->getUserPointer() );
					ps->reportable = true;
					ps->camera = true;
					body->setDamping( 0.f, 1.f );
					body->setFriction( 0.f );
					add_rigid_body( body, COL_CAMERA, _camera_collides_with, true );
					body->setCollisionFlags( body->getCollisionFlags() |
					                         btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
					// Without a start location, the initial camera position is the first listed
					if ( !has_start )
					{
						osg::BoundingSphere bbox = itn->second->getBound();
						_start_center.back().insert( _start_center.back().begin(), bbox.center() );
						_start_cross.back().insert( _start_cross.back().begin(), osg::Vec3f( 1, 0, 0 ) );
						_start_label.back().insert( _start_label.back().begin(), "camera" );
						start_set_to_camera = true;
					}
					
					camera_node = *itn;
					_camera_xform.back() = new btTransform( body->getWorldTransform() );
				}
				else
				{
					if ( body )
						add_rigid_body( body, COL_EE, _ee_collides_with );
				}
			}

			_invisible_objects.push_back( itn->second );				

			if ( !_show_invisibles )
				itn->second->setNodeMask( INVISIBLE_MASK );
		}				

		if ( has_path() )
		{
			process_animation_path( path_itn );
			path_itn->second->setMatrix( osg::Matrix::translate( path_itn->first ) );
			root->addChild( path_itn->second );
		}
		
		// If we set the start to the camera position, and then later processed a
		// start position, remove the camera values and set to the new values
		if ( has_start && start_set_to_camera )
		{
			_start_center.back().erase( _start_center.back().begin() );
			_start_cross.back().erase( _start_cross.back().begin() );
			_start_label.back().erase( _start_label.back().begin() );
		}
		
		if ( _camera_body[ _active_model ] != 0 )
		{
			move_to_new_start();
		}
			
		osg::Node* dbgRoot = _dynamics_world[ _active_model ]->debug_drawer->getSceneGraph();
		root->addChild( dbgRoot );
		create_scene_graph( root );
		
		osg::StateSet* root_state_set = root->getOrCreateStateSet();
		osg::Group* l = create_lights( _scene_bound.back(), root_state_set ) ;
		root->addChild( l );

		LightNodeVisitor lv;
		lv.apply( *root );
		_lights = lv.lights;
		for( uint32_t i = 0; i < _lights.size(); i++ )
		{
			_lights[i]->setLocalStateSetModes( osg::StateAttribute::ON );
			_lights[i]->setStateSetModes( *root_state_set, osg::StateAttribute::ON );
		}

		osg::BoundingBox const * const bb = scene_bound();
		_bound_width = 2.f * bb->radius();
		
		coord_file << std::endl << "Origin - " << bb->xMin() << " " << bb->yMin() << " " << bb->zMin() << std::endl;
		coord_file.close();
		
		for ( uint32_t i = 0; i < _start_center.back().size(); i++ )
		{
			_eye_pt.back().push_back( _start_center.back()[i] - _start_cross.back()[i] * 0.1f );
			_center_pt.back().push_back( _start_center.back()[i] );
			_up.back().push_back( osg::Vec3f( 0, 0, 1 ) );
		}
		
		// Setup code for tracking
		setup_tracking( _start_center.back().size() - 1 );
			
		if ( _camera_body[ _active_model ] != 0 )
		{
			if( _trace_path != 0 )
			{
				// Copy the path for bashing later
				osg::AnimationPath* ap = new osg::AnimationPath;
				ap->setLoopMode( osg::AnimationPath::LOOP );
				osg::AnimationPath::TimeControlPointMap& tcpm = _trace_path->getTimeControlPointMap();
				osg::AnimationPath::TimeControlPointMap::iterator it = tcpm.begin();
				for( ; it != tcpm.end(); it++ )
				{
					// Shift the trace coordinates back into world coordinates
					// (they're initially in scene (bound) coordinates )
					osg::Vec3d pt( (*it).second.getPosition().x() + scene_bound()->xMin(), 
					               (*it).second.getPosition().y() + scene_bound()->yMin(),
					               (*it).second.getPosition().z() );

					ap->insert( (*it).first, osg::AnimationPath::ControlPoint( pt, (*it).second.getRotation() ) );
				}
				
				_trace_path = ap;
				osg::Matrix mat = osg::Matrix::rotate( -1.57079, osg::Vec3d( 0, 1, 0 ) ) * osg::Matrix::rotate( 1.57079, osg::Vec3d( 1, 0, 0 ) );
				_positioned.back()->setMatrix( mat );
				
				_camera_callback.back() = new CameraTraceUpdateCallback( _camera_body[ _active_model ],
				                                                         _dynamics_world[ _active_model ],
				                                                         _tracker[ _active_model ],
				                                                         _xformer[ _active_model ],
				                                                         _trace_path );
			}
			else if ( has_path() )
			{
				osg::Geode* g = new osg::Geode;
				g->addDrawable(new osg::ShapeDrawable(new osg::Box( osg::Vec3f(0.f, 0.f, 0.f), 1.f )));
				osg::MatrixTransform* m = new osg::MatrixTransform;
				m->setDataVariance( osg::Object::STATIC );
				m->addChild( g );
				_sliding_body.back() = create_bullet_box( m, osg::Vec3f(0.f, 0.f, 0.f) );
				btVector3 x_axis( 1, 0, 0 );
				osg::Vec3d sc = _start_cross[ _active_model ][0];
				double rot = M_PI_2 - acosf( x_axis.dot( btVector3( sc.x(), sc.y(), sc.z() ) ) );
				btQuaternion q( btVector3( 0, 0, 1 ), rot );
				btTransform t;
				t.setRotation( q );
				_sliding_body.back()->setCenterOfMassTransform( t );
				
				osgbDynamics::MotionState* motion = dynamic_cast< osgbDynamics::MotionState* >( _sliding_body.back()->getMotionState() );
				osg::AnimationPath::ControlPoint cp;
				_animationPath[ _active_model ]->getInterpolatedControlPoint( 0.0, cp );
				osg::Vec3d v = cp.getPosition();
				
				m->setMatrix( osg::Matrix::translate( v ) );
				motion->setTransform( m );
 
				Physics_State* ps = new Physics_State( );
				ps->node = m;
				_sliding_body.back()->setUserPointer( (void*) ps );

				add_rigid_body( _sliding_body.back(), COL_CHANNEL, _channel_collides_with );
				root->addChild( m );

				_invisible_objects.push_back( m );				
				if ( !_show_invisibles )
					m->setNodeMask( INVISIBLE_MASK );

				_camera_callback.back() = new CameraPathUpdateCallback( _camera_body[ _active_model ],
				                                                        _sliding_body[ _active_model ], 
				                                                        _dynamics_world[ _active_model ],
				                                                        _tracker[ _active_model ], 
				                                                        _animationPath[ _active_model ],
				                                                        0.0, _start_cross[ _active_model ][0] );
			}
			
			else
				_camera_callback.back() = new CameraBlockUpdateCallback( _camera_body[ _active_model ], _dynamics_world[ _active_model ], _tracker[ _active_model ], _start_cross.back()[0] );
			_camera_callback.back()->set_average_framerate( _frame_rate );

			osgbDynamics::MotionState* motion = static_cast< osgbDynamics::MotionState* >( _camera_body[ _active_model ]->getMotionState() );

			if ( _has_channel )
				motion->getCallbackList().push_back( new ChannelMovementCallback( _xformer[ _active_model ], _tangent_grid, _min_point, _min_dist ) );
			else if( _trace_path != 0 )
				motion->getCallbackList().push_back( new MovementCallback( _xformer[ _active_model ] ) );   
			else if ( has_path() )
				motion->getCallbackList().push_back( new PathMovementCallback( _xformer[ _active_model ] ) );   
			else
				motion->getCallbackList().push_back( new MovementCallback( _xformer[ _active_model ] ) );   
		}	
	}
}

void
Scene_Model::load_image( std::string filename, bool flip, bool useTextureRectangle )
{
	std::string imageFile;
	osg::Image* image;
	osg::ref_ptr<osg::Node> loadedModel;
	osg::Vec3 pos(0.0f,0.0f,0.0f);
	osg::Vec3 topleft = pos;
	osg::Vec3 bottomright = pos;
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
       
	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	image = osgDB::readImageFile( filename );
	osg::notify( osg::NOTICE ) << "image->s()" << image->s() << " image-t()=" << image->t() << 
		" aspectRatio=" << image->getPixelAspectRatio() << std::endl;

	float width = image->s() * image->getPixelAspectRatio();
	float height = image->t();

	osg::ref_ptr<osg::Drawable> drawable = create_textured_quad( pos, width, height, image, 
	                                                             useTextureRectangle, flip );
                
	if ( image->isImageTranslucent() )
	{
		osg::notify( osg::NOTICE ) << "Transparent movie, enabling blending." << std::endl;

		drawable->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );
		drawable->getOrCreateStateSet()->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );
	}

	geode->addDrawable( drawable.get() );

	bottomright = pos + osg::Vec3( width, height, 0.0f );

	pos.z() += height * 1.05f;

	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild( geode );
	update_model_index();
	create_scene_graph( group );
}

void
Scene_Model::load_model( osg::ArgumentParser& arguments )
{
   // load the scene.
   osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFiles( arguments );
      
   if (loadedModel) 
   {
	   osg::ref_ptr<osg::Group> group = new osg::Group;
	   group->addChild( loadedModel );
	   update_model_index();
	   create_scene_graph( group );
   }
}

void
Scene_Model::load_osg( std::string filename )
{
   // load the scene.
   osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile( filename );
      
   if (!loadedModel)
      std::cout << " No data loaded." << std::endl;
   else
   {
	   osg::ref_ptr<osg::Group> group = new osg::Group;
	   group->addChild( loadedModel );
	   update_model_index();
	   create_scene_graph( group );
   }
}

void
Scene_Model::reset_motion()
{
	_camera_callback[ _active_model ]->home();
	_camera_body[ _active_model ]->setWorldTransform( *( _camera_xform[ _active_model ] ) );
	_tracker[ _active_model ]->setByMatrix( osg::Matrixd::identity() );
	_first = true;
}

void 
Scene_Model::select_scene( int id )
{
	_active_model = id;
	_root->setSingleChildOn( id );
	((osg::Switch*)(_root->getChild( 0 ) ) )->setSingleChildOn( _current_shader );

	LightNodeVisitor lv;
	lv.apply( *( _root->getChild( id ) ) );
	_lights = lv.lights;
}

void 
Scene_Model::step_simulation( btScalar time_step, int max_sub_steps, btScalar fixed_time_step )
{ 
	contactCallback.reset();
		
	if ( _debug_physics )
		_dynamics_world[ _active_model ]->debug_drawer->BeginDraw();
	
	if ( _first )
	{
		btScalar ts = time_step;
		
		if ( _engage_physics )
			ts = 0.f;
		
		_dynamics_world[ _active_model ]->world->stepSimulation( ts, max_sub_steps, fixed_time_step );
		_dynamics_world[ _active_model ]->world->stepSimulation( 2 * ts, max_sub_steps, fixed_time_step );
		_first = false;
	}
	
	if ( _engage_physics )
		_dynamics_world[ _active_model ]->world->stepSimulation( time_step, max_sub_steps, fixed_time_step );
	else
		_dynamics_world[ _active_model ]->world->stepSimulation( 0.f, max_sub_steps, fixed_time_step );

	if ( _debug_physics )
	{
		_dynamics_world[ _active_model ]->world->debugDrawWorld();
		_dynamics_world[ _active_model ]->debug_drawer->EndDraw();
	}
	
/*
	btVector3 start_pt = camera_position();
	btVector3 end_pt = start_pt + btVector3( _bound_width, 0, 0 );
	
	btCollisionWorld::AllHitsRayResultCallback ray_callback( start_pt, end_pt );

	ray_callback.m_collisionFilterGroup = COL_CAMERA;
	// Perform raycast
	_dynamics_world[ _active_model ]->concave->rayTest( start_pt, end_pt, 
	                                                    ray_callback );
	
	if( ray_callback.hasHit() ) 
	{
		//std::cout << "Object Count: " << ray_callback.m_collisionObjects.size() << ", Hit Count: " << ray_callback.m_hitPointWorld.size() << std::endl;
	}
*/
}

void 
Scene_Model::switch_scene( int id )
{
	if ( _tracking_initialized ) 
	{
		_xformer[ _active_model ]->setUpdateCallback( 0 );
		_camera_callback[ _active_model ]->clear();
		_camera_callback[ _active_model ] = 0; // Not a leak as it's a ref_ptr
		Motion_Data* md = new Motion_Data;
		_tracker[ _active_model ]->setUserData( (osg::Referenced*) md );

//		current_scene()->removeChild( _xformer );
	}

	_out_file[ _active_model ]->flush();
	_active_model = id;

	select_scene( id );

	if ( _camera_body[ _active_model ] != 0 )
	{
		if ( has_path() )
		{
			btVector3 x_axis( 1, 0, 0 );
			osg::Vec3d sc = _start_cross[ id ][0];
			double rot = M_PI_2 - acosf( x_axis.dot( btVector3( sc.x(), sc.y(), sc.z() ) ) );
			btQuaternion q( btVector3( 0, 0, 1 ), rot );
			btTransform t;
			t.setRotation( q );
			_sliding_body[ id ]->setCenterOfMassTransform( t );
				
			osgbDynamics::MotionState* motion = dynamic_cast< osgbDynamics::MotionState* >( _sliding_body[ _active_model ]->getMotionState() );
			osg::AnimationPath::ControlPoint cp;
			_animationPath[ id ]->getInterpolatedControlPoint( 0.0, cp );
			osg::Vec3d v = cp.getPosition();
				
			osg::MatrixTransform* m = motion->getTransform()->asMatrixTransform();
			m->setMatrix( osg::Matrix::translate( v ) );
			
			_camera_callback[ id ] = new CameraPathUpdateCallback( _camera_body[ id ], _sliding_body[ id ], _dynamics_world[ id ], _tracker[ _active_model ], _animationPath[ id ], 0.0, _start_cross[ id ][0] );
		}
		else
			_camera_callback[ id ] = new CameraBlockUpdateCallback( _camera_body[ id ], _dynamics_world[ id ], _tracker[ _active_model ], _start_cross[ id ][0] );
		_camera_callback[ id ]->set_average_framerate( _frame_rate );
	}
}

// Miscellaneous
// Obsolete
// Inapplicable
// Implementation

osg::Geometry*
create_textured_quad(const osg::Vec3& pos, float width, float height, osg::Image* image,
                     bool useTextureRectangle, bool option_flip )
{
    bool flip = image->getOrigin()==osg::Image::TOP_LEFT;
    if (option_flip) flip = !flip;

    if (useTextureRectangle)
    {
        osg::Geometry* pictureQuad = osg::createTexturedQuadGeometry(pos,
                                           osg::Vec3(width,0.0f,0.0f),
                                           osg::Vec3(0.0f,0.0f,height),
                                           0.0f, flip ? image->t() : 0.0, image->s(), flip ? 0.0 : image->t());

        osg::TextureRectangle* texture = new osg::TextureRectangle(image);
        texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);


        pictureQuad->getOrCreateStateSet()->setTextureAttributeAndModes(0,
                                                                        texture,
                                                                        osg::StateAttribute::ON);

        return pictureQuad;
    }
    else
    {
        osg::Geometry* pictureQuad = osg::createTexturedQuadGeometry(pos,
                                           osg::Vec3(width,0.0f,0.0f),
                                           osg::Vec3(0.0f,0.0f,height),
                                           0.0f, flip ? 1.0f : 0.0f , 1.0f, flip ? 0.0f : 1.0f);

        osg::Texture2D* texture = new osg::Texture2D(image);
        texture->setResizeNonPowerOfTwoHint(false);
        texture->setFilter(osg::Texture::MIN_FILTER,osg::Texture::LINEAR);
        texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
        texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);


        pictureQuad->getOrCreateStateSet()->setTextureAttributeAndModes(0,
                    texture,
                    osg::StateAttribute::ON);

        return pictureQuad;
    }
}

btDiscreteDynamicsWorld* 
Scene_Model::initialize_physics()
{
	_collision_configuration.back() = new btDefaultCollisionConfiguration() ;
	_dispatcher.back() = new btCollisionDispatcher( _collision_configuration.back() ) ;
	_solver.back() = new btSequentialImpulseConstraintSolver ;
	_inter.back() = new btDbvtBroadphase() ;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld( _dispatcher.back(), _inter.back(), _solver.back(), _collision_configuration.back() );

	dynamicsWorld->setGravity( btVector3( 0, 0, -10 ) );
	dynamicsWorld->getDispatchInfo().m_useContinuous=true;

	return( dynamicsWorld );
}

void
Scene_Model::create_scene_graph( osg::Group* root )
{
	_scene_bound.back() = new osg::BoundingBox;
   _scene_bound.back()->expandBy( root->getBound() );
   
	// create statesets.
   osg::StateSet* root_state_set = new osg::StateSet;
   root_state_set->setMode( GL_CULL_FACE, osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE );
   root->setStateSet(root_state_set);

   osg::Switch* switch_shader = new osg::Switch;
   osg::Group* g = new osg::Group;
   g->addChild( root );
   switch_shader->addChild( g );
   
   g = new osg::Group;
   g->setStateSet( _per_vertex_state_set );
   g->addChild( root );
   switch_shader->addChild( g );

   g = new osg::Group;
   g->setStateSet( _per_pixel_state_set );
   g->addChild( root );
   switch_shader->addChild( g );

   switch_shader->setSingleChildOn( _current_shader );
   
   _root->setAllChildrenOff();
   _root->addChild( switch_shader, true );
}

void
Scene_Model::create_physics_world( bool enable_physics )
{
	Collision_World* world = new Collision_World();
	world->world = initialize_physics();
	world->concave = initialize_physics();
	_dynamics_world.back() = world;
	btContactSolverInfo& info = _dynamics_world.back()->world->getSolverInfo();
	info.m_splitImpulse = true; //enable split impulse feature 

	world->debug_drawer = new osgbCollision::GLDebugDrawer();
	world->debug_drawer->setDebugMode( ~btIDebugDraw::DBG_DrawText );
	world->debug_drawer->setEnabled( _debug_physics );
	world->world->setDebugDrawer( world->debug_drawer );
}

osg::Group* 
Scene_Model::create_lights( osg::BoundingBox const* bb, osg::StateSet* root_state_set )
{
	osg::Group* lightGroup = new osg::Group;
    
	for ( int i = 0; i <= 7; i++ )
	{
		if ( i & 4 )
		{
			std::cout << i << " " << bb->corner( i ).x() << " " << bb->corner( i ).y() << " " << bb->corner( i ).z() << std::endl;
			// create a spot light.
			osg::Light* myLight1 = new osg::Light;
			myLight1->setLightNum( i );
			myLight1->setPosition( osg::Vec4(bb->corner( i ) + osg::Vec3f( 0, 0, 5 ), 1.0f) );
			myLight1->setAmbient( osg::Vec4( 0.1f, 0.1f, 0.1f, 1.0f ) );
			myLight1->setDiffuse( osg::Vec4( 0.1f, 0.1f, 0.1f, 1.0f ) );
			myLight1->setConstantAttenuation( 0.1 );
			myLight1->setLinearAttenuation( 0.0 );
			myLight1->setQuadraticAttenuation( 0.0 );
			
			osg::LightSource* lightS1 = new osg::LightSource;    
			lightS1->setLight( myLight1 );
			lightS1->setLocalStateSetModes( osg::StateAttribute::ON ); 
	  
			lightS1->setStateSetModes( *root_state_set, osg::StateAttribute::ON );
			lightGroup->addChild( lightS1 );
		}
	}

	return lightGroup;
}

void
Scene_Model::process_reportable( std::vector< NodeOffsetPair >::iterator itn )
{
	// Now process them
	btCollisionObject* colObj = new btCollisionObject;
					
	// Need to set the matrix before the collision, otherwise the position
	// doesn't get updated
	itn->second->setMatrix( osg::Matrix::translate( itn->first ) );

	/*  OSGBBULLET CODE */
	btCollisionShape * collision = osgbCollision::btTriMeshCollisionShapeFromOSG( itn->second );
	colObj->setCollisionShape( collision );

	void* ptr = itn->second;		
	colObj->setUserPointer( ptr );

	add_reportable_object( colObj );
	
/*
	if ( _debug_physics )
	{
		// Create an OSG representation of the Bullet shape and attach it.
		// This is mainly for debugging.
		osg::Node* debugNode = osgbCollision::osgNodeFromBtCollisionShape( collision );
		itn->second->addChild( debugNode );
		
		// Set debug node state.
		osg::StateSet* state = debugNode->getOrCreateStateSet();
		osg::PolygonMode* pm = new osg::PolygonMode( osg::PolygonMode::FRONT_AND_BACK,
		                                             osg::PolygonMode::LINE );
		state->setAttributeAndModes( pm );
		osg::PolygonOffset* po = new osg::PolygonOffset( -1, -1 );
		state->setAttributeAndModes( po );
		state->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	}
*/
}

btRigidBody*
Scene_Model::process_physics( std::vector< NodeOffsetPair >::iterator& itn,
                              Physics_State& pState, bool substitute_sphere )
{
	boost::regex e( ".*_p([im]?)(r?)(c?)_?$" );
	boost::smatch what;
	
	btRigidBody* body = 0;
	
	std::cout << itn->second->getName();
	if ( boost::regex_match( itn->second->getName(), what, e ) )
	{
		std::cout << " is physics enabled\n";
		std::string const& name = itn->second->getName();
		if ( name[0] == '_' && name[ name.size() - 1 ] == '_' )
			std::cout << "  and invisible" << std::endl;
		else
			std::cout << "  and visible" << std::endl;

		for ( uint32_t i=1; i < what.size(); i++ )
		{
			if ( what[i] == "i" )
				std::cout << "  and immovable" << std::endl;
			else if ( what[i] == "m" )
			{
				pState.movable = true;
				std::cout << "  and movable" << std::endl;
			}
			else if ( what[i] == "r" )
			{
				pState.reportable = true;
				std::cout << "  and reportable" << std::endl;
			}
			else if ( what[i] == "c" )
			{
				pState.convex = false;
				std::cout << "  and not convex" << std::endl;
			}
		}

		osgbDynamics::MotionState* motion = new osgbDynamics::MotionState();
		btCollisionShape* collision;
			
		// Collision objects have to be computed from origin based geometry 
		if ( substitute_sphere )
		{
			osg::BoundingSphere sphere = itn->second->getBound();
			// OSG first computes a bounding box, then bounds that with a
			// sphere. Need to divide by square root of 3 to get back to original
			// radius 
			_camera_mount_radius = sphere.radius() / sqrt( 3. );
			
			collision = new btSphereShape( _camera_mount_radius );
		}
		else
		{
			if ( !pState.convex )
			{
				collision = osgbCollision::btTriMeshCollisionShapeFromOSG( itn->second );
			}
			else
			{
				collision = osgbCollision::btConvexTriMeshCollisionShapeFromOSG( itn->second );
			}
		}
		
		// So we translate the objects back to their original positions
		// after the collision object is created
		itn->second->setMatrix( osg::Matrix::translate( itn->first ) );
		motion->setTransform( itn->second );
		motion->setParentTransform( osg::Matrix::translate( itn->first ) );

		/*
		if ( _debug_physics )
		{
			// Create an OSG representation of the Bullet shape and attach it.
			// This is mainly for debugging.
			osg::Node* debugNode = osgbCollision::osgNodeFromBtCollisionShape( collision );
			itn->second->addChild( debugNode );

			// Set debug node state.
			osg::StateSet* state = debugNode->getOrCreateStateSet();
			osg::PolygonMode* pm = new osg::PolygonMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
			state->setAttributeAndModes( pm );
			osg::PolygonOffset* po = new osg::PolygonOffset( -1, -1 );
			state->setAttributeAndModes( po );
			state->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
		}
		*/

		btScalar mass( 0.0 );
		btVector3 inertia( 0., 0., 0. );
		if ( pState.movable )
		{
			mass = 1.0;
			collision->calculateLocalInertia( mass, inertia );
		}

		const osg::BoundingSphere& bs = itn->second->getBound();

		btRigidBody::btRigidBodyConstructionInfo rbinfo( mass, motion, collision, inertia );
		body = new btRigidBody( rbinfo );

		if ( pState.movable )
		{
			float half_radius = bs.radius() / 8.f;
			body->setCcdMotionThreshold( half_radius );
			body->setCcdSweptSphereRadius( 0.9 * half_radius );
			body->setSleepingThresholds( 0.f, 0.1f );
		}
			
		Physics_State* ps = new Physics_State( pState );
		ps->node = itn->second;
		body->setUserPointer( (void*) ps );
		
		if ( pState.reportable )
		{
			body->setCollisionFlags( body->getCollisionFlags() |
			                         btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK | 
			                         btCollisionObject::CF_NO_CONTACT_RESPONSE );

		}

		int head = 0;
		std::string n = itn->second->getName();
		if ( n[0] == '_' )
			head = 1;
		
		size_t tail;
		if ( name[ name.size() - 1 ] == '_' )
			tail = n.find_last_of( "_p", name.size() - 2 );
		else
			tail = n.find_last_of( "_p", name.size() - 1 );
		itn->second->setName( n.substr(head, tail - head - 1 ) );
	}
	else
	{
		std::cout << " is not physics enabled\n";
		itn->second->setMatrix( osg::Matrix::translate( itn->first ) );
	}

	return body;	
}

void
Scene_Model::process_channel( std::vector< NodeOffsetPair >::iterator& itn )
{
	// In Collada, each MatrixTransform has one Geode node
	osg::Geode* geode = ( osg::Geode* )itn->second->getChild( 0 );
	// which in turn has one piece of geometry from which
	// we yank out the vertex data
	osg::Geometry* geom = geode->getDrawable( 0 )->asGeometry();
	osg::Array* vertices = geom->getVertexArray();
	osg::Array* normals = geom->getNormalArray();
	
	osg::Vec3f up( 0.f, 0.f, 1.f );
	uint32_t j = 0;
	float* data = (float*)( normals->getDataPointer() );
	int first_index = -1;
	int last_index = -1;
	int stride = 1;
	bool done = false;
	
	assert( normals->getNumElements() == vertices->getNumElements() );
	
	// loop through normals looking for those that have a significant Z-component 
	for ( j = 0; !done && j < normals->getNumElements(); j++ )
	{
		osg::Vec3f n( data[3*j],  data[3*j + 1],  data[3*j + 2] );
		if ( fabsf( n * up ) > 0.7 )
		{
			if ( first_index < 0 )
				first_index = j;
			// Once we've found the end of the previous run, we loop unti we find
			// the next one to get the offset/stride length
			else if ( last_index > 0 )
			{
				stride = j - first_index;
				done = true;
			}
			
		}
		else
		{
			if ( first_index > 0 && last_index < 0 )
				last_index = j;
		}
	}

	// Now find the min and max distance between vertices on the floor
	data = (float*)( vertices->getDataPointer() );
	std::vector< osg::Vec3f > pts( vertices->getNumElements() );
	osg::Vec3f max_point( -99999.f, -99999.f, -99999.f );
	
	// First get the bounds
	for ( uint32_t i = 0; i < vertices->getNumElements(); i+=stride )
	{
		for ( int j = first_index; j < last_index; j++ )
		{
			osg::Vec3f v( data[3*(i+j)],  data[3*(i+j) + 1],  data[3*(i+j) + 2] );
			// Need to add the center back in at this point
			pts[ i + j ] = v + itn->first;
			_min_point = osg::Vec3f( std::min( _min_point.x(), pts[i+j].x() ),
			                        std::min( _min_point.y(), pts[i+j].y() ),
			                        std::min( _min_point.z(), pts[i+j].z() ) );
			max_point = osg::Vec3f( std::max( max_point.x(), pts[i+j].x() ),
			                        std::max( max_point.y(), pts[i+j].y() ),
			                        std::max( max_point.z(), pts[i+j].z() ) );
		}
	}

	// Now find the distances
	for ( uint32_t i = 0; i < vertices->getNumElements(); i+=stride )
	{
		for ( int j = first_index; j < last_index-1; j++ )
		{
			float dist = (pts[(i+stride) % vertices->getNumElements() + j] - pts[i+j]).length();
			_min_dist = std::min( _min_dist, dist );
			_max_dist = std::max( _max_dist, dist );
			dist = (pts[i+j+1] - pts[i+j]).length();
			_min_dist = std::min( _min_dist, dist );
			_max_dist = std::max( _max_dist, dist );
		}
	}
	
	_min_dist /= 2.f;
	
	std::cout << "Min Bound = <" << _min_point.x() << ", " << _min_point.y() << ", " << _min_point.z() << "> " << std::endl;
	std::cout << "Max Bound = <" << max_point.x() << ", " << max_point.y() << ", " << max_point.z() << "> " << std::endl;
	std::cout << "Distances = <" << _min_dist << ", " << _max_dist << ">" << std::endl;

	osg::Vec3f deltas = max_point - _min_point;
	deltas /= _min_dist;
	std::cout << "Deltas = <" << deltas.x() << ", " << deltas.y() << "> " << std::endl;

	array_type* grid;
	_tangent_grid = new array_type ( boost::extents[ ( int( ceilf( deltas.x() ) ) ) + 1 ]
	                                              [ ( int( ceilf( deltas.y() ) ) ) + 1 ] );
	
	memset( _tangent_grid->data(), 0, _tangent_grid->num_elements() * sizeof( osg::Vec3f* ) );
	grid = new array_type ( boost::extents[ ( int( ceilf( deltas.x() ) ) ) + 1 ]
	                        [ ( int( ceilf( deltas.y() ) ) ) + 1 ] );
	
	memset( grid->data(), 0, grid->num_elements() * sizeof( osg::Vec3f* ) );
	
	for ( uint32_t i = 0; i < vertices->getNumElements(); i+=stride )
	{
		for ( int j = first_index; j < last_index; j++ )
		{
			osg::Vec3f pstart = pts[i+j];
			osg::Vec3f pnext = pts[(i+stride) % vertices->getNumElements() + j];
			osg::Vec3f plast = pts[(i+2*stride) % vertices->getNumElements() + j];
			
			osg::Vec3f v = pnext - pstart;
			osg::Vec3f vs = pnext - pstart;
			osg::Vec3f vn = plast - pnext;
			float dist = v.normalize();
			vn.normalize();
			
			int count = (int( ceilf( dist / _min_dist ) ) );
			for ( int k = 0; k < count; k++ )
			{
				float alpha = (float)k/(float)count;
				osg::Vec3f delta = vs * alpha;
				osg::Vec3f pt = ( pstart + delta - _min_point ) / _min_dist;
				osg::Vec3f* interp = new osg::Vec3f( v * (1.f - alpha ) + vn * alpha );
				interp->normalize();
				if ( (*grid)[ (int( roundf( pt.x() ) ) ) ][ (int( roundf( pt.y() ) ) ) ] == 0 )
					(*grid)[ (int( roundf( pt.x() ) ) ) ][ (int( roundf( pt.y() ) ) ) ] = interp;
				else
				{
					*((*grid)[ (int( roundf( pt.x() ) ) ) ][ (int( roundf( pt.y() ) ) ) ]) += *interp;
					(*grid)[ (int( roundf( pt.x() ) ) ) ][ (int( roundf( pt.y() ) ) ) ]->normalize();
				}
			}			
		}
	}

double gaussian[9][9] = 
	{  {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
	   {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
	   {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
	   {0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
	   {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
	   {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
	   {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067} 
	};

	// Now fill in holes
	int upper_x = ( int( ceilf( deltas.x() ) ) ) + 1;
	int upper_y = ( int( ceilf( deltas.y() ) ) ) + 1;
	
	for ( int i = 0; i < upper_x; i++ )
	{
		for ( int j = 0; j < upper_y; j++ )
		{
			osg::Vec3f v( 0.f, 0.f, 0.f );
			int count = 0;
			
			for ( int m = -3; m <= 3; m++ )
			{
				if ( i+m > 0 && i+m < upper_x )
				{
					for ( int n = -3; n <= 3; n++ )
					{
						if ( j+n > 0 && j+n < upper_y )
						{
							if ( (*grid)[i+m][j+n] != 0 )
							{
								v += ( *((*grid)[i+m][j+n] ) * gaussian[ 3 + m ][ 3 + n ] );
								count++;
							}
						}
					}
				}
			}
				
			if ( count > 0 )
			{
				v.normalize();
				(*_tangent_grid)[i][j] = new osg::Vec3f( v );
			}
		}
	}
	
	delete grid;
}
void
Scene_Model::process_animation_path( std::vector< NodeOffsetPair >::iterator& itn )
{
	float dist;
					
	// In Collada, each MatrixTransform has one Geode node
	osg::Geode* geode = ( osg::Geode* )itn->second->getChild( 0 );
	// which in turn has one piece of geometry from which
	// we yank out the vertex data
	osg::Array* vertices = geode->getDrawable( 0 )->asGeometry()->getVertexArray();
	float* data = (float*)( vertices->getDataPointer() );
	std::vector< osg::Vec3f > pts( vertices->getNumElements() );
	for ( uint32_t i = 0; i < vertices->getNumElements(); i++ )
	{
		osg::Vec3f v( data[3*i],  data[3*i + 1],  data[3*i + 2] );
		// Need to add the center back in at this point
		pts[ i ] = v + itn->first;
	}

	// Lane-Reisenfeld Subdivision
	for ( int i = 0; i < 3; i++ )
	{
		//  Step 1 - Insert Mid-points
		std::vector< osg::Vec3f > mid_pts( pts.size() * 2 );
		for ( uint32_t j = 0; j < pts.size(); j++ )
		{
			mid_pts[ 2 * j ] = pts[ j ];
			mid_pts[ 2 * j + 1 ] = ( pts[ j ] +  pts[ ( j + 1 ) % pts.size() ] ) / 2.f;
		}
		
		// Step 2 - Replace each vertex with midpoints of the segment
		
		for ( int k = 0; k < 2; k++ )
			for ( uint32_t j = 0; j < mid_pts.size(); j++ )
				mid_pts[ j ] = ( mid_pts[ j ] +  mid_pts[ ( j + 1 ) % mid_pts.size() ] ) / 2.f;
		
		pts = mid_pts;
	}

/*	
	for ( uint32_t i = 1; i < pts.size(); i++ )
		std::cout << pts[i].x() << "," << pts[i].y() << "," << pts[i].z() << " ";
	std::cout << std::endl << std::endl;
*/	
	dist = 0.f;

	for ( uint32_t i = 1; i < pts.size(); i++ )
		dist += ( pts[i] - pts[i-1] ).length();
					
	dist += ( pts[0] - pts[ pts.size() - 1 ] ).length();
				
	int path_index_at_start = 0;
	float dist_to_start = 999999.f;

	for ( uint32_t i = 0; i < pts.size(); i++ )
	{
		float d = ( pts[i] - _start_center.back()[0] ).length();
		if ( d < dist_to_start )
		{
			dist_to_start = d;
			path_index_at_start = i;
		}
	}

	// Remove the default empty path
	_animationPath.back() = new osg::AnimationPath;
	_animationPath.back()->setLoopMode( osg::AnimationPath::LOOP );
	osg::Vec3d cross;
	osg::Vec3d x_axis( 1, 0, 0 );
	osg::Vec3d up( 0, 0, 1 );
	osg::Vec3d v0 = pts[ path_index_at_start % pts.size() ] - 
		              pts[ ( path_index_at_start - 1 + pts.size()) % pts.size() ];
	v0.normalize();
	cross = x_axis ^ v0;
	int sign = ( up * cross ) < 0 ? -1 : 1;
	double cos_theta = v0 * x_axis;
//	double yaw = sign * acosf( cos_theta );
	double time = 0.0f;
	double yaw = 0.0f;
	osg::Vec3f last_pt = pts[path_index_at_start];

	for ( uint32_t i = 0; i <= pts.size(); i++ )
	{
		int offset = i + path_index_at_start;
		osg::Vec3d v1 = pts[ ( offset + 1 ) % pts.size() ] - pts[ offset % pts.size() ];
		v1.normalize();
		osg::Vec3d v0 = pts[ offset % pts.size() ] - pts[ ( offset - 1 + pts.size()) % pts.size() ];
		v0.normalize();
		osg::Vec3d cross = v1 ^ v0;
		int sign = cross.z() < 0 ? -1 : 1;
		double theta = asin( sign * cross.length() );

		yaw += theta;
		if ( yaw > 2.0 * M_PI )
			yaw -= 2.0 * M_PI;
		else if ( yaw < -2.0 * M_PI )
			yaw += 2.0 * M_PI;

//		std::cout << offset % pts.size() << ", " << theta << ", " << yaw << std::endl;

		osg::Quat rotation(osg::Quat((-yaw),osg::Vec3(0.0,0.0,1.0)));
		double time_delta = ( pts[ offset % pts.size() ] - last_pt ).length();
		time += time_delta;

		_animationPath.back()->insert( time, 
		                               osg::AnimationPath::ControlPoint( pts[offset%pts.size()], rotation ) );
		last_pt = pts[ offset % pts.size() ];
	}
}

void
Scene_Model::process_start( std::vector< NodeOffsetPair >::iterator& itn )
{
	// Add the center back in
	itn->second->setMatrix( osg::Matrix::translate( itn->first ) );

	osg::BoundingSphere bbox = itn->second->getBound();
	_start_center.back().push_back( bbox.center() );

	// In Collada, each MatrixTransform has one Geode node
	osg::Geode* geode = ( osg::Geode* )itn->second->getChild( 0 );
	// which in turn has one piece of geometry from which
	// we yank out the vertex data
	osg::Array* vertices = geode->getDrawable( 0 )->asGeometry()->getVertexArray();
	float* data = (float*)( vertices->getDataPointer() );
	std::vector< osg::Vec3f > pts( vertices->getNumElements() );
					
	for ( uint32_t i = 0; i < vertices->getNumElements(); i++ )
	{
		pts[ i ] = osg::Vec3f( data[3*i],  data[3*i + 1],  data[3*i + 2] );
	}

	osg::Vec3f v0 = pts[ 1 ] - pts[ 0 ];
	v0.normalize();
	bool done = false;
					
	// Loop through the points until we find two vectors that are not parallel so
	// we can compute the cross product to find the direction
	for ( uint32_t i = 2; !done && i < pts.size(); i++ )
	{
		osg::Vec3f v1 = pts[ i ] - pts[ 0 ];
		v1.normalize();
		if ( v1 * v0 < 0.99 )
		{
			done = true;
			_start_cross.back().push_back( v1 ^ v0 );
			_start_cross.back().back().normalize();
		}
	}
}

void
Scene_Model::process_crossbar( std::vector< NodeOffsetPair >::iterator& itn )
{
	// In Collada, each MatrixTransform has one Geode node
	osg::Geode* geode = ( osg::Geode* )itn->second->getChild( 0 );
	// which in turn has one piece of geometry from which
	// we yank out the vertex data
	osg::Array* vertices = geode->getDrawable( 0 )->asGeometry()->getVertexArray();
	float* data = (float*)( vertices->getDataPointer() );
	osg::Vec3f min_pt = osg::Vec3f( data[0],  data[1],  data[2] );
	osg::Vec3f max_pt = osg::Vec3f( data[0],  data[1],  data[2] );
	for ( uint32_t i = 0; i < vertices->getNumElements(); i++ )
	{
		osg::Vec3f v = osg::Vec3f( data[3*i],  data[3*i + 1],  data[3*i + 2] );
		min_pt = osg::Vec3f( std::min( min_pt[0], v[0] ),
		                     std::min( min_pt[1], v[1] ),
		                     std::min( min_pt[2], v[2] ) );
		max_pt = osg::Vec3f( std::max( max_pt[0], v[0] ),
		                     std::max( max_pt[1], v[1] ),
		                     std::max( max_pt[2], v[2] ) );
	}
	osg::Vec3f dist = max_pt - min_pt;
	_dynamics_world.back()->crossbar_width = std::max( std::max( dist.x(), dist.y() ), dist.z() ) / 2.f;
	_dynamics_world.back()->has_crossbar = true;
}


void
Scene_Model::setup_tracking( int index )
{
	osg::Geode* g = new osg::Geode;
	float box_width = std::min( 0.001f * scene_bound()->radius(), 0.02f );
	g->addDrawable(new osg::ShapeDrawable(new osg::Box( osg::Vec3f( 0.f, 0.f, 0.f ), box_width )));
	_homeOffset.back() = new osg::MatrixTransform;
	_homeOffset.back()->setDataVariance(osg::Object::STATIC);
	_homeOffset.back()->addChild( g );
	move_offset( 0, 0, 0 );

	osg::StateSet* state = _homeOffset.back()->getOrCreateStateSet();
	osg::PolygonMode* pm = new osg::PolygonMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
	state->setAttributeAndModes( pm );
	osg::PolygonOffset* po = new osg::PolygonOffset( -1, -1 );
	state->setAttributeAndModes( po );
	state->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

	_positioned.back() = new osg::MatrixTransform;
	_positioned.back()->setDataVariance(osg::Object::STATIC);
	_positioned.back()->addChild( _homeOffset.back() );

	_xformer.back() = new osg::MatrixTransform;
	_xformer.back()->addChild( _positioned.back() );
	_xformer.back()->setNodeMask( INVISIBLE_MASK );
	_invisible_objects.push_back( _xformer.back() );
	
	current_scene()->addChild( _xformer.back() );
	_tracker.back() = set_track_node( _positioned.back() );
	_tracker.back()->setDistance( 0.04 ); // For the default box_width, this
															 // makes the eye distance be 0.01.
	_tracker.back()->setHomePosition( _start_center.back()[ index ], 
	                                  _start_center.back()[ index ] + _start_cross.back()[ index ], 
	                                  osg::Vec3f( 0, 0, 1 ) );

	_tracking_initialized = true;
}

void 
Scene_Model::move_offset( float x, float y, float z )
{
	osg::Matrixd mat;
	
	mat.makeTranslate( x, y, z );
	_homeOffset[ _active_model ]->setMatrix( mat );
}

btRigidBody* 
Scene_Model::create_bullet_box( osg::MatrixTransform * box,
                                osg::Vec3 center )
{
	btCollisionShape* collision = osgbCollision::btBoxCollisionShapeFromOSG( box );

	/*
	if ( _debug_physics )
	{
		osg::Node* debugNode = osgbCollision::osgNodeFromBtCollisionShape( collision );
		box->addChild( debugNode );

		// Set debug node state.
		osg::StateSet* state = debugNode->getOrCreateStateSet();
		osg::PolygonMode* pm = new osg::PolygonMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
		state->setAttributeAndModes( pm );
		osg::PolygonOffset* po = new osg::PolygonOffset( -1, -1 );
		state->setAttributeAndModes( po );
		state->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	}
	*/
	
	osgbDynamics::MotionState * motion = new osgbDynamics::MotionState();
	motion->setTransform( box );
//	motion->setParentTransform( osg::Matrix::translate( center ) );

	btScalar mass( 0.0 );
	btVector3 inertia( 0, 0, 0 );
//	collision->calculateLocalInertia( mass, inertia );
	btRigidBody::btRigidBodyConstructionInfo rb( mass, motion, collision, inertia );
	btRigidBody* body = new btRigidBody( rb );

	return( body );
}

void
Scene_Model::move_to_new_start()
{
	// Update the camera transforms to match the start location
	osg::Vec3d v =  _start_nodes[ _active_model ][ _start_id ].first;
	btVector3 bv( v.x(), v.y(), v.z() );
	_camera_body[ _active_model ]->getWorldTransform().setOrigin( bv );
	_camera_xform[ _active_model ] = new btTransform( _camera_body[ _active_model ]->getWorldTransform() );
}

