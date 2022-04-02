#include "hm/light.h"


Attenuation::Attenuation() : constant(0.1f), linear(0.1f), quadratic(0.005f)
{
}


void
Attenuation::operator()(float c, float l, float q)
{
    constant = c;
    linear = l;
    quadratic = q;
}


CommonLightAttributes::CommonLightAttributes()
    : ambient(1.0f, 1.0f, 1.0f)
    , diffuse(1.0f, 1.0f, 1.0f)
    , specular(0.0f, 0.0f, 0.0f)
{
}


Light::Light()
    : spot(false)
    , position(0, 0, 0)
    , direction(0, 0, -1)
    , exponent(128)
    , cutoff(30)
{
}
