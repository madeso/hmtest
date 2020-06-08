#ifndef ANGLE_HPP
#define ANGLE_HPP

class Angle
{
public:
    explicit Angle(float radians) : mRadians(radians)
    {
    }
    Angle(float degrees, bool unused);
    float
    inRadians() const;
    float
    inDegrees() const;

private:
    float mRadians;
};

Angle
Radian(float value);
Angle
Degree(float value);

#endif