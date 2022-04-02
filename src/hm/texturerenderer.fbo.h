#pragma once

class TextureRendererFbo
{
public:
    TextureRendererFbo(int w, int h);
    ~TextureRendererFbo();

    void
    begin();

    void
    end();

    void
    bindImage();


protected:
    void
    bindRenderbuffer();

    void
    bindFbo();

    void
    test();


private:
    int width;
    int height;

    unsigned int fbo;
    unsigned int depth;
    unsigned int img;
};
