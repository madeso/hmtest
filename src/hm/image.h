#pragma once

#include "hm/templatemedia.h"
#include "hm/imagedescription.h"


class LoadedImage;
class Engine;

class Image : public TemplateMedia<LoadedImage>
{
public:
    Image(Engine* pEngine, const ImageDescription& pImageDesc);
    ~Image();


    void
    loadMedia();

    void
    unloadMedia();

    const LoadedImage*
    operator->() const;


private:
    const ImageDescription mImageDesc;
};

