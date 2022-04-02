#include "hm/opengl.h"

#include <iostream>

#include "hm/dep_gl.h"

void
doCheckOpengl()
{
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::string value = "???";
#define ERROR_VALUE(x)                                                         \
    case x: value = #x; break
        switch (error)
        {
            ERROR_VALUE(GL_INVALID_ENUM);
            ERROR_VALUE(GL_INVALID_VALUE);
            ERROR_VALUE(GL_INVALID_OPERATION);
            ERROR_VALUE(GL_STACK_OVERFLOW);
            ERROR_VALUE(GL_STACK_UNDERFLOW);
            ERROR_VALUE(GL_OUT_OF_MEMORY);
        }
#undef ERROR_VALUE
        std::cerr << "Opengl error: " << error << ": " << value << std::endl;
    }
}


void
initOpenGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1, 1, 0);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 1, 1);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_BLEND);

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const auto glew_init = glewInit();
    if (GLEW_OK != glew_init)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Error: " << glewGetErrorString(glew_init);
        return;
    }

    doCheckOpengl();
}


void
setDisplay2d()
{
    // setup 2d
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1, 1, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    doCheckOpengl();
}


void
renderVertex(float x, float y)
{
    glTexCoord2d(x, 1 - y);
    glVertex2d(x, y);
}


void
renderFullscreenQuad()
{
    constexpr float min = 0;
    constexpr float max = 1 - min;
    setDisplay2d();
    glBegin(GL_QUADS);
    renderVertex(min, min);
    renderVertex(min, max);
    renderVertex(max, max);
    renderVertex(max, min);
    glEnd();

    doCheckOpengl();
}


void
setDisplay3d(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // http://www.sjbaker.org/steve/omniv/love_your_z_buffer.html
    gluPerspective(45.0f, float(width) / float(height), 0.5f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    doCheckOpengl();
}


void
rectangle(float x, float y, float w, float h)
{
    glVertex2d(x, y);
    glVertex2d(x, y + h);
    glVertex2d(x + w, y + h);
    glVertex2d(x + w, y);
}
