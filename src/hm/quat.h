#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "hm/vec3.h"
#include "hm/angle.h"


class Quat
{
public:
    Quat(const Quat& p_other);

    Quat(float p_x, float p_y, float p_z, float p_w);

    Quat(const vec3& axis, const Angle& theta);

    Quat(const vec3& vec);

    Quat();

    ~Quat();

    void
    setEuler(float p_x, float p_y, float p_z);

    void
    setRotation(const vec3& axis, const Angle& theta);

    float
    length() const;

    float
    lengthSquared() const;

    bool
    isUnit() const;

    // returns length()
    float
    normalize();

    //void toMatrix(Matrix& p_out);
    void
    conjugate();

    Quat
    getConjugate() const;

    const Quat&
    operator*=(const Quat& p_quat);

    Quat
    operator*(const Quat& a) const;

    void
    set(float p_x, float p_y, float p_z, float p_w);

    void
    set(const Quat& p_quat);

    void
    setIdentity();

    vec3
    rotateVectorAroundOrigin(const vec3& v) const;

    vec3
    getIn() const;

    vec3
    getUp() const;

    vec3
    getRight() const;

    float
    getX() const;

    float
    getY() const;

    float
    getZ() const;

    float
    getW() const;

    Quat
    slerp(const Quat& p_to, float p_time) const;

    // glRotate(theta, axis.x, axis.y, axis.z)
    void
    toAxisAngle(vec3* axis, Angle* theta) const;

    void
    lookAt(const vec3& pFrom, const vec3& pTo, const vec3& pUp);

    void
    lookInDirection(const vec3& pDirection, const vec3& pUp);

    void
    fromMatrix3(const float pMatrix[3][3]);

    bool
    operator==(const Quat& pOther) const;

    bool
    operator!=(const Quat& pOther) const;

private:
    float m_x;
    float m_y;
    float m_z;
    float m_w;
};


#endif
