#ifndef MATH_HPP
#define MATH_HPP

namespace math
{
constexpr float PI = 3.1415926535898f;

constexpr float TWO_PI = PI * 2.0f;

//const real PI = 3.14159265358979323846f; // grabbed from a source - is it really needed

constexpr float ONE_DEG_IN_RADIAN = PI / 180;

bool
logicXor(bool a, bool b);

float
abs(float a);

float
maximum(float a, float b);

float
minimum(float a, float b);

float
floor(float a);

float
ceil(float a);

bool
equal(float a, float b);

bool
equal0(float a, float b);

bool
equal1(float a, float b);

bool
equal2(float a, float b);

bool
equal3(float a, float b);

bool
equal4(float a, float b);

bool
equal5(float a, float b);

bool
equal6(float a, float b);

bool
equal7(float a, float b);

float
clampZero(float a);

float
limitRange(float min, float a, float max);

float
wrapRange(float min, float a, float max);

float
square(float a);

float
cube(float a);

float
squareRoot(float a);

float
sin(float a);

float
cos(float a);

float
tan(float a);

float
asin(float a);

float
acos(float a);

float
atan(float a);

float
interpolate(float p_from, float p_fromOrTo, float p_to);
}

#endif
