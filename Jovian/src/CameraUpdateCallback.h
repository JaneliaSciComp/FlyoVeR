
#ifndef CAMERAUPDATECALLBACK_H
#define CAMERAUPDATECALLBACK_H

#include "CrudeObstacleVisitor.h"
#include "PunishmentRegionVisitor.h"

#include <boost/circular_buffer.hpp>
#include <boost/lexical_cast.hpp>

#include <osg/Matrix>
#include <osg/Vec4>
#include <osg/MatrixTransform>
#include <osg/NodeCallback>
#include <osgGA/CameraManipulator>
#include <osgGA/NodeTrackerManipulator>

#include <QElapsedTimer>

#include <Globals.h>
#include <Communicator.h>

#include <iostream>
#include <cmath>

class Motion_Data: public osg::Referenced
{
public:
	osg::Vec3d motion;
	osg::Vec4f raw;
	float roll, pitch, yaw;
	float interval;
	float minObstacleDistance;
	// float minPunishmentDistance;
	int reinforcementValue;

	Motion_Data() 
		: Referenced()
		, motion( 0.f, 0.f, 0.f )
		, raw( 0.f, 0.f, 0.f, 0.f )
		, roll( 0.f )
		, pitch( 0.f )
		, yaw( 0.f )
		, interval( 1./60.f ) 
		, minObstacleDistance(0)
		// , minPunishmentDistance(0)
		, reinforcementValue(0)
	{}
};


class CameraUpdateCallback : public osg::NodeCallback
{
public:
	CameraUpdateCallback( osgGA::CameraManipulator* ma ):
		_x_axis( 1, 0, 0 ), _y_axis( 0, 1, 0 ), _z_axis( 0, 0, 1 ),
		_displacement( 0.f, 0.f, 0.f ), _offset( 0.f, 0.f, 0.f ),
		verbose( false ), ntm( 0 ), _disableMotion( false )
	{
		tm = ma;
	}

	CameraUpdateCallback( osgGA::NodeTrackerManipulator* ma ):
		_x_axis( 1, 0, 0 ), _y_axis( 0, 1, 0 ), _z_axis( 0, 0, 1 ),
		_displacement( 0.f, 0.f, 0.f ), _offset( 0.f, 0.f, 0.f ),
		verbose( false ), tm( 0 ), _disableMotion( false )
	{
		ntm = ma;
	}

	virtual void updateScaleFactors( float xs, float ys, float zs )
	{
	}

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		traverse(node,nv);

		osg::Matrix mat = tm->getMatrix();
		osg::Matrix mat2;
		mat2.makeRotate( 0.002, _y_axis );

		tm->setByMatrix( mat2 * mat );
	}

	void set_verbose( bool state ) { verbose = state; }
	bool motion_state( ) const { return _disableMotion; }
	osg::Vec3d const& displacement() const { return _displacement; }
	void clear() { _displacement.set( 0.f, 0.f, 0.f ); }
	void set_tracking_vector( osg::Vec3d const& v ) { _offset = v; }

	void toggleMotion( )
	{
		_disableMotion = !_disableMotion;
		clear();
		if ( _disableMotion )
			_offset.set( 0.f, 0.f, 0.f );
		if( ntm )
		{
			osg::Vec3d* vec = (osg::Vec3d*)ntm->getUserData();
			vec->set( 0.f, 0.f, 0.f );
		}
	}

protected:
	osgGA::CameraManipulator* tm;
	osgGA::NodeTrackerManipulator* ntm;
	osg::Vec3d _x_axis, _y_axis, _z_axis;
	// displacement is the length of the motion vector since motion was disabled
	// offset is used in the case of researcher controlled movement
	osg::Vec3d _displacement, _offset;
	bool verbose;
	bool _disableMotion;
};


class RemoteCameraUpdateCallback : public CameraUpdateCallback
{
public:
	RemoteCameraUpdateCallback( osgGA::CameraManipulator* ma, Communicator* c, float xs, float ys, float zs, float rad, int numSamples, bool for_path = false )
		:  CameraUpdateCallback( ma ), comm( c ), _directionBuffer( numSamples ), _accumulatedVector( 0.f, 0.f, 0.f ), _Omega(0.f), _for_path( for_path )
		, _yaw_only(false), _stripe_distance(100.0)
		, _avoid_physics(true)
		, _crudeObstacleVisitor(NULL)
		, _punishmentRegionVisitor(NULL)
		, respawn_radius(1.0), respawn_timeout(15.0)
   {
	  _x_scale = xs;
	  _y_scale = ys;
	  _z_scale = zs;
	  _radius = rad;
   }

	RemoteCameraUpdateCallback( osgGA::NodeTrackerManipulator* ma, Communicator* c, float xs, float ys, float zs, float rad, int numSamples
			, bool for_path = false
			, CrudeObstacleVisitor * cov = NULL
			, PunishmentRegionVisitor * prv = NULL
			, osg::Vec3 respawn = osg::Vec3(0,0,0) )
		:  CameraUpdateCallback( ma ), comm( c ), _directionBuffer( numSamples ), _accumulatedVector( 0.f, 0.f, 0.f ), _Omega(0.f), _for_path( for_path )
		, _yaw_only(false), _stripe_distance(100.0)
		, _avoid_physics(true)
		, _crudeObstacleVisitor(cov)
		, _punishmentRegionVisitor(prv)
		, respawnLocation(respawn)
		, respawn_radius(1.0), respawn_timeout(15.0)
   {
	  _x_scale = xs;
	  _y_scale = ys;
	  _z_scale = zs;
	  _radius = rad;
	  ntm->setDistance( 0.04f );
   }

   ~RemoteCameraUpdateCallback()
   {
      comm->close();
//	  std::cout << _Vfwd << ", " << _Vss << ", " << _Omega << std::endl;
   }

   void setCrudeObstacleVisitor(CrudeObstacleVisitor * cov) {
	   _crudeObstacleVisitor = cov;
   }

   void setPunishmentRegionVisitor(PunishmentRegionVisitor * prv) {
	   _punishmentRegionVisitor = prv;
   }

   virtual void setYawOnly( bool doYawOnly ) {
	   _yaw_only = doYawOnly;
   }

   virtual void setStripeDistance( float stripe_distance ) {
	   _stripe_distance = stripe_distance;
   }

   virtual void updateScaleFactors( float xs, float ys, float zs )
   {
	  _x_scale = xs;
	  _y_scale = ys;
	  _z_scale = zs;
   }

	void averageDirection( osg::Vec3d dir )
	{
		if ( _directionBuffer.full() )
		{
			// Remove first element from the accumulated value
			_accumulatedVector -= _directionBuffer.front();
			_directionBuffer.push_back( dir );
			_accumulatedVector += dir;
		}
		else
		{
			// Filling the buffer
			_directionBuffer.push_back( dir );
			_accumulatedVector += dir;
		}
	}

   virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
   {
	   
	   // return; // testing short circuit temporarily
	   comm->read();
	   if ( !_disableMotion )
	   {
		   if ( comm->message_length() > 0 )
		   {
			   try
			   {
				   float Vfwd, Vss, Omega, interval, r1, r2, r3, r4;
				   const char * dbd = comm->data();
				   sscanf( dbd, "%f,%f,%f,%f,%f,%f,%f,%f",
				           &Vfwd, &Vss, &Omega, &r1, &r2, &r3, &r4, &interval );

				   /* For debugging only
				   std::cerr << Vfwd;
				   std::cerr << ", " << Vss;
				   std::cerr << ", " << Omega;
				   std::cerr << ", " << r1;
				   std::cerr << ", " << r2;
				   std::cerr << ", " << r3;
				   std::cerr << ", " << r4;
				   std::cerr << ", " << interval;
				   std::cerr << std::endl;
				   /* */


//				  Vfwd = -1.41421; Vss = 0.f;
				   // Convert to scaled angles
				   float Vfwdf = ( Vfwd / _x_scale ) * M_PI ;
				   float Omegaf = ( Omega / _y_scale ) * M_PI;
				   float Vssf = ( Vss / _z_scale ) * M_PI;

				   // Filter angles, in case of stripe fixation assay.
				   if (_yaw_only) {
					   // TODO - apply side slip to yaw
					   // float dOmega = Vss * ???; // TODO

					   // We approximate the roll/side-slip effect as a change in Yaw angle
					   float sideSlipDistance = Vssf * _radius; // in millimeters
					   float ssDeltaYaw = std::atan(sideSlipDistance / _stripe_distance); // in radians
					   // Effect on yaw, depends on stripe heading; e.g. roll has opposite effect upon stripe to rear, as stripe to front

					   // TODO - gain should be cos ABSOLUTE yaw angle. (Omegaf is relative)
					   osg::Matrix mat = ntm->getMatrix();
					   float deltaYawGain = -mat(2, 0); // Emprically determined to maybe be cos(total Yaw angle relative to stripe at +X)

					   ssDeltaYaw *= deltaYawGain;
					   Omegaf -= ssDeltaYaw;

					   Vfwd = 0;
					   Vss = 0;
					   Vfwdf = 0;
					   Vssf = 0;
				   }

				   _Omega = Omegaf;

				   if ( _avoid_physics ) {
					   // No-physics approach, April 2, 2015 CMB
					   float minObstacleDistance = 100;
					   int reinforcementValue = 0;
					   // float minPunishmentDistance = 100;

					    // Convert Local Fly Treadball yaw -> Absolute World Yaw
						const osg::Quat& oldRotation = ntm->getRotation(); // expect yaw only
						osg::Quat rotationChange(Omegaf, _y_axis); // local yaw change
						osg::Quat newRotation = rotationChange * oldRotation; // combined
						double absoluteYaw, x, y, z; // receiver variables for getRotate() method
						newRotation.getRotate(absoluteYaw, x, y, z);

						// TODO - there is something wrong here
						// newRotation = osg::Quat( absoluteYaw, _y_axis ); // enforce yaw-only rotation
                        ntm->setRotation( newRotation );

						/* */
						// Treadball roll/pitch -> World dx/dy
						// Convert treadball pitch/roll rotation directly into arena X/Y motion
						osg::Vec3f motionVector(
							    Vssf * _radius, // left/right motion
								0.0, // no up/down motion
								Vfwdf * _radius // forward/back motion
							);
						// Rotate motion vector by Omega, to convert from fly frame to world frame
						motionVector = newRotation * motionVector;

						/* */
						// Apply translational motion directly to the node this NodeTrackerManipulator is tracking
						// Hierarchy (parent-child):
						//   NodeTrackerManipulator (holds home position, rotation)
						//     Player Node (no transform?)
						//       Home Offset (translation from home position)
						osg::Node * trackedNode = ntm->getTrackNode();
						osg::MatrixTransform * trackedTransform = dynamic_cast<osg::MatrixTransform *>(trackedNode);
						if (trackedTransform != NULL) {

							// Try to find the homeOffset child of the player node
							osg::Group * playerGroup = trackedNode->asGroup();
							if (playerGroup != NULL) {
								for ( unsigned int c = 0; c < playerGroup->getNumChildren(); ++c ) {
									osg::Node * child = playerGroup->getChild(c);
									osg::MatrixTransform * homeOffsetXform = dynamic_cast<osg::MatrixTransform *>(child);
									if (homeOffsetXform != NULL) {
										trackedTransform = homeOffsetXform;
										trackedNode = child;
										break;
									}
								}
							}

							// Compute new location in world coordinates
							osg::Vec3 worldLocation = trackedNode->getWorldMatrices().back().getTrans();

							// Use CrudeObstacleVisitor to place camera outside of any obstacles.
							if (_crudeObstacleVisitor != NULL) {
								osg::Vec3 currentLocation = worldLocation;
								osg::Vec3 p1 = worldLocation + motionVector; // proposed absolute new location, in world coordinates
								osg::Vec2 preCollided2(p1[0], p1[1]);
								CollisionResult collision = _crudeObstacleVisitor->collide(preCollided2); // absolute XY location after collision
								minObstacleDistance = collision.minDistance; // used to report collision status in log

								osg::Vec2& collided2 = collision.decollidedXY;
								osg::Vec2 collisionDisplacement = collided2 - preCollided2;
								osg::Vec3 collided3(collided2.x(), collided2.y(), p1.z()); // absolute XYZ after collision
								motionVector = collided3 - worldLocation; // adjusted relative motion
								worldLocation = collided3;

								// Respawn to start if animal loiters by an obstacle
								if (minObstacleDistance < respawn_radius) {
									// in case the timer is not running yet, definitely start it running now
									if (! _loiter_timer.isValid()) 
										_loiter_timer.start();
									float elapsed_seconds = _loiter_timer.elapsed() / 1000.0;
									// std::cout << "Elapsed loiter seconds = " << elapsed_seconds << std::endl;
									// std::cout << "Respawn timeout = " << respawn_timeout << std::endl;
									if (elapsed_seconds > respawn_timeout) {
										// TODO: now we teleport the animal
										// std::cout << "Respawn!!" << std::endl;
										// _viewer->home();
										// _model->start

										// set location to origin?
										motionVector = respawnLocation - currentLocation;
										worldLocation.set(respawnLocation);

                                        // set random respawn orientation                                    
                                        float randomDirection = ((rand() % 180) + 1) * 2 * M_PI/180;
                                        const osg::Quat& oldRotation = ntm->getRotation(); // expect yaw only
                                        osg::Quat randomRespawnRotation(randomDirection, _y_axis); // local yaw change
                                        osg::Quat newRotation = randomRespawnRotation * oldRotation; // combined
                                        ntm->setRotation(newRotation);

										_loiter_timer.start();
									}
								}
								else {
									// restart the timer every single time we find ourselves outside of the loitering radius
									_loiter_timer.start();
								}

								// std::cout << "Collision min distance = " << minObstacleDistance << std::endl;
							}

							// Is the animal in a reinforcement zone?
							if (_punishmentRegionVisitor != NULL) {                                
								reinforcementValue = _punishmentRegionVisitor->reinforcementAtXy(worldLocation[0], worldLocation[1]);
							}

							// Move player to new location
							osg::Matrix translationXy;
							translationXy.makeTranslate(osg::Vec3( motionVector ));
							trackedTransform->preMult( translationXy );

							// std::cerr << "tracked: " << worldLocation.x();
							// std::cerr << ", " << worldLocation.y();
							// std::cerr << ", " << worldLocation.z();
							// std::cerr << std::endl;
						}

						/* */
						// Populate Motion_Data, so log file could get the values
						// TODO - need to follow old log file convention
						Motion_Data* md = (Motion_Data*)ntm->getUserData();
						// TODO why -x?
						md->motion.set( -motionVector.x(), motionVector.y(), motionVector.z() );
						md->raw.set( r1, r2, r3, r4 );

						md->roll = Vssf;
						md->pitch = Vfwdf;
						md->yaw += Omegaf;
						md->minObstacleDistance = minObstacleDistance;
						// md->minPunishmentDistance = minPunishmentDistance;
						md->reinforcementValue = reinforcementValue;
						/* */

				   }

				   else {
					   float c1, c2, c3, s1, s2, s3;
					   // These x/2.f values are a result of the peculiarities of quaternion arithmetic.
					   // i.e. Vssf, Vfwdf, Omegaf are, in fact, in units of radians. CMB
					   c1 = cosf(Vssf / 2.f);
					   s1 = sinf(Vssf / 2.f);
					   c2 = cosf(Vfwdf / 2.f);
					   s2 = sinf(Vfwdf / 2.f);
					   c3 = cosf(Omegaf / 2.f);
					   s3 = sinf(Omegaf / 2.f);

					   osg::Quat q( s1 * c2 * c3 - c1 * s2 * s3,
									c1 * s2 * c3 + s1 * c2 * s3,
									c1 * c2 * s3 - s1 * s2 * c3,
									c1 * c2 * c3 + s1 * s2 * s3 );
					   osg::Vec3f z_axis = osg::Vec3f( 0, 0, 1 );
					   osg::Vec3f v, v1 = q * z_axis;
					   // This appears to project the motion vector into the XY plane (floor plane?) CMB
					   v1.z() = 0.f;
					   v1.normalize();
					   double ang;
					   osg::Vec3f axis;
					   q.getRotate( ang, axis );
					   // Hmm. The full quaternion angle seems to be applied to just the XY projection... CMB
					   // TODO So wouldn't a nearly pure yaw motion result is a weirdly large translation? 
					   v = v1 * ang * _radius;

					   /* For debugging only
					   std::cerr << v1.x();
					   std::cerr << ", " << v1.y();
					   std::cerr << ", " << v1.z();
					   std::cerr << ", " << ang;
					   std::cerr << ", " << _radius;
					   std::cerr << std::endl;
					   /* */

					  //std::cout << std::endl;
					  //std::cout << Vfwd << ", " << Vss << ", " << Omega << ", " << interval << std::endl;
					  //std::cout << Vfwdf << ", " << Vssf << ", " << Omegaf <<  std::endl;
					  // std::cout << ang << ", " << axis << ", " << v << std::endl;

					   Motion_Data* md = (Motion_Data*)ntm->getUserData();
					   md->motion.set( -v.x(), v.y(), v.z() );
					   md->raw.set( r1, r2, r3, r4 );

					   md->roll = Vss;
					   md->pitch = Vfwd;

					   if ( _for_path )
						   md->yaw = _Omega;
					   else
					   {
						   md->yaw += _Omega;

						   osg::Matrix mat;
						   mat.makeRotate( _Omega, _y_axis );
						   const osg::Quat& rot = ntm->getRotation();
						   osg::Quat r;
						   r.set( mat );
						   ntm->setRotation( r * rot );
					   }
					   md->interval = interval;
					   _displacement.set( -v.x(), v.y(), v.z() );
				   }
			   }

			   catch (std::exception&)
			   {
				   std::cout << "Caught exception\n";
				   // Reset the connection
				   comm->close();
				   comm->reset();
			   }
		   }
	   }
	   else
	   {
		   float Vfwd, Vss, Omega;
		   sscanf( comm->data(), "%f,%f,%f", &Vfwd, &Vss, &Omega );
		   float Vfwdf = ( ( Vfwd / _x_scale ) * M_PI ) * _radius;
		   float Vssf = ( ( Vss / _z_scale ) * M_PI ) * _radius;

		   osg::Vec3d v = osg::Vec3d( -Vfwdf, Vssf, 0.f ) ;
		   _displacement += v;
		   Motion_Data* md = (Motion_Data*)ntm->getUserData();
		   md->motion = _offset;
	   }

	   // traverse at the end so then the camera update can run
	   traverse( node, nv );
   }

private:
   Communicator* comm;
	boost::circular_buffer< osg::Vec3d > _directionBuffer;
   float _x_scale, _y_scale, _z_scale;
   float _radius;
	osg::Vec3d _accumulatedVector;
	float _Omega;
	bool _for_path;
	bool _yaw_only;
	float _stripe_distance;
	bool _avoid_physics;
	CrudeObstacleVisitor * _crudeObstacleVisitor;
	PunishmentRegionVisitor * _punishmentRegionVisitor;
	// osgViewer::Viewer * _viewer;
	QElapsedTimer _loiter_timer;
	
public:
	float respawn_radius; // millimeters
	float respawn_timeout; // seconds
	osg::Vec3 respawnLocation;
};

#endif
