#include "hm/loadedimage.h"

#include <iostream>

#include "hm/engine.h"
#include "hm/imagedescription.h"
#include "hm/dep_gl.h"
#include "hm/vfs.h"

#include "stb_image.h"


LoadedImage::LoadedImage(Engine*, const ImageDescription& pDescription)
{
    int n = 0;
    const auto path = get_base_path() + pDescription.getFile();
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &n, 4);
    if (data == nullptr)
    {
        std::cerr << "failed to load " << pDescription.getFile()
                << " resolved to " << path << "\n";
    }
    unsigned int tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // todo(Gustav): add mimap?
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            data);
    /*
    gluBuild2DMipmaps(
            GL_TEXTURE_2D,
            GL_RGBA,
            width,
            height,
            ilGetInteger(IL_IMAGE_FORMAT),
            GL_UNSIGNED_BYTE,
            ilGetData());
    */
    stbi_image_free(data);
    imageId = tex;
}


LoadedImage::~LoadedImage()
{
    glDeleteTextures(1, &imageId);
}


int
LoadedImage::getId() const
{
    return imageId;
}
