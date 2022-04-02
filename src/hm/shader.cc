#include "hm/shader.h"

#include <fstream>
#include <vector>
#include <sstream>

#include "hm/dep_gl.h"
#include "hm/util.h"


Shader::Shader(const std::string& pVertex, const std::string& pFrag)
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
Shader::bind()
{
    glUseProgramObjectARB(program);
}


void
Shader::unbind()
{
    glUseProgramObjectARB(0);
}


// per primitive
unsigned int
Shader::getUniform(const std::string& pName)
{
    unsigned int id = glGetUniformLocationARB(program, pName.c_str());
    return id;
}


// per vetex
unsigned int
Shader::getAttribute(const std::string& pName)
{
    return glGetAttribLocationARB(program, pName.c_str());
}


Shader::~Shader()
{
    glDetachObjectARB(program, vertex);
    glDetachObjectARB(program, fragment);

    glDeleteObjectARB(vertex);
    glDeleteObjectARB(fragment);
    glDeleteObjectARB(program);
}


void
Shader::checkShaderCompilation(unsigned int shader, const std::string& shaderName)
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
Shader::checkProgram(const std::string& pVertex, const std::string& pFrag)
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
Shader::getInfoLog(unsigned int object)
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

