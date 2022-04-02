#ifndef VEC2_HPP
#define VEC2_HPP

#include "hm/vecop.h"
#include "hm/math.h"


class vec2
{
public:
    // constructors
    vec2(float x, float y);

    vec2(const vec2& vec);

    // operators
    const vec2&
    operator=(const vec2& vec);

    bool
    operator==(const vec2& vec) const;

    bool
    operator!=(const vec2& vec) const;

    const vec2
    operator+() const;

    const vec2
    operator+(const vec2& vec) const;

    void
    operator+=(const vec2& vec);

    const vec2
    operator-() const;

    const vec2
    operator-(const vec2& vec) const;

    void
    operator-=(const vec2& vec);

    void
    operator*=(float scalar);

    void
    operator/=(float scalar);

    const vec2
    operator*(float scalar) const;

    friend inline const vec2
    operator*(float scalar, const vec2& vec);

    const vec2
    operator/(float scalar) const;

    float
    getDotProduct(const vec2& r) const;

    float operator dot(const vec2& rexp) const;

    // methods
    float
    getLengthSquared() const;

    float
    getLength() const;

    vec2
    getNormalized() const;

    void
    normalize();

    // getters
    float
    getX() const;

    float
    getY() const;

    // setters
    void
    setX(float x);

    void
    setY(float y);

    // low-level accessors
    float
    get(int member) const;

    void
    set(int member, float value);

    void
    truncate(float l);

    vec2
    getTruncated(float l) const;

    const float*
    getArray() const;

private:
    float x;
    float y;
};


#endif  // VEC2_HPP
