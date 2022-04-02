#include "hm/gridrender.h"

#include "hm/dep_gl.h"
#include "hm/opengl.h"
#include "hm/util.h"


void
edgeVertex(float x, float y)
{
    glTexCoord2d(x, 1 - y);
    glVertex2d(x, y);
}


namespace detail
{

    Dot::Dot() : pos(0, 0), start(0, 0), vel(0, 0), movement(0, 0)
    {
    }


    void
    Dot::bang(const vec2& center, float power, float additionalForce)
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
    Dot::init(const vec2& p)
    {
        start = p;
        pos = p;
        vel = vec2(0, 0);
        movement = vec2(0, 0);
    }


    void
    Dot::step(float time, float xlim, float ylim, float maxForce, float maxSpeed)
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
    Dot::stepArrival(
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
    Dot::addForce(const vec2& p)
    {
        movement += p * 0.1;
    }


    void
    Dot::renderLine()
    {
        glVertex2d(start.getX(), start.getY());
        glVertex2d(pos.getX(), pos.getY());
    }


    void
    Dot::renderDot()
    {
        glVertex2d(pos.getX(), pos.getY());
    }


    void
    Dot::textureVertex()
    {
        glTexCoord2d(pos.getX(), 1 - pos.getY());
        glVertex2d(start.getX(), start.getY());
    }
}
        

GridRender::GridRender(TextureRenderer* pRender)
    : renderer(pRender)
    , isEnabled(false)
    , accum(0.0)
{
    const float widthSpan = 1.0 / (1 + detail::MIDDLE_WIDTH);
    const float widthStart = widthSpan;
    const float heightSpan = 1.0 / (1 + detail::MIDDLE_HEIGHT);
    const float heightStart = heightSpan;
    for (int x = 0; x < detail::MIDDLE_WIDTH; ++x)
    {
        for (int y = 0; y < detail::MIDDLE_HEIGHT; ++y)
        {
            array[x][y].init(
                    vec2(widthStart + x * widthSpan,
                            heightStart + y * heightSpan));
        }
    }
}

void
GridRender::render()
{
    setDisplay2d();

    const float widthSpan = 1.0 / (1 + detail::MIDDLE_WIDTH);
    const float heightSpan = 1.0 / (1 + detail::MIDDLE_HEIGHT);
    //glBindTexture(GL_TEXTURE_2D, image->getId());
    renderer->bindImage();
    glBegin(GL_QUADS);
    {
        float y = 0;
        for (int iy = 0; iy <= detail::MIDDLE_HEIGHT; ++iy)
        {
            float x = 0;
            for (int ix = 0; ix <= detail::MIDDLE_WIDTH; ++ix)
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

                    if (ix == detail::MIDDLE_WIDTH)
                    {
                        edgeVertex(1, y);
                    }
                    else
                    {
                        array[ix][iy - 1].textureVertex();
                    }
                }

                if (ix == detail::MIDDLE_WIDTH)
                {
                    edgeVertex(1, y + heightSpan);
                }
                else
                {
                    if (iy == detail::MIDDLE_HEIGHT)
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
                    if (iy == detail::MIDDLE_HEIGHT)
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
GridRender::addRandom()
{
    for (int x = 0; x < detail::MIDDLE_WIDTH; ++x)
    {
        for (int y = 0; y < detail::MIDDLE_HEIGHT; ++y)
        {
            array[x][y].addForce(vec2(randomReal(), randomReal()));
        }
    }
}


void
GridRender::bang()
{
    vec2 center(randomRealWithoutSign(), randomRealWithoutSign());
    float force = 0.1 + randomRealWithoutSign() * 0.05;
    for (int x = 0; x < detail::MIDDLE_WIDTH; ++x)
    {
        for (int y = 0; y < detail::MIDDLE_HEIGHT; ++y)
        {
            array[x][y].bang(center, force, 3);
        }
    }
}


void
GridRender::step(float pTime)
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
            //array[rand()%detail::MIDDLE_WIDTH][rand()%detail::MIDDLE_HEIGHT].force( vec2(randomReal(), randomReal()) );
        }
    }

    // const float widthSpan = 1.0 / (1 + detail::MIDDLE_WIDTH);
    // const float heightSpan = 1.0 / (1 + detail::MIDDLE_HEIGHT);
    for (int x = 0; x < detail::MIDDLE_WIDTH; ++x)
    {
        for (int y = 0; y < detail::MIDDLE_HEIGHT; ++y)
        {
            array[x][y].step(
                    pTime * 3,
                    detail::MIDDLE_WIDTH / 5.0,
                    detail::MIDDLE_HEIGHT / 5.0,
                    0.05,
                    5.0);
        }
    }
}


void
GridRender::setEnable(bool e)
{
    isEnabled = e;
}

