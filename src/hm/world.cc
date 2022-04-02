#include "hm/world.h"

#include "hm/dep_gl.h"
#include "hm/material.h"
#include "hm/light.h"
#include "hm/loadedimage.h"


World::World(Engine* pEngine)
    : wall(pEngine, ImageDescription("images/wand.bmp"))
    , water(pEngine, ImageDescription("images/water512.jpg"))
    , spotImage(pEngine, ImageDescription("images/spot.png"))
    , hasSpot(false)
    , spotLocation(0, 0, 0)
    , spotDirection(0, 0, -1)
    , shaderPointLight(
                "shaders/light_point.vert",
                "shaders/light_point.frag")
    , shaderSpotLight("shaders/light_spot.vert", "shaders/light_spot.frag")
    , mesh("level.obj")
{
}


void
World::step(float)
{
}


void
World::render()
{
    if (false)
    {
        drawScene(true, true);
    }
    else
    {
        drawOnDepthBuffer();
        drawAmbient();
        drawLightning();
        drawTextures();
    }
}


void
World::setSpot(const vec3& pos, const vec3& dir)
{
    spotLocation = pos;
    spotDirection = dir;
}


void
World::setSpot(bool enable)
{
    hasSpot = enable;
}


void
World::drawOnDepthBuffer()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    drawScene(false, false);
}


void
World::setupOpenglLight(const Light& light)
{
    if (light.spot)
    {
        glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, light.exponent);
        glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, light.cutoff);
    }
    else
    {
        glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, 0);
        glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
    }

    glLightf(
            GL_LIGHT0,
            GL_CONSTANT_ATTENUATION,
            light.attenuation.constant);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, light.attenuation.linear);
    glLightf(
            GL_LIGHT0,
            GL_QUADRATIC_ATTENUATION,
            light.attenuation.quadratic);

    //#pragma warning(disable : 4244)
    float directionArray[3] = {
            light.direction.getX(),
            light.direction.getY(),
            light.direction.getZ()};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, directionArray);

    float positionArray[4] = {
            light.position.getX(),
            light.position.getY(),
            light.position.getZ(),
            1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, positionArray);

    float ambientArray[4] = {
            light.ambient.getX(),
            light.ambient.getY(),
            light.ambient.getZ(),
            1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientArray);

    float diffuseArray[4] = {
            light.diffuse.getX(),
            light.diffuse.getY(),
            light.diffuse.getZ(),
            1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseArray);

    float specularArray[4] = {
            light.specular.getX(),
            light.specular.getY(),
            light.specular.getZ(),
            1.0f};
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularArray);
    //#pragma warning(default : 4244)
}


void
World::drawAmbient()
{
    glDisable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    float ambient = 0.0;
    glColor4d(ambient, ambient, ambient, 1);
    drawScene(false, false);
    glColor4d(1, 1, 1, 1);
}


void
World::doLight(const Light& light)
{
    glDisable(GL_BLEND);
    //glClear(GL_STENCIL_BUFFER_BIT);
    glDepthFunc(GL_LESS);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    //glStencilFunc(GL_ALWAYS, 0, 0);

    /*for(allObjects) {
        ComputeShadowVolume();
        glCullFace(GL_FRONT);
        glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
        DrawShadowVolume();
        glCullFace(GL_BACK);
        glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
        DrawShadowVolume();
    }*/

    glDepthFunc(GL_LEQUAL);
    //glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    //glStencilFunc(GL_EQUAL, 0x0, 0xff);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    glEnable(GL_TEXTURE_GEN_R);
    glEnable(GL_TEXTURE_GEN_Q);
    if (light.spot)
    {
        setupOpenglLight(light);
        shaderSpotLight.bind();
        unsigned int spotTexture =
                shaderSpotLight.getUniform("spotTexture");
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glBindTexture(GL_TEXTURE_2D, spotImage->getId());
        glUniform1iARB(spotTexture, 0);
    }
    else
    {
        setupOpenglLight(light);
        shaderPointLight.bind();
    }
    drawScene(false, true);
    Shader::unbind();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
    //glDisable(GL_LIGHTING);
}


void
World::drawLightning()
{
    glBlendFunc(GL_ONE, GL_ONE);
    glDepthMask(GL_FALSE);
    float ambientArray[4] = {0, 0, 0, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientArray);

    //	glEnable(GL_STENCIL_TEST);
    /*for(allLights)*/ {
        Light light;
        doLight(light);

        if (hasSpot)
        {
            light.position = spotLocation;
            light.spot = true;
            light.direction = spotDirection;
            light.attenuation(0.05f, 0.01f, 0.005f);
            light.cutoff = 20;
            doLight(light);
        }
    }

    //	glDisable(GL_STENCIL_TEST);
}


void
World::drawTextures()
{
    glCullFace(GL_BACK);
    glDepthFunc(GL_LEQUAL);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    /*glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_ZERO);
    glEnable(GL_TEXTURE_2D);
    drawScene(true, true);*/

    glDepthMask(GL_TRUE);
}


void
World::drawScene(bool applyTexture, bool)
{
    Material().sendToGl();
    if (applyTexture) glBindTexture(GL_TEXTURE_2D, wall->getId());
    mesh.render();
}
