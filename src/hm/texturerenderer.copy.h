#pragma once


class TextureRendererCopy
{
public:
    TextureRendererCopy(int w, int h);
    ~TextureRendererCopy();

    void
    begin();

    void
    end();

    void
    bindImage();

private:
    int width;
    int height;

    unsigned int img;
};
