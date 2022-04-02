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
    std::vector<std::string> indexData;
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




class Mesh
{
public:
    explicit Mesh(const std::string& pFile)
    {
        const auto path = get_base_path() + pFile;
        std::ifstream f(path.c_str());
        if (!f.good())
        {
            std::cerr << "Failed to load mesh " << pFile << " resolved to " << path;
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
