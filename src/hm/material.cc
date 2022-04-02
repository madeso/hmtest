#include "hm/material.h"

#include "hm/dep_gl.h"


Material::Material()
    : mAmbient(0.2f, 0.2f, 0.2f)
    , mDiffuse(0.8f, 0.8f, 0.8f)
    , mSpecular(1.0f, 1.0f, 1.0f)
    , mEmissive(0.0f, 0.0f, 0.0f)
    , mShininess(100.0f)
    , mAlpha(1.0f)
{
}


void
Material::sendToGl()
{
    //#pragma warning(disable : 4244)
    const float alpha = mAlpha;
    float ambientArray[4] = {
            mAmbient.getX(),
            mAmbient.getY(),
            mAmbient.getZ(),
            alpha};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray);

    float diffuseArray[4] = {
            mDiffuse.getX(),
            mDiffuse.getY(),
            mDiffuse.getZ(),
            alpha};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray);

    float specularArray[4] = {
            mSpecular.getX(),
            mSpecular.getY(),
            mSpecular.getZ(),
            alpha};
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray);

    float emissionArray[4] = {
            mEmissive.getX(),
            mEmissive.getY(),
            mEmissive.getZ(),
            alpha};
    glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray);

    glMaterialf(GL_FRONT, GL_SHININESS, mShininess);
    //#pragma warning(default : 4244)
}

