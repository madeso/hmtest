#ifndef VEC3_HPP
#define VEC3_HPP

#include "vecop.hpp"
#include "math.hpp"
#include "Angle.hpp"

class vec3 {
public:
	// constructors
	vec3(sgl::real x, sgl::real y, sgl::real z);

	// operators
	const vec3& operator=(const vec3& vec);
	const bool operator==(const vec3& vec) const;
	const bool operator!=(const vec3& vec) const;
	const vec3 operator+() const;// +foo
	const vec3 operator+(const vec3& vec) const;
	void operator+=(const vec3& vec);
	const vec3 operator-() const; // -foo
	const vec3 operator-(const vec3& vec) const;
	void operator-=(const vec3& vec);
	void operator*=(sgl::real scalar);
	void operator/=(sgl::real scalar);
	const vec3 operator*(sgl::real scalar) const;
	const vec3 operator/(sgl::real scalar) const;

	sgl::real getDotProduct(const vec3& r) const;
	sgl::real operator dot(const vec3& rexp) const;

	vec3 getCrossProduct(const vec3& r) const;
	vec3 operator cross(const vec3& rexp) const;

	// methods
	sgl::real getLengthSquared() const;
	sgl::real getLength() const;
	vec3 getNormalized() const;
	void normalize();

	bool isUnit() const;
	bool isZero() const;

	// getters
	const sgl::real getX() const;
	const sgl::real getY() const;
	const sgl::real getZ() const;

	// setters
	void setX(sgl::real x);
	void setY(sgl::real y);
	void setZ(sgl::real z);

	// low-level accessors
	const sgl::real get(int member) const;
	void set(int member, sgl::real value);

	const sgl::real* getArray() const;
private:
	union {
		sgl::real array[3];
		struct {
			sgl::real x;
			sgl::real y;
			sgl::real z;
		};
	} data;
};

const vec3 operator*(sgl::real scalar, const vec3& vec);

namespace op {
	class vec3 {
	public:
		sgl::real static lengthBetween(const ::vec3& from, const ::vec3& to);
		sgl::real static lengthBetweenSquared(const ::vec3& from, const ::vec3& to);

		::vec3 static getLinearInterpolation(const ::vec3& from, sgl::real value, const ::vec3& to);
		void static getLinearInterpolation(const ::vec3& from, sgl::real value, const ::vec3& to, ::vec3* out);

		sgl::real static getCosAngleBetween(const ::vec3& a, const ::vec3& b) ;
		Angle static getAngleBetween(const ::vec3& a, const ::vec3& b) ;

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