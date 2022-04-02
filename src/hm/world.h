#pragma once

#include "hm/vec3.h"

#include "hm/image.h"
#include "hm/shader.h"
#include "hm/mesh.h"


struct Light;
class Engine;


class World
{
public:
    explicit World(Engine* pEngine);


    void
    step(float pTime);


    void
    render();

    void
    setSpot(const vec3& pos, const vec3& dir);

    void
    setSpot(bool enable);


protected:
    void
    drawOnDepthBuffer();

    void
    setupOpenglLight(const Light& light);

    void
    drawAmbient();

    void
    doLight(const Light& light);

    void
    drawLightning();

    void
    drawTextures();

    void
    drawScene(bool applyTexture, bool doTransparent);

private:
    Image wall;
    Image water;
    Image spotImage;

    bool hasSpot;
    vec3 spotLocation;
    vec3 spotDirection;

    Shader shaderPointLight;
    Shader shaderSpotLight;

    Mesh mesh;
};

