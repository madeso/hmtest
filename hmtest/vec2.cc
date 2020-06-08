#include "vec2.h"

#include <cmath>


vec2::vec2(float x, float y)
{
    setX(x);
    setY(y);
}


// operators
const vec2&
vec2::operator=(const vec2& vec)
{
    setX(vec.getX());
    setY(vec.getY());
    return (*this);
}


const bool
vec2::operator!=(const vec2& vec) const
{
    return !(vec == (*this));
}


const vec2
vec2::operator+() const
{
    return (*this);
}


const vec2
vec2::operator+(const vec2& vec) const
{
    vec2 temp(*this);
    temp += vec;
    return temp;
}


void
vec2::operator+=(const vec2& vec)
{
    setX(getX() + vec.getX());
    setY(getY() + vec.getY());
}


const vec2
vec2::operator-() const
{
    return vec2(-getX(), -getY());
}


const vec2
vec2::operator-(const vec2& vec) const
{
    vec2 temp(*this);
    temp -= vec;
    return temp;
}


void
vec2::operator-=(const vec2& vec)
{
    setX(getX() - vec.getX());
    setY(getY() - vec.getY());
}


void
vec2::operator*=(float scalar)
{
    setX(getX() * scalar);
    setY(getY() * scalar);
}


void
vec2::operator/=(float scalar)
{
    (*this) *= (1 / scalar);
}


const vec2
vec2::operator*(float scalar) const
{
    vec2 temp(*this);
    temp *= scalar;
    return temp;
}


const vec2
operator*(float scalar, const vec2& vec)
{
    return vec * scalar;
}


const vec2
vec2::operator/(float scalar) const
{
    vec2 temp(*this);
    temp /= scalar;
    return temp;
}


float
vec2::getDotProduct(const vec2& r) const
{
    return getX() * r.getX() + getY() * r.getY();
}


float vec2::operator dot(const vec2& rexp) const
{
    return getDotProduct(rexp);
}


// methods
float
vec2::getLengthSquared() const
{
    return math::square(getX()) + math::square(getY());
}


vec2
vec2::getNormalized() const
{
    vec2 temp(*this);
    temp.normalize();
    return temp;
}


void
vec2::normalize()
{
    const float len = getLength();
    if (math::equal7(0, len)) return;
    (*this) /= len;
}


// getters
const float
vec2::getX() const
{
    return data.x;
}


const float
vec2::getY() const
{
    return data.y;
}


const float*
vec2::getArray() const
{
    return data.array;
}


// setters
void
vec2::setX(float x)
{
    data.x = x;
}


void
vec2::setY(float y)
{
    data.y = y;
}


void
vec2::truncate(float l)
{
    const float cl = getLength();
    if (cl > l)
    {
        (*this) /= cl;
        (*this) *= l;
    }
}


vec2
vec2::getTruncated(float l) const
{
    vec2 temp(getX(), getY());
    temp.truncate(l);
    return temp;
}


// low-level accessors
const float
vec2::get(int member) const
{
    return data.array[member];
}


void
vec2::set(int member, float value)
{
    data.array[member] = value;
}


const bool
vec2::operator==(const vec2& vec) const
{
    return math::equal(getX(), vec.getX()) && math::equal(getY(), vec.getY());
}


float
vec2::getLength() const
{
    return math::squareRoot(getLengthSquared());
}
