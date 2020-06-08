#include "Angle.hpp"
#include "math.hpp"

namespace convert {
	const sgl::real
	radToDeg(const sgl::real a) {
		return a / math::ONE_DEG_IN_RADIAN;
	}
			
	const sgl::real
	degToRad(const sgl::real a){
		return a * math::ONE_DEG_IN_RADIAN;
	}
}

Angle Radian(sgl::real value) {
	return Angle(value);
}
Angle Degree(sgl::real value) {
	return Angle(value, false);
}

Angle::Angle(sgl::real degrees, bool unused) : mRadians(convert::degToRad(degrees)) {
}

sgl::real Angle::inRadians() const {
	return mRadians;
}
sgl::real Angle::inDegrees() const {
	return convert::radToDeg(mRadians);
}