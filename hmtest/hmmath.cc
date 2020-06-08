#include "math.h"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cassert>

namespace math
{
const bool
logicXor(const bool a, const bool b)
{
    return a != b;
}

const float
abs(const float a)
{
    if (a > 0.0f)
        return a;
    else
        return -a;
}

// ----------------------------------------------------------------------------------------------------

const float
maximum(const float a, const float b)
{
    if (a > b)
        return a;
    else
        return b;
}

// ----------------------------------------------------------------------------------------------------

const float
minimum(const float a, const float b)
{
    if (a < b)
        return a;
    else
        return b;
}

// ----------------------------------------------------------------------------------------------------

const float
floor(const float a)
{
    return std::floor(a);
}

// ----------------------------------------------------------------------------------------------------

const float
ceil(const float a)
{
    return std::ceil(a);
}

// ----------------------------------------------------------------------------------------------------

const bool
equal(const float a, const float b)
{
    return equal5(a, b);
}

// ----------------------------------------------------------------------------------------------------

const bool
equal0(const float a, const float b)
{
    return abs(a - b) < 1.0f;
}

// ----------------------------------------------------------------------------------------------------

const bool
equal1(const float a, const float b)
{
    return abs(a - b) < 0.1f;
}

// ----------------------------------------------------------------------------------------------------

const bool
equal2(const float a, const float b)
{
    return abs(a - b) < 0.01f;
}

// ----------------------------------------------------------------------------------------------------

const bool
equal3(const float a, const float b)
{
    return abs(a - b) < 0.001f;
}

// ----------------------------------------------------------------------------------------------------

const bool
equal4(const float a, const float b)
{
    return abs(a - b) < 0.0001f;
}

// ----------------------------------------------------------------------------------------------------

const bool
equal5(const float a, const float b)
{
    return abs(a - b) < 0.00001f;
}

// ----------------------------------------------------------------------------------------------------

const bool
equal6(const float a, const float b)
{
    return abs(a - b) < 0.000001f;
}

// ----------------------------------------------------------------------------------------------------

const bool
equal7(const float a, const float b)
{
    return abs(a - b) < 0.0000001f;
}

// ----------------------------------------------------------------------------------------------------

const float
clampZero(const float a)
{
    if (0.005f > abs(a))
    {
        return 0.0f;
    }
    else
    {
        return a;
    }
}

// ----------------------------------------------------------------------------------------------------

const float
limitRange(const float min, const float a, const float max)
{
    if (a < min)
    {
        return min;
    }
    else
    {
        if (a > max)
        {
            return max;
        }
        else
        {
            return a;
        }
    }
}

// ----------------------------------------------------------------------------------------------------

const float
wrapRange(const float min, const float a, const float max)
{
    assert(min < max);
    float f = a;
    while (f > max)
        f -= max - min;
    while (f < min)
        f += max - min;
    return f;
}

// ----------------------------------------------------------------------------------------------------

const float
square(const float a)
{
    return a * a;
}

// ----------------------------------------------------------------------------------------------------

const float
squareRoot(const float a)
{
    return sqrt(a);
}

// ----------------------------------------------------------------------------------------------------

const float
cube(const float a)
{
    return a * a * a;
}

// ----------------------------------------------------------------------------------------------------

const float
sin(const float a)
{
    return std::sin(a);
}

// ----------------------------------------------------------------------------------------------------

const float
cos(const float a)
{
    return std::cos(a);
}

// ----------------------------------------------------------------------------------------------------

const float
tan(const float a)
{
    return std::tan(a);
}

// ----------------------------------------------------------------------------------------------------

const float
asin(const float a)
{
    return std::asin(a);
}

// ----------------------------------------------------------------------------------------------------

const float
acos(const float a)
{
    return std::acos(a);
}

// ----------------------------------------------------------------------------------------------------

const float
atan(const float a)
{
    return std::atan(a);
}

// ----------------------------------------------------------------------------------------------------

const float
interpolate(const float p_from, float p_fromOrTo, float p_to)
{
    return p_from + (p_fromOrTo * (p_to - p_from));
}
}