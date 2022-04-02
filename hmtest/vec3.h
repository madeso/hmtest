#ifndef VEC3_HPP
#define VEC3_HPP

#include "vecop.h"
#include "hmmath.h"
#include "angle.h"


class vec3
{
public:
    // constructors
    vec3(float x, float y, float z);
    vec3(const vec3& vec);

    // operators
    const vec3&
    operator=(const vec3& vec);

    bool
    operator==(const vec3& vec) const;

    bool
    operator!=(const vec3& vec) const;

    const vec3
    operator+() const;  // +foo

    const vec3
    operator+(const vec3& vec) const;

    void
    operator+=(const vec3& vec);

    const vec3
    operator-() const;  // -foo

    const vec3
    operator-(const vec3& vec) const;

    void
    operator-=(const vec3& vec);

    void
    operator*=(float scalar);

    void
    operator/=(float scalar);

    const vec3
    operator*(float scalar) const;

    const vec3
    operator/(float scalar) const;

    float
    getDotProduct(const vec3& r) const;

    float operator dot(const vec3& rexp) const;

    vec3
    getCrossProduct(const vec3& r) const;

    vec3 operator cross(const vec3& rexp) const;

    // methods
    float
    getLengthSquared() const;

    float
    getLength() const;

    vec3
    getNormalized() const;

    void
    normalize();

    bool
    isUnit() const;

    bool
    isZero() const;

    // getters
    float
    getX() const;

    float
    getY() const;

    float
    getZ() const;

    // setters
    void
    setX(float x);

    void
    setY(float y);

    void
    setZ(float z);

    // low-level accessors
    float
    get(int member) const;

    void
    set(int member, float value);

    const float*
    getArray() const;

private:
    float x;
    float y;
    float z;
};


const vec3
operator*(float scalar, const vec3& vec);


namespace op
{
class vec3
{
public:
    float static lengthBetween(const ::vec3& from, const ::vec3& to);

    float static lengthBetweenSquared(const ::vec3& from, const ::vec3& to);

    ::vec3 static getLinearInterpolation(
            const ::vec3& from,
            float value,
            const ::vec3& to);

    void static getLinearInterpolation(
            const ::vec3& from,
            float value,
            const ::vec3& to,
            ::vec3* out);

    float static getCosAngleBetween(const ::vec3& a, const ::vec3& b);

    Angle static getAngleBetween(const ::vec3& a, const ::vec3& b);

    static const ::vec3 origo;
    static const ::vec3 xAxisPositive;
    static const ::vec3 yAxisPositive;
    static const ::vec3 zAxisPositive;
    static const ::vec3 xAxisNegative;
    static const ::vec3 yAxisNegative;
    static const ::vec3 zAxisNegative;
};
};

#endif
