#include "angle.h"
#include "hmmath.h"

namespace convert
{
const float
radToDeg(const float a)
{
    return a / math::ONE_DEG_IN_RADIAN;
}

const float
degToRad(const float a)
{
    return a * math::ONE_DEG_IN_RADIAN;
}
}

Angle
Radian(float value)
{
    return Angle(value);
}
Angle
Degree(float value)
{
    return Angle(value, false);
}

Angle::Angle(float degrees, bool unused) : mRadians(convert::degToRad(degrees))
{
}

float
Angle::inRadians() const
{
    return mRadians;
}
float
Angle::inDegrees() const
{
    return convert::radToDeg(mRadians);
}