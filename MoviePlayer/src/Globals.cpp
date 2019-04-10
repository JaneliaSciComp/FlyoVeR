/* -*-c++-*- */

#include <sstream>
#include <cmath>

#include <Globals.h>

const double     _PI= 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348;
const double _TWO_PI= 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696;

// Floating-point modulo
// The result (the remainder) has same sign as the divisor.
// Similar to matlab's mod(); Not similar to fmod() -   Mod(-3,4)= 1   fmod(-3,4)= -3
template<typename T>
T Mod(T x, T y)
{
    if (0. == y)
        return x;

    double m= x - y * floor(x/y);

    // handle boundary cases resulted from floating-point cut off:

    if (y > 0)              // modulo range: [0..y)
    {
        if (m>=y)           // Mod(-1e-16             , 360.    ): m= 360.
            return 0;

        if (m<0 )
        {
            if (y+m == y)
                return 0  ; // just in case...
            else
                return y+m; // Mod(106.81415022205296 , _TWO_PI ): m= -1.421e-14 
        }
    }
    else                    // modulo range: (y..0]
    {
        if (m<=y)           // Mod(1e-16              , -360.   ): m= -360.
            return 0;

        if (m>0 )
        {
            if (y+m == y)
                return 0  ; // just in case...
            else
                return y+m; // Mod(-106.81415022205296, -_TWO_PI): m= 1.421e-14 
        }
    }

    return m;
}

// wrap [rad] angle to [-PI..PI)
double WrapPosNegPI(double fAng)
{
    return Mod(fAng + _PI, _TWO_PI) - _PI;
}

// wrap [rad] angle to [0..TWO_PI)
double WrapTwoPI(double fAng)
{
    return Mod(fAng, _TWO_PI);
}

// wrap [deg] angle to [-180..180)
double WrapPosNeg180(double fAng)
{
    return Mod(fAng + 180., 360.) - 180.;
}

// wrap [deg] angle to [0..360)
double Wrap360(double fAng)
{
    return Mod(fAng ,360.);
}

double unwrap( double prev, double cur )
{
	double result = cur;
	while ( fabs( result - prev ) > M_PI )
	{
		if ( result - prev > 0 )
			result -= _TWO_PI;
		else
			result += _TWO_PI;
	}
	
	return result;
}

