#ifndef MATH_HPP
#define MATH_HPP

#include "sgl_types.hpp"

namespace math {
		const sgl::real PI = 3.1415926535898f;
		const sgl::real TWO_PI = PI * 2.0f;
		//const real PI = 3.14159265358979323846f; // grabbed from a source - is it really needed
		const sgl::real ONE_DEG_IN_RADIAN = PI / 180;

		const bool logicXor(const bool a, const bool b);

		const sgl::real abs(const sgl::real a);
		const sgl::real maximum(const sgl::real a, const sgl::real b);
		const sgl::real minimum(const sgl::real a, const sgl::real b);

		const sgl::real floor(const sgl::real a);
		const sgl::real ceil(const sgl::real a);

		const bool equal(const sgl::real a, const sgl::real b);
		const bool equal0(const sgl::real a, const sgl::real b);
		const bool equal1(const sgl::real a, const sgl::real b);
		const bool equal2(const sgl::real a, const sgl::real b);
		const bool equal3(const sgl::real a, const sgl::real b);
		const bool equal4(const sgl::real a, const sgl::real b);
		const bool equal5(const sgl::real a, const sgl::real b);
		const bool equal6(const sgl::real a, const sgl::real b);
		const bool equal7(const sgl::real a, const sgl::real b);
		
		const sgl::real clampZero(const sgl::real a);
		const sgl::real limitRange(const sgl::real min, const sgl::real a, const sgl::real max);

		const sgl::real wrapRange(const sgl::real min, const sgl::real a, const sgl::real max);

		const sgl::real square(const sgl::real a);
		const sgl::real cube(const sgl::real a);
		const sgl::real squareRoot(const sgl::real a);

		const sgl::real sin(const sgl::real a);
		const sgl::real cos(const sgl::real a);
		const sgl::real tan(const sgl::real a);

		const sgl::real asin(const sgl::real a);
		const sgl::real acos(const sgl::real a);
		const sgl::real atan(const sgl::real a);

		const sgl::real interpolate(const sgl::real p_from, sgl::real p_fromOrTo, sgl::real p_to);
}

#endif