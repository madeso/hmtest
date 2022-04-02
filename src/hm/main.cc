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

#include "hm/dep_sdl.h"
#include "hm/dep_gl.h"

#include "hm/vec2.h"
#include "hm/vec3.h"
#include "hm/quat.h"
#include "hm/assert.h"
#include "hm/vfs.h"
#include "hm/opengl.h"
#include "hm/texturerenderer.h"
#include "hm/state.h"
#include "hm/statemanager.h"
#include "hm/imagedescription.h"
#include "hm/engine.h"
#include "hm/media.h"
#include "hm/loadedimage.h"
#include "hm/templatemedia.h"
#include "hm/util.h"
#include "hm/image.h"
#include "hm/shader.h"
#include "hm/gridrender.h"
#include "hm/mesh.h"
#include "hm/world.h"
#include "hm/loading.h"


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

    TextureRenderer textureRenderer;
    GridRender grid;

    Shader shader;

    bool light;
};


int
run()
{
    constexpr int width = 800;
    constexpr int height = 600;

    std::cout << "Hello gfx demo\n";

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

    std::cout << "Goodbye heightmap demo\n";
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
                set_base_path(argv[i]);
            }
            else
            {
                std::cerr << "Invalid argument for --root\n";
                return 42;
            }
        }
    }

    if (console)
    {
        return sdlmain();
    }
    else
    {
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
}
