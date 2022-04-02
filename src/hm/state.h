#pragma once

#include "hm/dep_sdl.h"


class State
{
public:
    virtual ~State();

    virtual void
    render(int width, int height) = 0;

    virtual bool
    step(float pTime) = 0;

    virtual void
    onEvent(const SDL_Event& pEvent) = 0;
};
