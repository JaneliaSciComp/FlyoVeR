/* -*-c++-*- */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <istream>
#include <ostream>
#include <sstream>

#ifdef _WINDOWS
// Needed to pickup M_PI and M_PI_2 on Windows because it's too stupid
// to actually let you use them when you include where they're defined.
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include <btBulletDynamicsCommon.h>

#include <osgbCollision/GLDebugDrawer.h>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifdef _WINDOWS
inline float roundf(float x) { return floorf(x + 0.5f); }
#endif

extern int INVISIBLE_MASK;

typedef std::vector< std::vector< std::pair< float, float > > > GraphPositions;

template <class T1, class T2, class T3> struct triple
{
	typedef T1 first_type;
	typedef T2 second_type;
	typedef T3 third_type;

	T1 first;
	T2 second;
	T3 third;
	triple() : first( T1() ), second( T2() ), third( T3() ) {}
	triple( const T1& x, const T2& y, const T3& z ): first( x ), second( y ), third( z ) {}
	template <class U, class V, class W>
	triple( const triple< U, V, W > &p ): first( p.first ), second( p.second ), third( p.third ) {}
};

struct Collision_World
{
	btDiscreteDynamicsWorld* world;
	btDiscreteDynamicsWorld* concave;
	osgbCollision::GLDebugDrawer* debug_drawer;
	bool has_crossbar;
	float crossbar_width;

	Collision_World(): world( 0 ), concave( 0 ), debug_drawer( 0 ),
	                   has_crossbar( false ), crossbar_width( 0.f ) {}
};

template <class T>
bool from_string(T& t,
                 const std::string& s,
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

enum GeometryType { Flat, Cylindrical };

struct ScreenGeometry {
    // Initialization
    ScreenGeometry() :
        screen_width(10.0), projector_luminance(1)
    {}


    void copy(ScreenGeometry const& sg)
    {
        screen_width = sg.screen_width;
        projector_luminance = sg.projector_luminance;
    }

    double screen_width; // used for brightness distortion
    double projector_luminance; // values from 0 to 1
};

struct Frustum
{
	// Initialization
	Frustum():
		left( -1.0 ), right( 1.0 ), bottom( -1.0 ), top( 1.0 ), near_( 1.0 ), far_( 10000.0 ),
		aperture( 1.57079 ), distance( 0 ), screen_distance( 0 ), eye_sep( 0 ), shift_left( true ),
		use_matrix( true )
	{}

	Frustum( double l, double r, double b, double t, double n, double f ):
		left( l ), right( r ), bottom( b ), top( t ), near_( n ), far_( f ),
		aperture( 1.57079 ), distance( 0 ), screen_distance( 0 ), eye_sep( 0 ), shift_left( true ),
		use_matrix( true )
	{}

	Frustum( osg::Matrixd m ): mat( m ), use_matrix( true ) {}

	void from_offset( double width, double height,
	                  double center, double frac_height,
	                  double n, double f )
	{
		left = center - width;
		right = center;
		bottom = frac_height * height;
		top = (1. - frac_height ) * height;
		near_ = n;
		far_ = f;
	}

	void copy ( Frustum const& f )
	{
		left = f.left;
		right = f.right;
		bottom = f.bottom;
		top = f.top;
		near_ = f.near_;
		far_ = f.far_;
		mat = f.mat;
		aperture = f.aperture;
		distance = f.distance;
		screen_distance = f.screen_distance;
		eye_sep = f.eye_sep;
		shift_left = f.shift_left;
		use_matrix = f.use_matrix;
	}

	// Access
	double left, right, bottom, top, near_, far_, aperture, distance, screen_distance, eye_sep;
	osg::Matrixd mat;
	bool shift_left, use_matrix;
};

//////////////////////////////////////////////////////////////////////////
// Matrixd steaming operators.
inline std::ostream& operator<< (std::ostream& os, const osg::Matrixd& m )
{
    for(int row=0; row<4; ++row) {
        for(int col=0; col<4; ++col)
            os << m(row,col) << ",";
    }
    return os;
}

inline std::istream& operator>>(std::istream& os, osg::Matrixd& m )
{
	char ch;

    for(int row=0; row<4; ++row) {
        for(int col=0; col<4; ++col)
            os >> m(row,col) >> ch;
    }
    return os;
}

inline std::ostream& operator<<( std::ostream& ofs, const Frustum& f )
{
	ofs << f.left << "," << f.right << "," << f.bottom << "," << f.top << "," << f.near_ << "," << f.far_;
	ofs << "," << f.aperture << "," << f.distance << "," << f.screen_distance << "," << f.eye_sep;
	ofs << "," << f.mat << "," << f.shift_left << "," << f.use_matrix;
	return ofs;
}

inline std::istream& operator>>( std::istream& ifs, Frustum& f )
{
	char ch;
	ifs >> f.left >> ch >> f.right >> ch >> f.bottom >> ch >> f.top >> ch >> f.near_ >> ch >> f.far_;
	ifs >> ch >> f.aperture >> ch >> f.distance >> ch >> f.screen_distance >> ch >> f.eye_sep;
	ifs >> ch >> f.mat >> ch >> f.shift_left >> ch >> f.use_matrix;
	return ifs;
}

#endif
