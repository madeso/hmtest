#include "hm/loading.h"

#include "hm/dep_gl.h"
#include "hm/engine.h"
#include "hm/opengl.h"


Loading::Loading(Engine* pEngine)
    : mEngine(pEngine)
    , mLoaded(0)
    , mToLoad(0)
    , mProgress(0)
    , mTime(0)
{
    mEngine->loadMedia();
    mToLoad = mEngine->getLoadCount();
}


void
Loading::render(int, int)
{
    setDisplay2d();
    glDisable(GL_TEXTURE_2D);
    glColor3d(0.8, 0.8, 0.8);
    glBegin(GL_QUADS);
    rectangle(0.1, 0.8, 0.8, 0.02);
    if (mLoaded > 0)
    {
        glColor3d(0.4, 0.4, 0.4);
        rectangle(0.1, 0.8, 0.8 * mProgress, 0.02);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
}


bool
Loading::step(float pTime)
{
    const float SIZE = 0.1;
    mTime += pTime;
    if (mTime > SIZE)
    {
        mTime -= SIZE;
        mProgress = ((float)mLoaded) / ((float)mToLoad);
        if (mLoaded >= mToLoad) return false;
        mEngine->processOne();
        mLoaded += 1;
        return true;
    }
    return true;
}


void
Loading::onEvent(const SDL_Event&)
{
}

