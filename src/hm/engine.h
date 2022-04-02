#pragma once

#include <list>
#include <stack>
#include <map>
#include <memory>

#include "hm/imagedescription.h"
#include "hm/loadedimage.h"

class Media;

class Engine
{
public:
    void
    media_register(Media* pMedia);

    void
    media_unregister(Media* pMedia);

    void
    unloadHardware();
    
    void
    loadMedia();

    std::size_t
    getLoadCount() const;

    void
    processOne();

    LoadedImage*
    getLoadedImage(const ImageDescription& pImageDesc);

    void
    unloadLoadedImage(const ImageDescription& pImageDesc, LoadedImage* pMedia);

private:
    typedef std::list<Media*> MediaList;
    MediaList mMediaList;
    std::stack<Media*> mMediaStack;
    enum
    {
        LS_UNDEFINED,
        LS_LOADING,
        LS_UNLOADING
    } mLoadState;
    typedef std::map<ImageDescription, std::shared_ptr<LoadedImage>>
            DescriptionImageMap;
    DescriptionImageMap mImages;
};
