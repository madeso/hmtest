#include "vec3.hpp"
#include <cmath>
#include <cassert>

vec3::vec3(sgl::real x, sgl::real y, sgl::real z) {
	setX(x);
	setY(y);
	setZ(z);
}

// operators
const vec3& vec3::operator=(const vec3& vec) {
	setX(vec.getX());
	setY(vec.getY());
	setZ(vec.getZ());
	return (*this);
}
const bool vec3::operator!=(const vec3& vec) const {
	return !(vec==(*this));
}
const vec3 vec3::operator+() const {
	return (*this);
}
const vec3 vec3::operator+(const vec3& vec) const {
	vec3 temp(*this);
	temp+=vec;
	return temp;
}
void vec3::operator+=(const vec3& vec) {
	setX(getX() + vec.getX());
	setY(getY()+vec.getY());
	setZ(getZ()+vec.getZ());
}
const vec3 vec3::operator-() const {
	return vec3( -getX(), -getY(), -getZ() );
}
const vec3 vec3::operator-(const vec3& vec) const {
	vec3 temp(*this);
	temp-=vec;
	return temp;
}
void vec3::operator-=(const vec3& vec) {
	setX(getX()-vec.getX());
	setY(getY()-vec.getY());
	setZ(getZ()-vec.getZ());
}
void vec3::operator*=(sgl::real scalar) {
	setX(getX()*scalar);
	setY(getY()*scalar);
	setZ(getZ()*scalar);
}
void vec3::operator/=(sgl::real scalar) {
	(*this)*=(1/scalar);
}
const vec3 vec3::operator*(sgl::real scalar) const {
	vec3 temp(*this);
	temp*=scalar;
	return temp;
}
const vec3 operator*(sgl::real scalar, const vec3& vec) {
	return vec*scalar;
}
const vec3 vec3::operator/(sgl::real scalar) const {
	vec3 temp(*this);
	temp/=scalar;
	return temp;
}

sgl::real vec3::getDotProduct(const vec3& r) const {
	return getX()*r.getX() +
		getY()*r.getY() +
		getZ()*r.getZ();
}
sgl::real vec3::operator dot(const vec3& rexp) const {
	return getDotProduct(rexp);
}

vec3 vec3::getCrossProduct(const vec3& r) const {
	return vec3(getY()*r.getZ() - getZ()*r.getY(),
				getZ()*r.getX() - getX()*r.getZ(),
				getX()*r.getY() - getY()*r.getX());
}
vec3 vec3::operator cross(const vec3& rexp) const {
	return getCrossProduct(rexp);
}

// methods
sgl::real vec3::getLengthSquared() const {
	return  math::square(getX())+
			math::square(getY())+
			math::square(getZ());
}
vec3 vec3::getNormalized() const {
	vec3 temp(*this);
	temp.normalize();
	return temp;
}


// getters
const sgl::real vec3::getX() const {
	return data.x;
}
const sgl::real vec3::getY() const {
	return data.y;
}
const sgl::real vec3::getZ() const {
	return data.z;
}

// setters
void vec3::setX(sgl::real x) {
	data.x = x;
}
void vec3::setY(sgl::real y) {
	data.y = y;
}
void vec3::setZ(sgl::real z) {
	data.z = z;
}

// low-level accessors
const sgl::real vec3::get(int member) const {
	return data.array[member];
}
void vec3::set(int member, sgl::real value) {
	data.array[member] = value;
}

const sgl::real* vec3::getArray() const {
	return data.array;
}

const bool vec3::operator==(const vec3& vec) const {
	return math::equal(getX(), vec.getX()) && math::equal(getY(), vec.getY()) && math::equal(getZ(), vec.getZ());
}

sgl::real vec3::getLength() const {
	return math::squareRoot(getLengthSquared());
}
bool vec3::isUnit() const {
	return math::equal(1.0, getLengthSquared());
}
bool vec3::isZero() const {
	return math::equal(0.0, getLengthSquared());
}
void vec3::normalize() {
	const sgl::real len = getLength();
	if( math::equal(len, 0.0f) ) return;
	(*this) /= len;
}

namespace op {
	sgl::real vec3::lengthBetween(const ::vec3& from, const ::vec3& to) {
		return (from-to).getLength();
	}
	sgl::real vec3::lengthBetweenSquared(const ::vec3& from, const ::vec3& to) {
		return (from-to).getLengthSquared();
	}
	::vec3 vec3::getLinearInterpolation(const ::vec3& from, sgl::real value, const ::vec3& to) {
		return (to-from)*value + from;
	}
	void vec3::getLinearInterpolation(const ::vec3& from, sgl::real value, const ::vec3& to, ::vec3* out) {
		assert(out);
		*out = getLinearInterpolation(from, value, to);
	}

	sgl::real vec3::getCosAngleBetween(const ::vec3& a, const ::vec3& b) {
		assert(a.isUnit());
		assert(b.isUnit());
		return a dot b;
	}
	Angle vec3::getAngleBetween(const ::vec3& a, const ::vec3& b) {
		return Radian(acos(getCosAngleBetween(a, b)));
	}

	const ::vec3 vec3::origo(0,0,0);
	const ::vec3 vec3::xAxisPositive(1,0,0);
	const ::vec3 vec3::yAxisPositive(0,1,0);
	const ::vec3 vec3::zAxisPositive(0,0,1);
	const ::vec3 vec3::xAxisNegative(-1,0,0);
	const ::vec3 vec3::yAxisNegative(0,-1,0);
	const ::vec3 vec3::zAxisNegative(0, 0,-1);
};
