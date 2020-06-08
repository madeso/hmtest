#ifndef VEC2_HPP
#define VEC2_HPP

#include "vecop.hpp"
#include "math.hpp"

class vec2 {
public:
	// constructors
	vec2(sgl::real x, sgl::real y);

	// operators
	const vec2& operator=(const vec2& vec);
	const bool operator==(const vec2& vec) const;
	const bool operator!=(const vec2& vec) const;
	const vec2 operator+() const;
	const vec2 operator+(const vec2& vec) const;
	void operator+=(const vec2& vec);
	const vec2 operator-() const;
	const vec2 operator-(const vec2& vec) const;
	void operator-=(const vec2& vec);
	void operator*=(sgl::real scalar);
	void operator/=(sgl::real scalar);
	const vec2 operator*(sgl::real scalar) const;
	friend inline const vec2 operator*(sgl::real scalar, const vec2& vec);
	const vec2 operator/(sgl::real scalar) const;

	sgl::real getDotProduct(const vec2& r) const;
	sgl::real operator dot(const vec2& rexp) const;

	// methods
	sgl::real getLengthSquared() const;
	sgl::real getLength() const;
	vec2 getNormalized() const;
	void normalize();

	// getters
	const sgl::real getX() const;
	const sgl::real getY() const;

	// setters
	void setX(sgl::real x);
	void setY(sgl::real y);

	// low-level accessors
	const sgl::real get(int member) const;
	void set(int member, sgl::real value);

	void truncate(sgl::real l);
	vec2 getTruncated(sgl::real l) const;

	const sgl::real* getArray() const;
private:
	union {
		sgl::real array[2];
		struct {
			sgl::real x;
			sgl::real y;
		};
	} data;
};

#endif // VEC2_HPP