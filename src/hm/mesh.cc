#include "hm/mesh.h"

#include <iostream>
#include <fstream>

#include "hm/dep_gl.h"
#include "hm/vfs.h"
#include "hm/util.h"



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


Face::Face(Index a, Index b, Index c)
{
    indices[0] = a;
    indices[1] = b;
    indices[2] = c;
}




Mesh::Mesh(const std::string& pFile)
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
Mesh::render() const
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
Mesh::renderFace(const Face& face) const
{
    for (int i = 0; i < 3; ++i)
    {
        glNormal3fv(normals[face.indices[i].normal].getArray());
        glTexCoord2fv(tex[face.indices[i].tex].getArray());
        glVertex3fv(vertices[face.indices[i].vertex].getArray());
    }
}
