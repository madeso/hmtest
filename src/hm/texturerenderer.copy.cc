#include "hm/texturerenderer.copy.h"

#include "hm/dep_gl.h"
#include "hm/opengl.h"

TextureRendererCopy::TextureRendererCopy(int w, int h) : width(w), height(h)
{
    // build texture
    glGenTextures(1, &img);
    glBindTexture(GL_TEXTURE_2D, img);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            width,
            height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    doCheckOpengl();
}

TextureRendererCopy::~TextureRendererCopy()
{
    glDeleteTextures(1, &img);
}

void
TextureRendererCopy::begin()
{
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, width, height);
}


void
TextureRendererCopy::end()
{
    glBindTexture(GL_TEXTURE_2D, img);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, width, height, 0);
    glPopAttrib();
    glClear(GL_COLOR_BUFFER_BIT);
    doCheckOpengl();
}


void
TextureRendererCopy::bindImage()
{
    glBindTexture(GL_TEXTURE_2D, img);
}

