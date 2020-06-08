#include "math.hpp"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>

namespace math {
		const bool
		logicXor(const bool a, const bool b) {
			return a != b;
		}
		
		const sgl::real
		abs(const sgl::real a) {
			if( a > 0.0f ) return a;
			else return -a;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		maximum(const sgl::real a, const sgl::real b) {
			if( a > b ) return a;
			else return b;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		minimum(const sgl::real a, const sgl::real b) {
			if( a < b ) return a;
			else return b;
		}
		
		// ----------------------------------------------------------------------------------------------------

		const sgl::real
		floor(const sgl::real a) {
			return std::floor(a);
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		ceil(const sgl::real a) {
			return std::ceil(a);
		}
		
		// ----------------------------------------------------------------------------------------------------

		const bool
		equal(const sgl::real a, const sgl::real b) {
			return equal5(a,b);
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const bool
		equal0(const sgl::real a, const sgl::real b) {
			return abs(a-b) < 1.0f;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const bool
		equal1(const sgl::real a, const sgl::real b) {
			return abs(a-b) < 0.1f;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const bool
		equal2(const sgl::real a, const sgl::real b) {
			return abs(a-b) < 0.01f;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const bool
		equal3(const sgl::real a, const sgl::real b) {
			return abs(a-b) < 0.001f;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const bool
		equal4(const sgl::real a, const sgl::real b) {
			return abs(a-b) < 0.0001f;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const bool
		equal5(const sgl::real a, const sgl::real b) {
			return abs(a-b) < 0.00001f;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const bool
		equal6(const sgl::real a, const sgl::real b) {
			return abs(a-b) < 0.000001f;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const bool
		equal7(const sgl::real a, const sgl::real b) {
			return abs(a-b) < 0.0000001f;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		clampZero(const sgl::real a) {
			if( 0.005f > abs(a) ) {
				return 0.0f;
			}
			else {
				return a;
			}
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		limitRange(const sgl::real min, const sgl::real a, const sgl::real max) {
			if( a < min ){
				return min;
			}
			else{
				if(a>max){
					return max;
				}
				else {
					return a;
				}
			}
		}

		// ----------------------------------------------------------------------------------------------------

		const sgl::real
		wrapRange(const sgl::real min, const sgl::real a, const sgl::real max) {
			assert( min < max );
			sgl::real f = a;
			while( f > max ) f-=max-min;
			while( f < min ) f+=max-min;
			return f;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		square(const sgl::real a) {
			return a*a;
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		squareRoot(const sgl::real a) {
			return sqrt(a);
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		cube(const sgl::real a) {
			return a*a*a;
		}
		
		// ----------------------------------------------------------------------------------------------------

		const sgl::real
		sin(const sgl::real a) {
			return std::sin(a);
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		cos(const sgl::real a) {
			return std::cos(a);
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		tan(const sgl::real a) {
			return std::tan(a);
		}
		
		// ----------------------------------------------------------------------------------------------------

		const sgl::real
		asin(const sgl::real a) {
			return std::asin(a);
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		acos(const sgl::real a) {
			return std::acos(a);
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		atan(const sgl::real a) {
			return std::atan(a);
		}
		
		// ----------------------------------------------------------------------------------------------------
		
		const sgl::real
		interpolate(const sgl::real p_from, sgl::real p_fromOrTo, sgl::real p_to) {
			return p_from +
						( p_fromOrTo * (p_to - p_from) );
		}
}