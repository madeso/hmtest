#pragma once

#include <stack>
#include <memory>

#include "hm/state.h"
#include "hm/dep_sdl.h"


class StateManager
{
public:
    void
    addState(State* pState);

    void
    step(float pTime);

    void
    render(int width, int height);

    void
    onEvent(const SDL_Event& pEvent);

private:
    std::stack<std::shared_ptr<State>> mStack;
};
