#pragma once

#include <string>


class Shader
{
public:
    Shader(const std::string& pVertex, const std::string& pFrag);

    void
    bind();

    static void
    unbind();

    // per primitive
    unsigned int
    getUniform(const std::string& pName);

    // per vetex
    unsigned int
    getAttribute(const std::string& pName);

    ~Shader();


protected:
    void
    checkShaderCompilation(unsigned int shader, const std::string& shaderName);

    void
    checkProgram(const std::string& pVertex, const std::string& pFrag);

    const std::string
    getInfoLog(unsigned int object);


private:
    unsigned int vertex;
    unsigned int fragment;
    unsigned int program;
};
