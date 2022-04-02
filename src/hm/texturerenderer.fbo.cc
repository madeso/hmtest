#include "hm/texturerenderer.fbo.h"

#include <sstream>

#include "hm/dep_gl.h"
#include "hm/opengl.h"


TextureRendererFbo::TextureRendererFbo(int w, int h) : width(w), height(h)
{
    glGenFramebuffersEXT(1, &fbo);
    bindFbo();
    glGenRenderbuffersEXT(1, &depth);
    bindRenderbuffer();
    glRenderbufferStorageEXT(
            GL_RENDERBUFFER_EXT,
            GL_DEPTH_COMPONENT,
            width,
            height);
    glFramebufferRenderbufferEXT(
            GL_FRAMEBUFFER_EXT,
            GL_DEPTH_ATTACHMENT_EXT,
            GL_RENDERBUFFER_EXT,
            depth);

    // build texture
    glGenTextures(1, &img);
    glBindTexture(GL_TEXTURE_2D, img);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    doCheckOpengl();

    glFramebufferTexture2DEXT(
            GL_FRAMEBUFFER_EXT,
            GL_COLOR_ATTACHMENT0_EXT,
            GL_TEXTURE_2D,
            img,
            0);

    test();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

TextureRendererFbo::~TextureRendererFbo()
{
    glDeleteFramebuffersEXT(1, &fbo);
    glDeleteRenderbuffersEXT(1, &depth);
    glDeleteTextures(1, &img);
}

void
TextureRendererFbo::begin()
{
    bindFbo();
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(0, 0, width, height);
}


void
TextureRendererFbo::end()
{
    glPopAttrib();
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}


void
TextureRendererFbo::bindImage()
{
    glBindTexture(GL_TEXTURE_2D, img);
}

void
TextureRendererFbo::bindRenderbuffer()
{
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth);
}


void
TextureRendererFbo::bindFbo()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
}


void
TextureRendererFbo::test()
{
    GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
    {
        std::ostringstream str;
        str << "Fbo failed: " << status;
        throw str.str();
    }
}
