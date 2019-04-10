
#ifndef CAMERAUPDATECALLBACK_H
#define CAMERAUPDATECALLBACK_H

#include <boost/circular_buffer.hpp>
#include <boost/lexical_cast.hpp>

#include <osg/Matrix>
#include <osg/NodeCallback>
#include <osgGA/CameraManipulator>
#include <osgGA/NodeTrackerManipulator>

#include <osg/io_utils>

#include <Globals.h>
#include <Communicator.h>

class Motion_Data: public osg::Referenced
{
public:
	osg::Vec3d motion;
	osg::Vec4f raw;
	float roll, pitch, yaw;
	float interval;

	Motion_Data(): Referenced(), motion( 0.f, 0.f, 0.f ), raw( 0.f, 0.f, 0.f, 0.f ), roll( 0.f ), pitch( 0.f ), yaw( 0.f ), interval( 1./60.f ) {}
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
	RemoteCameraUpdateCallback( osgGA::CameraManipulator* ma, Communicator* c, float xs, float ys, float zs, float rad, int numSamples, bool for_path = false ):  CameraUpdateCallback( ma ), comm( c ), _directionBuffer( numSamples ), _accumulatedVector( 0.f, 0.f, 0.f ), _Omega(0.f), _for_path( for_path )
   {
	  _x_scale = xs;
	  _y_scale = ys;
	  _z_scale = zs;
	  _radius = rad;
   }
   
	RemoteCameraUpdateCallback( osgGA::NodeTrackerManipulator* ma, Communicator* c, float xs, float ys, float zs, float rad, int numSamples, bool for_path = false ):  CameraUpdateCallback( ma ), comm( c ), _directionBuffer( numSamples ), _accumulatedVector( 0.f, 0.f, 0.f ), _Omega(0.f), _for_path( for_path )
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
	   comm->read();

	   if ( !_disableMotion )
	   {
		   if ( comm->message_length() > 0 )
		   {
			   try
			   {
				   float Vfwd, Vss, Omega, interval, r1, r2, r3, r4;
				   sscanf( comm->data(), "%f,%f,%f,%f,%f,%f,%f,%f", 
				           &Vfwd, &Vss, &Omega, &r1, &r2, &r3, &r4, &interval );
//				  Vfwd = -1.41421; Vss = 0.f;
				   // Convert to scaled angles
				   float Vfwdf = ( Vfwd / _x_scale ) * M_PI ;
				   float Omegaf = ( Omega / _y_scale ) * M_PI;
				   float Vssf = ( Vss / _z_scale ) * M_PI;
				  
				   _Omega = Omegaf;

				   float c1, c2, c3, s1, s2, s3;
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
				   v1.z() = 0.f;
				   v1.normalize();
				   double ang;
				   osg::Vec3f axis;
				   q.getRotate( ang, axis );
				   v = v1 * ang * _radius;

//			      std::cout << std::endl;
//			      std::cout << Vfwd << ", " << Vss << ", " << Omega << ", " << interval << std::endl;
//			      std::cout << Vfwdf << ", " << Vssf << ", " << Omegaf <<  std::endl;
//			      std::cout << ang << ", " << axis << ", " << v << std::endl;

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

			   catch (std::exception& e)
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
};

#endif
