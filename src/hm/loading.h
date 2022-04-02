#pragma once

#include <vector>

#include "hm/state.h"


class Engine;

class Loading : public State
{
public:
    Loading(Engine* pEngine);


    void
    render(int, int) override;


    bool
    step(float pTime) override;


    void
    onEvent(const SDL_Event& pEvent) override;


private:
    Engine* mEngine;
    std::size_t mLoaded;
    std::size_t mToLoad;

    float mProgress;
    float mTime;
};


