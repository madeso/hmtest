#include "hm/engine.h"

#include "hm/assert.h"
#include "hm/media.h"


void
Engine::media_register(Media* pMedia)
{
    Assert(pMedia, "Need a valid pointer to register");
    mMediaList.push_back(pMedia);
}


void
Engine::media_unregister(Media* pMedia)
{
    Assert(pMedia, "Need a valid pointer to un-register");
    mMediaList.remove(pMedia);
}


void
Engine::unloadHardware()
{
    mLoadState = LS_UNLOADING;
    for (MediaList::iterator mediaIterator = mMediaList.begin();
         mediaIterator != mMediaList.end();
         ++mediaIterator)
    {
        Media* media = *mediaIterator;
        Assert(media, "Internal pointer is not valid, bug?");
        if (media->isHardwareDependent() && media->isLoaded())
        {
            mMediaStack.push(media);
        }
    }
}


void
Engine::loadMedia()
{
    mLoadState = LS_LOADING;
    for (MediaList::iterator mediaIterator = mMediaList.begin();
         mediaIterator != mMediaList.end();
         ++mediaIterator)
    {
        Media* media = *mediaIterator;
        Assert(media, "Internal pointer is not valid, bug?");
        if (!media->isLoaded())
        {
            mMediaStack.push(media);
        }
    }
}



std::size_t
Engine::getLoadCount() const
{
    return mMediaStack.size();
}


void
Engine::processOne()
{
    Assert(!mMediaStack.empty(), "Don't have anything to load, bug?");
    Media* media = mMediaStack.top();
    Assert(media, "Internal pointer isn't valid, probably a bug");
    mMediaStack.pop();
    if (mLoadState == LS_LOADING)
    {
        media->load();
    }
    else if (mLoadState == LS_UNLOADING)
    {
        media->unload();
    }
    else
    {
        Assert(mLoadState == LS_UNDEFINED,
               "Unknown value, state may be valid but unimplemented, consult programer");
        Assert(mLoadState != LS_UNDEFINED,
               "Don't know what to do, called to often?");
    }
    if (mMediaStack.empty())
    {
        mLoadState = LS_UNDEFINED;
    }
}


LoadedImage*
Engine::getLoadedImage(const ImageDescription& pImageDesc)
{
    std::shared_ptr<LoadedImage> image(new LoadedImage(this, pImageDesc));
    mImages.insert(DescriptionImageMap::value_type(pImageDesc, image));
    return image.get();
}



void
Engine::unloadLoadedImage(const ImageDescription& pImageDesc, LoadedImage*)
{
    mImages.erase(mImages.find(pImageDesc));
}







