/* -*-c++-*- */

#ifdef _WINDOWS
// Needed to pickup M_PI and M_PI_2 on Windows because it's too stupid
// to actually let you use them when you include where they're defined.
#define _USE_MATH_DEFINES
#endif

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

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

// Floating-point modulo
// The result (the remainder) has same sign as the divisor.
// Similar to matlab's mod(); Not similar to fmod() -   Mod(-3,4)= 1   fmod(-3,4)= -3
template<typename T>
T Mod(T x, T y);

// wrap [rad] angle to [-PI..PI)
double WrapPosNegPI(double fAng);
// wrap [rad] angle to [0..TWO_PI)
double WrapTwoPI(double fAng);
// wrap [deg] angle to [-180..180)
double WrapPosNeg180(double fAng);
// wrap [deg] angle to [0..360)
double Wrap360(double fAng);
double unwrap( double prev, double cur );


#endif
