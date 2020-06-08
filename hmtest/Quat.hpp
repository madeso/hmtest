#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "vec3.hpp"
#include "Angle.hpp"

class Quat {
public:
	Quat(const Quat& p_other);
	Quat(const sgl::real p_x, const sgl::real p_y, const sgl::real p_z, const sgl::real p_w);
	Quat(const vec3& axis, const Angle& theta);
	Quat(const vec3& vec);
	Quat();
	~Quat();
	void setEuler(const sgl::real p_x, const sgl::real p_y, const sgl::real p_z);
	void setRotation(const vec3& axis, const Angle& theta);
	sgl::real length() const;
	sgl::real lengthSquared() const;
	bool isUnit() const;
	// returns length()
	sgl::real normalize();
	//void toMatrix(Matrix& p_out);
	void conjugate();
	Quat getConjugate() const;
	const Quat& operator*=(const Quat& p_quat);
	Quat operator*(const Quat& a) const;
	void set(const sgl::real p_x, const sgl::real p_y, const sgl::real p_z, const sgl::real p_w);
	void set(const Quat& p_quat);
	void setIdentity();
			
	vec3 rotateVectorAroundOrigin(const vec3& v) const ;

	vec3 getIn() const ;
	vec3 getUp() const ;
	vec3 getRight() const ;
	const sgl::real getX() const ;
	const sgl::real getY() const ;
	const sgl::real getZ() const ;
	const sgl::real getW() const ;

	Quat slerp(const Quat& p_to, const sgl::real p_time) const;

	// glRotate(theta, axis.x, axis.y, axis.z)
	void toAxisAngle(vec3* axis, Angle* theta) const;

	void lookAt(const vec3& pFrom, const vec3& pTo, const vec3& pUp);
	void lookInDirection(const vec3& pDirection, const vec3& pUp);
	void fromMatrix3(const sgl::real pMatrix[3][3]);

	bool operator==(const Quat& pOther) const;
	bool operator!=(const Quat& pOther) const;
private:
	sgl::real m_x;
	sgl::real m_y;
	sgl::real m_z;
	sgl::real m_w;
};

#endif
