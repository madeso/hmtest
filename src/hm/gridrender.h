#pragma once

#include "hm/vec2.h"
#include "hm/texturerenderer.h"


namespace detail
{
    constexpr int MIDDLE_WIDTH = 48;
    constexpr int MIDDLE_HEIGHT = 32;

    struct Dot
    {
        Dot();

        void
        bang(const vec2& center, float power, float additionalForce);

        void
        init(const vec2& p);

        void
        step(float time, float xlim, float ylim, float maxForce, float maxSpeed);

        void
        stepArrival(
                float time,
                float xlim,
                float ylim,
                float maxForce,
                float maxSpeed);

        void
        addForce(const vec2& p);

        void
        renderLine();

        void
        renderDot();

        void
        textureVertex();

        vec2 pos;
        vec2 start;
        vec2 vel;
        vec2 movement;
    };
}


class GridRender
{
public:
    GridRender(TextureRenderer* pRender);

    void
    render();

    void
    addRandom();

    void
    bang();

    void
    step(float pTime);

    void
    setEnable(bool e);

private:
    TextureRenderer* renderer;
    bool isEnabled;
    detail::Dot array[detail::MIDDLE_WIDTH][detail::MIDDLE_HEIGHT];
    float accum;
};

