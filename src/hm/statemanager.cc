#include "hm/statemanager.h"


void
StateManager::addState(State* pState)
{
    std::shared_ptr<State> state(pState);
    mStack.push(state);
}

void
StateManager::step(float pTime)
{
    if (!mStack.empty())
    {
        if (!mStack.top()->step(pTime))
        {
            mStack.pop();
        }
    }
}

void
StateManager::render(int width, int height)
{
    if (!mStack.empty())
    {
        mStack.top()->render(width, height);
    }
}

void
StateManager::onEvent(const SDL_Event& pEvent)
{
    if (!mStack.empty())
    {
        mStack.top()->onEvent(pEvent);
    }
}


