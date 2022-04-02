#include <iostream>
#include <stack>
#include <map>
#include <list>
#include <stack>
#include <vector>
#include <sstream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <memory>
#include "stb_image.h"

#include "SDL.h"
#include "GL/glew.h"

#include "vec2.h"
#include "vec3.h"
#include "quat.h"

#define Assert(condition, message) assert((condition) && message)

using namespace std;

std::string base_path;

constexpr bool ONLY_RENDER_WORLD = false;
// #define USE_FBO 1


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


#ifdef USE_FBO
#pragma message("using fbo")
class TextureRenderer
{
public:
    TextureRenderer(int w, int h) : width(w), height(h)
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

    ~TextureRenderer()
    {
        glDeleteFramebuffersEXT(1, &fbo);
        glDeleteRenderbuffersEXT(1, &depth);
        glDeleteTextures(1, &img);
    }

    void
    begin()
    {
        bindFbo();
        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(0, 0, width, height);
    }
    void
    end()
    {
        glPopAttrib();
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }
    void
    bindImage()
    {
        glBindTexture(GL_TEXTURE_2D, img);
    }

protected:
    void
    bindRenderbuffer()
    {
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth);
    }
    void
    bindFbo()
    {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
    }
    void
    test()
    {
        GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
        {
            std::ostringstream str;
            str << "Fbo failed: " << status;
            throw str.str();
        }
    }

private:
    int width;
    int height;

    unsigned int fbo;
    unsigned int depth;
    unsigned int img;
};


#else
#pragma message("using glCopyTexImage")
class TextureRenderer
{
public:
    TextureRenderer(int w, int h) : width(w), height(h)
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

    ~TextureRenderer()
    {
        glDeleteTextures(1, &img);
    }

    void
    begin()
    {
        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(0, 0, width, height);
    }
    void
    end()
    {
        glBindTexture(GL_TEXTURE_2D, img);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, width, height, 0);
        glPopAttrib();
        glClear(GL_COLOR_BUFFER_BIT);
        doCheckOpengl();
    }
    void
    bindImage()
    {
        glBindTexture(GL_TEXTURE_2D, img);
    }

private:
    int width;
    int height;

    //unsigned int fbo;
    //unsigned int depth;
    unsigned int img;
};


#endif
void
rectangle(float x, float y, float w, float h)
{
    glVertex2d(x, y);
    glVertex2d(x, y + h);
    glVertex2d(x + w, y + h);
    glVertex2d(x + w, y);
}


class State
{
public:
    virtual ~State()
    {
    }

    virtual void
    render(int width, int height) = 0;

    virtual bool
    step(float pTime) = 0;

    virtual void
    onEvent(const SDL_Event& pEvent) = 0;
};


class StateManager
{
public:
    void
    addState(State* pState)
    {
        std::shared_ptr<State> state(pState);
        mStack.push(state);
    }
    void
    step(float pTime)
    {
        if (!mStack.empty())
        {
            if (!mStack.top()->step(pTime))
            {
                mStack.pop();
            }
        }
    }
    void
    render(int width, int height)
    {
        if (!mStack.empty())
        {
            mStack.top()->render(width, height);
        }
    }
    void
    onEvent(const SDL_Event& pEvent)
    {
        if (!mStack.empty())
        {
            mStack.top()->onEvent(pEvent);
        }
    }

private:
    std::stack<std::shared_ptr<State>> mStack;
};


class Media;
class LoadedImage;


class ImageDescription
{
public:
    explicit ImageDescription(const std::string& pFileName) : mFile(pFileName)
    {
    }

    const std::string&
    getFile() const
    {
        return mFile;
    }

    bool
    operator<(const ImageDescription& pOther) const
    {
        return mFile < pOther.mFile;
    }

private:
    const std::string mFile;
};


class Engine
{
public:
    void
    media_register(Media* pMedia)
    {
        Assert(pMedia, "Need a valid pointer to register");
        mMediaList.push_back(pMedia);
    }
    void
    media_unregister(Media* pMedia)
    {
        Assert(pMedia, "Need a valid pointer to un-register");
        mMediaList.remove(pMedia);
    }
    void
    unloadHardware();
    void
    loadMedia();
    std::size_t
    getLoadCount() const
    {
        return mMediaStack.size();
    }
    void
    processOne();

    LoadedImage*
    getLoadedImage(const ImageDescription& pImageDesc);
    void
    unloadLoadedImage(const ImageDescription& pImageDesc, LoadedImage* pMedia);

private:
    typedef std::list<Media*> MediaList;
    MediaList mMediaList;
    std::stack<Media*> mMediaStack;
    enum
    {
        LS_UNDEFINED,
        LS_LOADING,
        LS_UNLOADING
    } mLoadState;
    typedef std::map<ImageDescription, std::shared_ptr<LoadedImage>>
            DescriptionImageMap;
    DescriptionImageMap mImages;
};


class Media
{
public:
    Media(Engine* pEngine, bool pHardware)
        : loaded(false)
        , hardware(pHardware)
        , engine(pEngine)
    {
        Assert(pEngine, "Need a valid engine");
        engine->media_register(this);
    }
    virtual ~Media()
    {
        engine->media_unregister(this);
    }

    virtual void
    loadMedia() = 0;
    virtual void
    unloadMedia() = 0;

    bool
    isLoaded() const
    {
        return loaded;
    }
    bool
    isHardwareDependent() const
    {
        return hardware;
    }

    void
    load()
    {
        if (!loaded)
        {
            loadMedia();
            loaded = true;
        }
    }
    void
    unload()
    {
        if (loaded)
        {
            unloadMedia();
            loaded = false;
        }
    }
    Engine&
    getEngine()
    {
        Assert(engine, "A null engine is stored, bug?");
        return *engine;
    }

private:
    bool loaded;
    bool hardware;
    Engine* engine;
};


template <class Type>
class TemplateMedia : public Media
{
public:
    TemplateMedia(Engine* pEngine, bool pHardware)
        : Media(pEngine, pHardware)
        , mMedia(0)
    {
    }
    virtual ~TemplateMedia()
    {
    }
    const Type*
    getMedia() const
    {
        Assert(isLoaded(), "Media isn't loaded, call load before use");
        Assert(mMedia,
               "Doesn't have a valid type, but the media is loaded - bug?");
        return mMedia;
    }

protected:
    Type* mMedia;
};


class LoadedImage
{
public:
    LoadedImage(Engine*, const ImageDescription& pDescription)
    {
        int n = 0;
        const auto path = base_path + pDescription.getFile();
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &n, 4);
        if (data == nullptr)
        {
            cerr << "failed to load " << pDescription.getFile()
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
    ~LoadedImage()
    {
        glDeleteTextures(1, &imageId);
    }
    int
    getId() const
    {
        return imageId;
    }

private:
    unsigned int imageId;
    int width;
    int height;
};


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


class Image : public TemplateMedia<LoadedImage>
{
public:
    Image(Engine* pEngine, const ImageDescription& pImageDesc)
        : TemplateMedia(pEngine, true)
        , mImageDesc(pImageDesc)
    {
    }
    ~Image()
    {
    }

    void
    loadMedia()
    {
        mMedia = getEngine().getLoadedImage(mImageDesc);
    }
    void
    unloadMedia()
    {
        Assert(mMedia, "Internal image is null, bug?");
        getEngine().unloadLoadedImage(mImageDesc, mMedia);
        mMedia = 0;
    }
    const LoadedImage*
    operator->() const
    {
        return getMedia();
    }

private:
    const ImageDescription mImageDesc;
};


std::string
loadFile(const std::string& pFileName)
{
    const auto path = base_path + pFileName;
    std::ifstream file(path.c_str());
    if (file.good())
    {
        std::ostringstream ret;
        ret << file.rdbuf();
        file.close();
        return ret.str();
    }
    else
    {
        std::cerr << "Failed to load " << pFileName << " resolved to " << path
                  << "\n";
    }
    return "";
}


class Shader
{
public:
    Shader(const std::string& pVertex, const std::string& pFrag)
    {
        program = glCreateProgramObjectARB();


        {  // vertex shader
            vertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
            const std::string source(loadFile(pVertex));
            const char* str = source.c_str();
            glShaderSourceARB(vertex, 1, &(str), NULL);
            glCompileShaderARB(vertex);
            checkShaderCompilation(vertex, pVertex);
            glAttachObjectARB(program, vertex);
        }

        {  // fragment shader
            fragment = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
            std::string source(loadFile(pFrag));
            const char* str = source.c_str();
            glShaderSourceARB(fragment, 1, &(str), NULL);
            glCompileShaderARB(fragment);
            checkShaderCompilation(fragment, pFrag);
            glAttachObjectARB(program, fragment);
        }


        glLinkProgramARB(program);
        checkProgram(pVertex, pFrag);
    }

    void
    bind()
    {
        glUseProgramObjectARB(program);
    }
    static void
    unbind()
    {
        glUseProgramObjectARB(0);
    }

    // per primitive
    unsigned int
    getUniform(const std::string& pName)
    {
        unsigned int id = glGetUniformLocationARB(program, pName.c_str());
        return id;
    }

    // per vetex
    unsigned int
    getAttribute(const std::string& pName)
    {
        return glGetAttribLocationARB(program, pName.c_str());
    }

    ~Shader()
    {
        glDetachObjectARB(program, vertex);
        glDetachObjectARB(program, fragment);

        glDeleteObjectARB(vertex);
        glDeleteObjectARB(fragment);
        glDeleteObjectARB(program);
    }

protected:
    void
    checkShaderCompilation(unsigned int shader, const std::string& shaderName)
    {
        int status = 0;
        glGetObjectParameterivARB(
                shader,
                GL_OBJECT_COMPILE_STATUS_ARB,
                &status);
        if (status != 1)
        {
            std::ostringstream message;
            message << "Error when compiling: " << shaderName << ": "
                    << getInfoLog(shader);
            throw message.str();
        }
    }
    void
    checkProgram(const std::string& pVertex, const std::string& pFrag)
    {
        int status = 0;
        glGetObjectParameterivARB(program, GL_OBJECT_LINK_STATUS_ARB, &status);
        if (status != 1)
        {
            std::ostringstream message;
            message << "Error when linking program(" << pVertex << ", " << pFrag
                    << "): " << getInfoLog(program);
            throw message.str();
        }
    }
    const std::string
    getInfoLog(unsigned int object)
    {
        int len = 0;
        glGetObjectParameterivARB(object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &len);
        if (len > 0)
        {
            int read = 0;
            std::vector<char> log;
            log.resize(len);
            glGetInfoLogARB(object, len, &read, &log[0]);
            return std::string(&log[0]);
        }
        else
            return "";
    }

private:
    unsigned int vertex;
    unsigned int fragment;
    unsigned int program;
};


class Cube
{
public:
    Cube(Engine* pEngine, const vec3& pPos, const std::string& pTexture)
        : pos(pPos)
        , image(pEngine, ImageDescription(pTexture))
    {
    }
    void
    render(bool applyTexture)
    {
        if (applyTexture) glBindTexture(GL_TEXTURE_2D, image->getId());
        glPushMatrix();
        glTranslated(pos.getX(), pos.getY(), pos.getZ());
        glBegin(GL_QUADS);
        // Front Face
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);  // Top Left Of The Texture and Quad
        // Back Face
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(
                -1.0f,
                -1.0f,
                -1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
        // Top Face
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);  // Top Right Of The Texture and Quad
        // Bottom Face
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom Right Of The Texture and Quad
        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, -1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);  // Top Left Of The Texture and Quad
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom Left Of The Texture and Quad
        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom Right Of The Texture and Quad
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);  // Top Right Of The Texture and Quad
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Left Of The Texture and Quad
        glEnd();
        glPopMatrix();
    }

private:
    vec3 pos;
    Image image;
};


float
randomSign()
{
    return (rand() % 2 == 0) ? -1.0 : 1.0;
}


float
randomRealWithoutSign()
{
    return float(rand()) / float(RAND_MAX);
}


float
randomReal()
{
    const float sign = randomSign();
    return sign * randomRealWithoutSign();
}


void
edgeVertex(float x, float y)
{
    glTexCoord2d(x, 1 - y);
    glVertex2d(x, y);
}


#define MIDDLE_WIDTH 48
#define MIDDLE_HEIGHT 32

class GridRender
{
public:
    GridRender(TextureRenderer* pRender)
        : renderer(pRender)
        , isEnabled(false)
        , accum(0.0)
    {
        const float widthSpan = 1.0 / (1 + MIDDLE_WIDTH);
        const float widthStart = widthSpan;
        const float heightSpan = 1.0 / (1 + MIDDLE_HEIGHT);
        const float heightStart = heightSpan;
        for (int x = 0; x < MIDDLE_WIDTH; ++x)
        {
            for (int y = 0; y < MIDDLE_HEIGHT; ++y)
            {
                array[x][y].init(
                        vec2(widthStart + x * widthSpan,
                             heightStart + y * heightSpan));
            }
        }
    }

    void
    render()
    {
        setDisplay2d();

        const float widthSpan = 1.0 / (1 + MIDDLE_WIDTH);
        const float heightSpan = 1.0 / (1 + MIDDLE_HEIGHT);
        //glBindTexture(GL_TEXTURE_2D, image->getId());
        renderer->bindImage();
        glBegin(GL_QUADS);
        {
            float y = 0;
            for (int iy = 0; iy <= MIDDLE_HEIGHT; ++iy)
            {
                float x = 0;
                for (int ix = 0; ix <= MIDDLE_WIDTH; ++ix)
                {
                    if (iy == 0)
                    {
                        edgeVertex(x, y);
                        edgeVertex(x + widthSpan, y);
                    }
                    else
                    {
                        if (ix == 0)
                        {
                            edgeVertex(0, y);
                        }
                        else
                        {
                            array[ix - 1][iy - 1].textureVertex();
                        }

                        if (ix == MIDDLE_WIDTH)
                        {
                            edgeVertex(1, y);
                        }
                        else
                        {
                            array[ix][iy - 1].textureVertex();
                        }
                    }

                    if (ix == MIDDLE_WIDTH)
                    {
                        edgeVertex(1, y + heightSpan);
                    }
                    else
                    {
                        if (iy == MIDDLE_HEIGHT)
                        {
                            edgeVertex(x + widthSpan, 1);
                        }
                        else
                        {
                            array[ix][iy].textureVertex();
                        }
                    }

                    if (ix == 0)
                    {
                        edgeVertex(0, y + heightSpan);
                    }
                    else
                    {
                        if (iy == MIDDLE_HEIGHT)
                        {
                            edgeVertex(x, 1);
                        }
                        else
                        {
                            array[ix - 1][iy].textureVertex();
                        }
                    }

                    x += widthSpan;
                }
                y += heightSpan;
            }
        }
        glEnd();
    }

    void
    addRandom()
    {
        for (int x = 0; x < MIDDLE_WIDTH; ++x)
        {
            for (int y = 0; y < MIDDLE_HEIGHT; ++y)
            {
                array[x][y].addForce(vec2(randomReal(), randomReal()));
            }
        }
    }
    void
    bang()
    {
        vec2 center(randomRealWithoutSign(), randomRealWithoutSign());
        float force = 0.1 + randomRealWithoutSign() * 0.05;
        for (int x = 0; x < MIDDLE_WIDTH; ++x)
        {
            for (int y = 0; y < MIDDLE_HEIGHT; ++y)
            {
                array[x][y].bang(center, force, 3);
            }
        }
    }

    void
    step(float pTime)
    {
        if (isEnabled)
        {
            const float lim = 0.05;
            accum += pTime;
            while (accum > lim)
            {
                accum -= lim;
                //addRandom();
                bang();
                //array[rand()%MIDDLE_WIDTH][rand()%MIDDLE_HEIGHT].force( vec2(randomReal(), randomReal()) );
            }
        }

        // const float widthSpan = 1.0 / (1 + MIDDLE_WIDTH);
        // const float heightSpan = 1.0 / (1 + MIDDLE_HEIGHT);
        for (int x = 0; x < MIDDLE_WIDTH; ++x)
        {
            for (int y = 0; y < MIDDLE_HEIGHT; ++y)
            {
                array[x][y].step(
                        pTime * 3,
                        MIDDLE_WIDTH / 5.0,
                        MIDDLE_HEIGHT / 5.0,
                        0.05,
                        5.0);
            }
        }
    }

    void
    setEnable(bool e)
    {
        isEnabled = e;
    }

private:
    TextureRenderer* renderer;
    bool isEnabled;
    struct Dot
    {
        Dot() : pos(0, 0), start(0, 0), vel(0, 0), movement(0, 0)
        {
        }
        void
        bang(const vec2& center, float power, float additionalForce)
        {
            const vec2 direction = pos - center;
            const float length = direction.getLength();
            if (length < power)
            {  // within blast radius
                const float powerScale =
                        (power - length) / power;  // length to outer rim
                // ps range from 0-1 where 1 is center, and 0 outer rim
                const vec2 force = direction.getNormalized() *
                        math::square(math::sin(powerScale * math::PI / 2));
                addForce(force * additionalForce);
            }
        }
        void
        init(const vec2& p)
        {
            start = p;
            pos = p;
            vel = vec2(0, 0);
            movement = vec2(0, 0);
        }
        void
        step(float time, float xlim, float ylim, float maxForce, float maxSpeed)
        {
            const vec2 wantedVel = (start - pos).getTruncated(maxForce);
            const vec2 suggestedVel = wantedVel - movement;
            movement += suggestedVel * time;
            movement.truncate(maxSpeed);

            pos += movement * time;
            pos.setX(math::limitRange(
                    start.getX() - xlim,
                    pos.getX(),
                    start.getX() + xlim));
            pos.setY(math::limitRange(
                    start.getY() - ylim,
                    pos.getY(),
                    start.getY() + ylim));
        }
        void
        stepArrival(
                float time,
                float xlim,
                float ylim,
                float maxForce,
                float maxSpeed)
        {
            // arival behaviour

            const float slowing_distance = 0.01;

            const vec2 target_offset = start - pos;
            const float distance = target_offset.getLength();
            const float ramped_speed = maxSpeed * (distance / slowing_distance);
            const float clipped_speed = math::minimum(ramped_speed, maxSpeed);
            const float ratio = math::equal7(0, distance)
                    ? 0.0
                    : (clipped_speed / distance);
            const vec2 desiredVel = target_offset * ratio;

            //const vec2 desiredVel = (start - pos).getNormalized() * maxSpeed;
            const vec2 steer = desiredVel - vel;
            const vec2 force = steer.getTruncated(maxForce);
            const float weight = 0.01;
            const vec2 acc = force / weight;
            vel = (vel + acc * time).getTruncated(maxSpeed);
            pos += vel * time + movement * time;
            movement -= movement * time;
            pos.setX(math::limitRange(
                    start.getX() - xlim,
                    pos.getX(),
                    start.getX() + xlim));
            pos.setY(math::limitRange(
                    start.getY() - ylim,
                    pos.getY(),
                    start.getY() + ylim));
            /*pos += vel;
			pos.setX( math::limitRange(start.getX()-xlim, pos.getX(), start.getX()+xlim) );
			pos.setY( math::limitRange(start.getY()-ylim, pos.getY(), start.getY()+ylim) );
			const vec2 suggestedVel = start - pos;
			vel += suggestedVel * str;
			vel.setX( math::limitRange(-xlim, vel.getX(), xlim) );
			vel.setY( math::limitRange(-ylim, vel.getY(), ylim) );*/
        }
        void
        addForce(const vec2& p)
        {
            movement += p * 0.1;
        }
        void
        renderLine()
        {
            glVertex2d(start.getX(), start.getY());
            glVertex2d(pos.getX(), pos.getY());
        }
        void
        renderDot()
        {
            glVertex2d(pos.getX(), pos.getY());
        }
        void
        textureVertex()
        {
            glTexCoord2d(pos.getX(), 1 - pos.getY());
            glVertex2d(start.getX(), start.getY());
        }
        vec2 pos;
        vec2 start;
        vec2 vel;
        vec2 movement;
    };

    Dot array[MIDDLE_WIDTH][MIDDLE_HEIGHT];
    float accum;
};


struct Attenuation
{
    Attenuation();

    float constant;
    float linear;
    float quadratic;

    void
    operator()(float c, float l, float q)
    {
        constant = c;
        linear = l;
        quadratic = q;
    }
};


struct CommonLightAttributes
{
    CommonLightAttributes();

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    Attenuation attenuation;
};


CommonLightAttributes::CommonLightAttributes()
    : ambient(1.0f, 1.0f, 1.0f)
    , diffuse(1.0f, 1.0f, 1.0f)
    , specular(0.0f, 0.0f, 0.0f)
{
}


Attenuation::Attenuation() : constant(0.1f), linear(0.1f), quadratic(0.005f)
{
}


struct Material
{
    Material()
        : mAmbient(0.2f, 0.2f, 0.2f)
        , mDiffuse(0.8f, 0.8f, 0.8f)
        , mSpecular(1.0f, 1.0f, 1.0f)
        , mEmissive(0.0f, 0.0f, 0.0f)
        , mShininess(100.0f)
        , mAlpha(1.0f)
    {
    }
    void
    sendToGl()
    {
        //#pragma warning(disable : 4244)
        const float alpha = mAlpha;
        float ambientArray[4] = {
                mAmbient.getX(),
                mAmbient.getY(),
                mAmbient.getZ(),
                alpha};
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambientArray);

        float diffuseArray[4] = {
                mDiffuse.getX(),
                mDiffuse.getY(),
                mDiffuse.getZ(),
                alpha};
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseArray);

        float specularArray[4] = {
                mSpecular.getX(),
                mSpecular.getY(),
                mSpecular.getZ(),
                alpha};
        glMaterialfv(GL_FRONT, GL_SPECULAR, specularArray);

        float emissionArray[4] = {
                mEmissive.getX(),
                mEmissive.getY(),
                mEmissive.getZ(),
                alpha};
        glMaterialfv(GL_FRONT, GL_EMISSION, emissionArray);

        glMaterialf(GL_FRONT, GL_SHININESS, mShininess);
        //#pragma warning(default : 4244)
    }

    vec3 mAmbient;
    vec3 mDiffuse;
    vec3 mSpecular;
    vec3 mEmissive;
    float mShininess;
    float mAlpha;
};


struct Light : public CommonLightAttributes
{
    Light()
        : spot(false)
        , position(0, 0, 0)
        , direction(0, 0, -1)
        , exponent(128)
        , cutoff(30)
    {
    }

    bool spot;
    vec3 position;
    vec3 direction;

    int exponent;
    int cutoff;
};


void
splitString(char delim, const std::string& s, std::vector<std::string>* numbers)
{
    assert(numbers);
    std::stringstream ss(s);
    std::string item;
    auto result = std::back_inserter(*numbers);
    while (std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}


std::string
trim(const std::string& s, const std::string& drop = " ")
{
    std::string r = s;
    r = r.erase(s.find_last_not_of(drop) + 1);
    return r.erase(0, r.find_first_not_of(drop));
}


struct Index
{
    unsigned int vertex;
    unsigned int normal;
    unsigned int tex;
};


int
toInt(const std::string& str)
{
    std::istringstream s(str);
    int r;
    s >> r;
    return r;
}


Index
toIndex(const std::string& str)
{
    vector<std::string> indexData;
    splitString('/', str, &indexData);
    if (indexData.size() != 3)
    {
        throw "missing something in index declearation (mesh)";
    }
    Index index;
    index.vertex = toInt(indexData[0]) - 1;
    index.normal = toInt(indexData[2]) - 1;
    index.tex = toInt(indexData[1]) - 1;
    return index;
}


struct Face
{
    Face(Index a, Index b, Index c)
    {
        indices[0] = a;
        indices[1] = b;
        indices[2] = c;
    }
    Index indices[3];
};


float
toReal(const std::string& str)
{
    std::istringstream s(str);
    float r;
    s >> r;
    return r;
}


class Mesh
{
public:
    explicit Mesh(const std::string& pFile)
    {
        const auto path = base_path + pFile;
        std::ifstream f(path.c_str());
        if (!f.good())
        {
            cerr << "Failed to load mesh " << pFile << " resolved to " << path;
            throw "Failed to load mesh";
        }
        std::string line;
        while (std::getline(f, line))
        {
            std::vector<std::string> commands;
            splitString(' ', trim(line), &commands);
            if (commands.size() > 0)
            {
                const std::string type = trim(commands[0]);
                if (type == "v")
                {
                    vertices.push_back(
                            vec3(toReal(commands[1]),
                                 toReal(commands[2]),
                                 toReal(commands[3])));
                }
                else if (type == "vn")
                {
                    normals.push_back(
                            vec3(toReal(commands[1]),
                                 toReal(commands[2]),
                                 toReal(commands[3])));
                }
                else if (type == "vt")
                {
                    tex.push_back(
                            vec2(toReal(commands[1]), toReal(commands[2])));
                }
                else if (type == "f")
                {
                    faces.push_back(
                            Face(toIndex(commands[1]),
                                 toIndex(commands[2]),
                                 toIndex(commands[3])));
                }
            }
        }
    }

    void
    render() const
    {
        glBegin(GL_TRIANGLES);
        for (std::list<Face>::const_iterator face = faces.begin();
             face != faces.end();
             ++face)
        {
            renderFace(*face);
        }
        glEnd();
    }

    void
    renderFace(const Face& face) const
    {
        for (int i = 0; i < 3; ++i)
        {
            glNormal3fv(normals[face.indices[i].normal].getArray());
            glTexCoord2fv(tex[face.indices[i].tex].getArray());
            glVertex3fv(vertices[face.indices[i].vertex].getArray());
        }
    }

private:
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec2> tex;
    std::list<Face> faces;
};


class World
{
public:
    explicit World(Engine* pEngine)
        : wall(pEngine, ImageDescription("images/wand.bmp"))
        , water(pEngine, ImageDescription("images/water512.jpg"))
        , spotImage(pEngine, ImageDescription("images/spot.png"))
        , hasSpot(false)
        , spotLocation(0, 0, 0)
        , spotDirection(0, 0, -1)
        , shaderPointLight(
                  "shaders/light_point.vert",
                  "shaders/light_point.frag")
        , shaderSpotLight("shaders/light_spot.vert", "shaders/light_spot.frag")
        , mesh("level.obj")
    {
    }

    void
    step(float pTime)
    {
    }

    void
    render()
    {
        if (false)
        {
            drawScene(true, true);
        }
        else
        {
            drawOnDepthBuffer();
            drawAmbient();
            drawLightning();
            drawTextures();
        }
    }
    void
    setSpot(const vec3& pos, const vec3& dir)
    {
        spotLocation = pos;
        spotDirection = dir;
    }
    void
    setSpot(bool enable)
    {
        hasSpot = enable;
    }

protected:
    void
    drawOnDepthBuffer()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        drawScene(false, false);
    }
    void
    setupOpenglLight(const Light& light)
    {
        if (light.spot)
        {
            glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, light.exponent);
            glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, light.cutoff);
        }
        else
        {
            glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, 0);
            glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, 180);
        }

        glLightf(
                GL_LIGHT0,
                GL_CONSTANT_ATTENUATION,
                light.attenuation.constant);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, light.attenuation.linear);
        glLightf(
                GL_LIGHT0,
                GL_QUADRATIC_ATTENUATION,
                light.attenuation.quadratic);

        //#pragma warning(disable : 4244)
        float directionArray[3] = {
                light.direction.getX(),
                light.direction.getY(),
                light.direction.getZ()};
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, directionArray);

        float positionArray[4] = {
                light.position.getX(),
                light.position.getY(),
                light.position.getZ(),
                1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, positionArray);

        float ambientArray[4] = {
                light.ambient.getX(),
                light.ambient.getY(),
                light.ambient.getZ(),
                1.0f};
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientArray);

        float diffuseArray[4] = {
                light.diffuse.getX(),
                light.diffuse.getY(),
                light.diffuse.getZ(),
                1.0f};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseArray);

        float specularArray[4] = {
                light.specular.getX(),
                light.specular.getY(),
                light.specular.getZ(),
                1.0f};
        glLightfv(GL_LIGHT0, GL_SPECULAR, specularArray);
        //#pragma warning(default : 4244)
    }
    void
    drawAmbient()
    {
        glDisable(GL_BLEND);
        glDepthFunc(GL_LEQUAL);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        float ambient = 0.0;
        glColor4d(ambient, ambient, ambient, 1);
        drawScene(false, false);
        glColor4d(1, 1, 1, 1);
    }
    void
    doLight(const Light& light)
    {
        glDisable(GL_BLEND);
        //glClear(GL_STENCIL_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        //glStencilFunc(GL_ALWAYS, 0, 0);

        /*for(allObjects) {
			ComputeShadowVolume();
			glCullFace(GL_FRONT);
			glStencilOp(GL_KEEP, GL_INCR, GL_KEEP);
			DrawShadowVolume();
			glCullFace(GL_BACK);
			glStencilOp(GL_KEEP, GL_DECR, GL_KEEP);
			DrawShadowVolume();
		}*/

        glDepthFunc(GL_LEQUAL);
        //glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        //glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        //glStencilFunc(GL_EQUAL, 0x0, 0xff);

        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT0);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        glEnable(GL_TEXTURE_GEN_R);
        glEnable(GL_TEXTURE_GEN_Q);
        if (light.spot)
        {
            setupOpenglLight(light);
            shaderSpotLight.bind();
            unsigned int spotTexture =
                    shaderSpotLight.getUniform("spotTexture");
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glBindTexture(GL_TEXTURE_2D, spotImage->getId());
            glUniform1iARB(spotTexture, 0);
        }
        else
        {
            setupOpenglLight(light);
            shaderPointLight.bind();
        }
        drawScene(false, true);
        Shader::unbind();
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_R);
        glDisable(GL_TEXTURE_GEN_Q);
        //glDisable(GL_LIGHTING);
    }
    void
    drawLightning()
    {
        glBlendFunc(GL_ONE, GL_ONE);
        glDepthMask(GL_FALSE);
        float ambientArray[4] = {0, 0, 0, 1.0f};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientArray);

        //	glEnable(GL_STENCIL_TEST);
        /*for(allLights)*/ {
            Light light;
            doLight(light);

            if (hasSpot)
            {
                light.position = spotLocation;
                light.spot = true;
                light.direction = spotDirection;
                light.attenuation(0.05f, 0.01f, 0.005f);
                light.cutoff = 20;
                doLight(light);
            }
        }

        //	glDisable(GL_STENCIL_TEST);
    }
    void
    drawTextures()
    {
        glCullFace(GL_BACK);
        glDepthFunc(GL_LEQUAL);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        /*glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		glEnable(GL_TEXTURE_2D);
		drawScene(true, true);*/

        glDepthMask(GL_TRUE);
    }

    void
    drawScene(bool applyTexture, bool doTransparent)
    {
        Material().sendToGl();
        if (applyTexture) glBindTexture(GL_TEXTURE_2D, wall->getId());
        mesh.render();
    }

private:
    Image wall;
    Image water;
    Image spotImage;

    bool hasSpot;
    vec3 spotLocation;
    vec3 spotDirection;

    Shader shaderPointLight;
    Shader shaderSpotLight;

    Mesh mesh;
};


class Game : public State
{
public:
    Game(Engine* pEngine)
        : mCameraPosition(0, 0, 0)
        , mTemporaryCameraRotation(0, 0)
        , in(false)
        , out(false)
        , left(false)
        , right(false)
        , up(false)
        , down(false)
        , world(pEngine)
        , textureRenderer(512, 512)
        , grid(&textureRenderer)
        , shader("shaders/simple.vert", "shaders/simple.frag")
        , light(false)
    {
    }
    void
    render(int width, int height)
    {
        glColor4f(1, 1, 1, 1);
        if (ONLY_RENDER_WORLD)
        {
            renderWorld(width, height);
        }
        else
        {
            textureRenderer.begin();
            renderWorld(width, height);
            textureRenderer.end();

            glDisable(GL_BLEND);
            glDisable(GL_CULL_FACE);
            glEnable(GL_TEXTURE_2D);
            grid.render();
            //textureRenderer.bindImage();
            //renderFullscreenQuad();
        }
    }
    bool
    step(float pTime)
    {
        world.step(pTime);
        grid.step(pTime);
        const float speed = 2 * pTime;
        if (in) mCameraPosition += mCameraRotation.getIn() * speed;
        if (out) mCameraPosition -= mCameraRotation.getIn() * speed;
        if (right) mCameraPosition += mCameraRotation.getRight() * speed;
        if (left) mCameraPosition -= mCameraRotation.getRight() * speed;
        if (up) mCameraPosition += op::vec3::yAxisPositive * speed;
        if (down) mCameraPosition -= op::vec3::yAxisPositive * speed;

        const float sensitivity = 0.005;

        increaseRotation(
                Quat(-mCameraRotation.getRight(),
                     Radian(mTemporaryCameraRotation.getY() * sensitivity)));
        increaseRotation(
                Quat(op::vec3::yAxisNegative,
                     Radian(mTemporaryCameraRotation.getX() * sensitivity)));
        mTemporaryCameraRotation = vec2(0, 0);

        world.setSpot(light);
        if (light)
        {
            vec3 dir = mCameraRotation.getIn();
            //dir.setZ( -dir.getZ() );
            world.setSpot(mCameraPosition, dir);
        }

        return true;
    }
    void
    increaseRotation(const Quat& pRot)
    {
        mCameraRotation = pRot * mCameraRotation;
    }
    void
    onEvent(const SDL_Event& pEvent)
    {
        switch (pEvent.type)
        {
        case SDL_MOUSEMOTION:
            if (mousedown)
            {
                mTemporaryCameraRotation += vec2(pEvent.motion.xrel, 0);
                mTemporaryCameraRotation += vec2(0, pEvent.motion.yrel);
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            if (pEvent.button.button == SDL_BUTTON_LEFT)
            {
                mousedown = pEvent.type == SDL_MOUSEBUTTONDOWN;
            }
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            bool isDown = (pEvent.type == SDL_KEYDOWN);
            switch (pEvent.key.keysym.sym)
            {
            case SDLK_w: in = isDown; break;
            case SDLK_s: out = isDown; break;
            case SDLK_a: left = isDown; break;
            case SDLK_d: right = isDown; break;
            case SDLK_SPACE: up = isDown; break;
            case SDLK_TAB: grid.setEnable(isDown); break;
            case SDLK_q:
                if (isDown)
                {
                    grid.bang();
                }
                break;
            case SDLK_LCTRL: down = isDown; break;
            case SDLK_f:
                if (isDown)
                {
                    light = !light;
                }
                break;
            }
        }
        break;
        }
    }

protected:
    void
    rotateCamera()
    {
        vec3 axis(0, 0, 0);
        Angle angle(0);
        mCameraRotation.getConjugate().toAxisAngle(&axis, &angle);
        glRotated(angle.inDegrees(), axis.getX(), axis.getY(), axis.getZ());
    }
    void
    translateCamera()
    {
        vec3 inverted = -mCameraPosition;
        glTranslated(inverted.getX(), inverted.getY(), inverted.getZ());
    }
    void
    renderWorld(int width, int height)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        setDisplay3d(width, height);
        rotateCamera();
        translateCamera();
        //shader.bind();
        world.render();
        //Shader::unbind();
    }

private:
    Quat mCameraRotation;
    vec3 mCameraPosition;
    vec2 mTemporaryCameraRotation;

    bool in;
    bool out;
    bool left;
    bool right;
    bool up;
    bool down;

    bool mousedown = false;

    World world;

    GridRender grid;

    TextureRenderer textureRenderer;

    Shader shader;

    bool light;
};


class Loading : public State
{
public:
    Loading(Engine* pEngine)
        : mEngine(pEngine)
        , mLoaded(0)
        , mToLoad(0)
        , mProgress(0)
        , mTime(0)
    {
        mEngine->loadMedia();
        mToLoad = mEngine->getLoadCount();
    }

    void
    render(int, int) override
    {
        setDisplay2d();
        glDisable(GL_TEXTURE_2D);
        glColor3d(0.8, 0.8, 0.8);
        glBegin(GL_QUADS);
        rectangle(0.1, 0.8, 0.8, 0.02);
        if (mLoaded > 0)
        {
            glColor3d(0.4, 0.4, 0.4);
            rectangle(0.1, 0.8, 0.8 * mProgress, 0.02);
        }
        glEnd();
        glEnable(GL_TEXTURE_2D);
    }

    bool
    step(float pTime) override
    {
        const float SIZE = 0.1;
        mTime += pTime;
        if (mTime > SIZE)
        {
            mTime -= SIZE;
            mProgress = ((float)mLoaded) / ((float)mToLoad);
            if (mLoaded >= mToLoad) return false;
            mEngine->processOne();
            mLoaded += 1;
            return true;
        }
        return true;
    }

    void
    onEvent(const SDL_Event& pEvent) override
    {
    }

private:
    Engine* mEngine;
    std::size_t mLoaded;
    std::size_t mToLoad;

    float mProgress;
    float mTime;
};


int
run()
{
    constexpr int width = 800;
    constexpr int height = 600;

    cout << "Hello gfx demo\n";

    SDL_Window* win = SDL_CreateWindow(
            "GFX demo",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_OPENGL);
    if (win == nullptr)
    {
        std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext glcontext = SDL_GL_CreateContext(win);
    if (glcontext == nullptr)
    {
        std::cout << "GL context error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_GL_SetSwapInterval(1);


    initOpenGL();

    if (!GLEW_EXT_framebuffer_object)
    {
        throw "FBO not supported by your gfx card, required - sorry";
    }
    if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
    {
        throw "GLSL not supported by your gfx card, required - sorry";
    }

    Engine engine;
    StateManager manager;
    manager.addState(new Game(&engine));
    manager.addState(new Loading(&engine));

    bool running = true;
    auto oldTime = SDL_GetTicks();
    while (running)
    {
        const auto newTime = SDL_GetTicks();
        const auto timeSinceLastFrame = newTime - oldTime;
        oldTime = newTime;
        const float delta = timeSinceLastFrame / 1000.0f;
        manager.step(delta);

        glClear(GL_COLOR_BUFFER_BIT);
        manager.render(width, height);
        SDL_GL_SwapWindow(win);
        // sgl::ProcessAxis();

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
            manager.onEvent(e);
        }
    }

    cout << "Goodbye heightmap demo\n";
    return 0;
}


int
sdlmain()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    try
    {
        const auto r = run();
        SDL_Quit();
        return r;
    }
    catch (const std::string& str)
    {
        std::cerr << "ERROR: " << str << "\n";
        SDL_Quit();
        return 10;
    }
    catch (const char* const str)
    {
        std::cerr << "ERROR: " << str << "\n";
        SDL_Quit();
        return 10;
    }
    catch (...)
    {
        std::cerr << "unknown error\n";
        SDL_Quit();
        return 12;
    }
}

int
main(int argc, char** argv)
{
    bool console = false;

    for (int i = 1; i < argc; i += 1)
    {
        const std::string a = argv[i];
        if (a == "--console")
        {
            console = true;
        }
        else if (a == "--root")
        {
            i += 1;
            if (i < argc)
            {
                base_path = argv[i];
            }
            else
            {
                cerr << "Invalid argument for --root\n";
                return 42;
            }
        }
    }

    if (console)
    {
        return sdlmain();
    }
    std::ofstream out("log.txt");
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());

    std::ofstream eout("error.txt");
    std::streambuf* cerrbuf = std::cerr.rdbuf();
    std::cerr.rdbuf(eout.rdbuf());

    const auto r = sdlmain();

    std::cout.rdbuf(coutbuf);
    std::cerr.rdbuf(cerrbuf);

    return r;
}
