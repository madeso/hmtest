#include "hm/media.h"

#include "hm/assert.h"
#include "hm/engine.h"


Media::Media(Engine* pEngine, bool pHardware)
    : loaded(false)
    , hardware(pHardware)
    , engine(pEngine)
{
    Assert(pEngine, "Need a valid engine");
    engine->media_register(this);
}


Media::~Media()
{
    engine->media_unregister(this);
}


bool
Media::isLoaded() const
{
    return loaded;
}


bool
Media::isHardwareDependent() const
{
    return hardware;
}


void
Media::load()
{
    if (!loaded)
    {
        loadMedia();
        loaded = true;
    }
}


void
Media::unload()
{
    if (loaded)
    {
        unloadMedia();
        loaded = false;
    }
}


Engine&
Media::getEngine()
{
    Assert(engine, "A null engine is stored, bug?");
    return *engine;
}
