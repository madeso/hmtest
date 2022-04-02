#pragma once

#include "hm/media.h"
#include "hm/assert.h"


template <class Type>
class TemplateMedia : public Media
{
public:
    TemplateMedia(Engine* pEngine, bool pHardware)
        : Media(pEngine, pHardware)
        , mMedia(0)
    {
    }

    virtual ~TemplateMedia()
    {
    }
    
    const Type*
    getMedia() const
    {
        Assert(isLoaded(), "Media isn't loaded, call load before use");
        Assert(mMedia,
               "Doesn't have a valid type, but the media is loaded - bug?");
        return mMedia;
    }

protected:
    Type* mMedia;
};

