#pragma once

#include "hm/vec3.h"


struct Material
{
    Material();

    void
    sendToGl();

    vec3 mAmbient;
    vec3 mDiffuse;
    vec3 mSpecular;
    vec3 mEmissive;
    float mShininess;
    float mAlpha;
};

