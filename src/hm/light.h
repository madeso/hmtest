#pragma once

#include "hm/vec3.h"


struct Attenuation
{
    Attenuation();

    float constant;
    float linear;
    float quadratic;

    void
    operator()(float c, float l, float q);
};


struct CommonLightAttributes
{
    CommonLightAttributes();

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    Attenuation attenuation;
};


struct Light : public CommonLightAttributes
{
    Light();


    bool spot;
    vec3 position;
    vec3 direction;

    int exponent;
    int cutoff;
};

