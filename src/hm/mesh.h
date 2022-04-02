#pragma once

#include <list>
#include <vector>
#include <string>

#include "hm/vec2.h"
#include "hm/vec3.h"


struct Index
{
    unsigned int vertex;
    unsigned int normal;
    unsigned int tex;
};

struct Face
{
    Face(Index a, Index b, Index c);

    Index indices[3];
};


class Mesh
{
public:
    explicit Mesh(const std::string& pFile);

    void
    render() const;

    void
    renderFace(const Face& face) const;

private:
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec2> tex;
    std::list<Face> faces;
};

