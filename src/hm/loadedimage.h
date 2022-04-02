#pragma once

class Engine;
class ImageDescription;


class LoadedImage
{
public:
    LoadedImage(Engine* pEngine, const ImageDescription& pDescription);

    ~LoadedImage();

    int
    getId() const;


private:
    unsigned int imageId;
    int width;
    int height;
};

