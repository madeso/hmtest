#ifndef ANGLE_HPP
#define ANGLE_HPP

#include "sgl_types.hpp"

class Angle {
public:
	explicit Angle(sgl::real radians) : mRadians(radians) {}
	Angle(sgl::real degrees, bool unused);
	sgl::real inRadians() const ;
	sgl::real inDegrees() const ;
private:
	sgl::real mRadians;
};

Angle Radian(sgl::real value);
Angle Degree(sgl::real value);

#endif