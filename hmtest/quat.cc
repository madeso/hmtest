#include "quat.h"
#include "vec3.h"
#include "hmmath.h"
#include <cassert>

using namespace math;


Quat::Quat(float p_x, float p_y, float p_z, float p_w)
{
    set(p_x, p_y, p_z, p_w);
    normalize();
}


Quat::Quat(const Quat& p_other)
{
    set(p_other);
}


Quat::Quat()
{
    setIdentity();
}


Quat::~Quat()
{
}


float
Quat::length() const
{
    return squareRoot(lengthSquared());
}


float
Quat::lengthSquared() const
{
    return square(m_x) + square(m_y) + square(m_z) + square(m_w);
}


bool
Quat::isUnit() const
{
    const float ls = lengthSquared();
    return equal(ls, 1.0f);
}


void
Quat::setIdentity()
{
    m_x = m_y = m_z = 0.0f;
    m_w = 1.0f;
}


void
Quat::setEuler(const float p_x, const float p_y, const float p_z)
{
    const vec3 aAxis((float)math::sin(p_x / 2), 0, 0),
            bAxis(0, (float)math::sin(p_y / 2), 0),
            cAxis(0, 0, (float)math::sin(p_z / 2));
    const Angle aAngle = Radian(math::cos(p_x / 2)),
                bAngle = Radian(math::cos(p_y / 2)),
                cAngle = Radian(math::cos(p_z / 2));
    const Quat b(bAxis, bAngle), c(cAxis, cAngle);

    setRotation(aAxis, aAngle);
    (*this) *= b;
    (*this) *= c;
    normalize();
}


void
Quat::conjugate()
{
    set(-m_x, -m_y, -m_z, m_w);
}


Quat
Quat::getConjugate() const
{
    Quat temp(*this);
    temp.conjugate();
    return temp;
}


const Quat&
Quat::operator*=(const Quat& p_quat)
{
    vec3 v1(m_x, m_y, m_z);
    vec3 v2(p_quat.m_x, p_quat.m_y, p_quat.m_z);
    float sc1 = m_w;
    float sc2 = p_quat.m_w;

    float scalar = sc1 * sc2 - (v1 dot v2);
    vec3 result = v2 * sc1 + v1 * sc2 + (v1 cross v2);

    m_x = result.getX();
    m_y = result.getY();
    m_z = result.getZ();
    m_w = scalar;

    normalize();
    return *this;
}


Quat
Quat::operator*(const Quat& b) const
{
    Quat temp(*this);
    temp *= b;
    return temp;
}


void
Quat::set(const float p_x, const float p_y, const float p_z, const float p_w)
{
    m_x = p_x;
    m_y = p_y;
    m_z = p_z;
    m_w = p_w;
    normalize();
}


void
Quat::set(const Quat& p_quat)
{
    set(p_quat.m_x, p_quat.m_y, p_quat.m_z, p_quat.m_w);
}


Quat::Quat(const vec3& angle, const Angle& theta)
{
    setRotation(angle, theta);
}


void
Quat::setRotation(const vec3& pAxis, const Angle& theta)
{
    const float halfTheta = theta.inRadians() / 2;
    const float sinHalfTheta = math::sin(halfTheta);
    const float cosHalfTheta = math::cos(halfTheta);

    const vec3 axis = pAxis.getNormalized();
    m_x = axis.getX() * sinHalfTheta;
    m_y = axis.getY() * sinHalfTheta;
    m_z = axis.getZ() * sinHalfTheta;
    m_w = cosHalfTheta;
    normalize();
}


void
Quat::toAxisAngle(vec3* axis, Angle* theta) const
{
    assert(axis);
    assert(theta);
    const float len = length();
    assert(equal(1.0f, len));  // normalised
    const float cosAngle = m_w;
    const float angle = acos(cosAngle) * 2;
    float sinAngle = squareRoot(1 - square(cosAngle));
    if (abs(sinAngle) < 0.0005f)
    {
        sinAngle = 1;
    }
    *axis =
            vec3(m_x / sinAngle,
                 m_y / sinAngle,
                 m_z / sinAngle);  //.getNormalized();
    *theta = Radian(angle);
    // const float l = axis->getLength();
}


float
Quat::normalize()
{
    float l = length();
    m_x /= l;
    m_y /= l;
    m_z /= l;
    m_w /= l;
    return l;
}


Quat::Quat(const vec3& vec)
{
    set(vec.getX(), vec.getY(), vec.getZ(), 0.0f);
}


vec3
Quat::rotateVectorAroundOrigin(const vec3& vec) const
{
    const Quat point(vec);
    const Quat result = (*this) * point * getConjugate();
    return vec3(result.m_x, result.m_y, result.m_z);
}


vec3
Quat::getIn() const
{
    return rotateVectorAroundOrigin(op::vec3::zAxisNegative);
}


vec3
Quat::getUp() const
{
    return rotateVectorAroundOrigin(op::vec3::yAxisPositive);
}


vec3
Quat::getRight() const
{
    return rotateVectorAroundOrigin(op::vec3::xAxisPositive);
}


float
Quat::getX() const
{
    return m_x;
}


float
Quat::getY() const
{
    return m_y;
}


float
Quat::getZ() const
{
    return m_z;
}


float
Quat::getW() const
{
    return m_w;
}


bool
Quat::operator==(const Quat& pOther) const
{
    return math::equal(getX(), pOther.getX()) &&
            math::equal(getY(), pOther.getY()) &&
            math::equal(getZ(), pOther.getZ()) &&
            math::equal(getW(), pOther.getW());
}


bool
Quat::operator!=(const Quat& pOther) const
{
    return !(*this == pOther);
}


void
Quat::lookAt(const vec3& pFrom, const vec3& pTo, const vec3& pUp)
{
    assert(pUp.isUnit());
    lookInDirection((pTo - pFrom).getNormalized(), pUp);
}


void
Quat::lookInDirection(const vec3& pDirection, const vec3& pUp)
{
    assert(pDirection.isUnit());
    assert(pUp.isUnit());
    const vec3 v = (pDirection cross pUp).getNormalized();
    const vec3 u = v cross pDirection;
    const vec3 n = -pDirection;
#define vecAsArray(v)                                                          \
    {                                                                          \
        v.getX(), v.getY(), v.getZ()                                           \
    }
    const float mat[3][3] = {vecAsArray(v), vecAsArray(u), vecAsArray(n)};
    fromMatrix3(mat);
}


// this code is grabbed from somwhere
// rework with my own code from mathfaq
void
Quat::fromMatrix3(const float mat[3][3])
{
    int NXT[] = {1, 2, 0};
    float q[4];

    // check the diagonal
    float tr = mat[0][0] + mat[1][1] + mat[2][2];
    if (tr > 0.0f)
    {
        float s = (float)squareRoot(tr + 1.0f);
        m_w = s * 0.5f;
        s = 0.5f / s;
        m_x = (mat[1][2] - mat[2][1]) * s;
        m_y = (mat[2][0] - mat[0][2]) * s;
        m_z = (mat[0][1] - mat[1][0]) * s;
    }
    else
    {
        // diagonal is negative
        // get biggest diagonal element
        int i = 0;
        if (mat[1][1] > mat[0][0]) i = 1;
        if (mat[2][2] > mat[i][i]) i = 2;
        //setup index sequence
        int j = NXT[i];
        int k = NXT[j];

        float s =
                (float)squareRoot((mat[i][i] - (mat[j][j] + mat[k][k])) + 1.0f);

        q[i] = s * 0.5f;

        if (s != 0.0f) s = 0.5f / s;

        q[j] = (mat[i][j] + mat[j][i]) * s;
        q[k] = (mat[i][k] + mat[k][i]) * s;
        q[3] = (mat[j][k] - mat[k][j]) * s;

        m_x = q[0];
        m_y = q[1];
        m_z = q[2];
        m_w = q[3];
    }
}


Quat
Quat::slerp(const Quat& p_to, const float p_time) const
{
    float cosOmega =
            m_x * p_to.m_x + m_y * p_to.m_y + m_z * p_to.m_z + m_w * p_to.m_w;

    const float sign = (cosOmega < 0.0f) ? (-1.0f) : (1.0f);
    cosOmega *= sign;

    float scaleFrom;
    float scaleTo;
    constexpr float DELTA = 0.0001f;
    if ((1.0f - cosOmega) > DELTA)
    {
        // regular slerp
        const float omega = acos(cosOmega);
        const float sinOmega = math::sin(omega);
        scaleFrom = math::sin((1.0f - p_time) * omega) / sinOmega;
        scaleTo = math::sin(p_time * omega) / sinOmega;
    }
    else
    {
        // from and to are close, do a linear interpolation to speed things up a little
        scaleFrom = 1.0f - p_time;
        scaleTo = p_time;
    }
    scaleTo *= sign;  // save two multiplications

    return Quat(
            (scaleFrom * m_x) + (scaleTo * p_to.m_x),
            (scaleFrom * m_y) + (scaleTo * p_to.m_y),
            (scaleFrom * m_z) + (scaleTo * p_to.m_z),
            (scaleFrom * m_w) + (scaleTo * p_to.m_w));
}
