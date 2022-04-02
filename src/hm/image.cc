#include "hm/image.h"

#include "hm/engine.h"


Image::Image(Engine* pEngine, const ImageDescription& pImageDesc)
    : TemplateMedia(pEngine, true)
    , mImageDesc(pImageDesc)
{
}


Image::~Image()
{
}

void
Image::loadMedia()
{
    mMedia = getEngine().getLoadedImage(mImageDesc);
}


void
Image::unloadMedia()
{
    Assert(mMedia, "Internal image is null, bug?");
    getEngine().unloadLoadedImage(mImageDesc, mMedia);
    mMedia = 0;
}


const LoadedImage*
Image::operator->() const
{
    return getMedia();
}

