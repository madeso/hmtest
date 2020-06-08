#include "vec2.hpp"
#include <cmath>

vec2::vec2(sgl::real x, sgl::real y) {
	setX(x);
	setY(y);
}

	// operators
const vec2& vec2::operator=(const vec2& vec) {
	setX(vec.getX());
	setY(vec.getY());
	return (*this);
}
const bool vec2::operator!=(const vec2& vec) const {
	return !(vec==(*this));
}
const vec2 vec2::operator+() const {
	return (*this);
}
const vec2 vec2::operator+(const vec2& vec) const {
	vec2 temp(*this);
	temp+=vec;
	return temp;
}
void vec2::operator+=(const vec2& vec) {
	setX(getX() + vec.getX());
	setY(getY()+vec.getY());
}
const vec2 vec2::operator-() const {
	return vec2( -getX(), -getY() );
}
const vec2 vec2::operator-(const vec2& vec) const {
	vec2 temp(*this);
	temp-=vec;
	return temp;
}
void vec2::operator-=(const vec2& vec) {
	setX(getX()-vec.getX());
	setY(getY()-vec.getY());
}
void vec2::operator*=(sgl::real scalar) {
	setX(getX()*scalar);
	setY(getY()*scalar);
}
void vec2::operator/=(sgl::real scalar) {
	(*this)*=(1/scalar);
}
const vec2 vec2::operator*(sgl::real scalar) const {
	vec2 temp(*this);
	temp*=scalar;
	return temp;
}
const vec2 operator*(sgl::real scalar, const vec2& vec) {
	return vec*scalar;
}
const vec2 vec2::operator/(sgl::real scalar) const {
	vec2 temp(*this);
	temp/=scalar;
	return temp;
}

sgl::real vec2::getDotProduct(const vec2& r) const {
	return getX()*r.getX() + getY()*r.getY();
}
sgl::real vec2::operator dot(const vec2& rexp) const {
	return getDotProduct(rexp);
}

// methods
sgl::real vec2::getLengthSquared() const {
	return math::square(getX())+math::square(getY());
}

vec2 vec2::getNormalized() const {
	vec2 temp(*this);
	temp.normalize();
	return temp;
}
void vec2::normalize() {
	const sgl::real len = getLength();
	if( math::equal7(0, len) ) return;
	(*this) /= len;
}

// getters
const sgl::real vec2::getX() const {
	return data.x;
}
const sgl::real vec2::getY() const {
	return data.y;
}

const sgl::real* vec2::getArray() const {
	return data.array;
}

// setters
void vec2::setX(sgl::real x) {
	data.x = x;
}
void vec2::setY(sgl::real y) {
	data.y = y;
}

void vec2::truncate(sgl::real l) {
	const sgl::real cl = getLength();
	if( cl > l ) {
		(*this) /= cl;
		(*this) *= l;
	}
}
vec2 vec2::getTruncated(sgl::real l) const {
	vec2 temp(getX(), getY());
	temp.truncate(l);
	return temp;
}

// low-level accessors
const sgl::real vec2::get(int member) const {
	return data.array[member];
}
void vec2::set(int member, sgl::real value) {
	data.array[member] = value;
}

const bool vec2::operator==(const vec2& vec) const {
	return math::equal(getX(), vec.getX()) && math::equal(getY(), vec.getY());
}

sgl::real vec2::getLength() const {
	return math::squareRoot(getLengthSquared());
}