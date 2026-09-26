#include "Shader.h"

#include <fstream>
#include <sstream>

#include <SDL3/SDL.h>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Game.h"

bool Shader::Load(const std::string &vertName, const std::string &fragName)
{
    if (!CompileShader(vertName, GL_VERTEX_SHADER, mVertexShaderID) ||
        !CompileShader(fragName, GL_FRAGMENT_SHADER, mFragmentShaderID))
    {
        return false;
    }

    mShaderProgramID = glCreateProgram();
    glAttachShader(mShaderProgramID, mVertexShaderID);
    glAttachShader(mShaderProgramID, mFragmentShaderID);
    glLinkProgram(mShaderProgramID);

    if (!IsValidProgram())
    {
        return false;
    }

    return true;
}

void Shader::Unload()
{
    glDeleteProgram(mShaderProgramID);
    glDeleteShader(mVertexShaderID);
    glDeleteShader(mFragmentShaderID);
}

bool Shader::CompileShader(const std::string &fileName, GLenum shaderType, GLuint &outShader)
{
    std::ifstream shaderFile(fileName);
    if (shaderFile.is_open())
    {
        std::stringstream sstream;
        sstream << shaderFile.rdbuf();
        std::string contents = sstream.str();
        const char *contentsChar = contents.c_str();

        outShader = glCreateShader(shaderType);
        glShaderSource(outShader, 1, &(contentsChar), nullptr);
        glCompileShader(outShader);

        if (!IsCompiled(outShader))
        {
            SDL_Log("シェーダーのコンパイルに失敗しました: %s", fileName.c_str());
            return false;
        }
    }
    else
    {
        SDL_Log("シェーダーが見つかりませんでした: %s", fileName.c_str());
        return false;
    }

    return true;
}

bool Shader::IsCompiled(GLuint shader)
{
    GLint status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetShaderInfoLog(shader, 511, nullptr, buffer);
        SDL_Log("GLSLコンパイルに失敗しました:\n%s", buffer);
        return false;
    }

    return true;
}

bool Shader::IsValidProgram()
{
    GLint status;

    glGetProgramiv(mShaderProgramID, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        char buffer[512];
        memset(buffer, 0, 512);
        glGetProgramInfoLog(mShaderProgramID, 511, nullptr, buffer);
        SDL_Log("GLSLのリンク状態: \n%s", buffer);
        return false;
    }

    return true;
}

void Shader::SetActive()
{
    glUseProgram(mShaderProgramID);
}

void Shader::SetMatrixUniform(const char *name, const glm::mat4 &matrix4)
{
    GLuint loc = glGetUniformLocation(mShaderProgramID, name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix4));
}

void Shader::SetVectorUniform(const char *name, const glm::vec3 &vector3)
{
    GLuint loc = glGetUniformLocation(mShaderProgramID, name);
    glUniform3fv(loc, 1, glm::value_ptr(vector3));
}

void Shader::SetFloatUniform(const char *name, float value)
{
    GLuint loc = glGetUniformLocation(mShaderProgramID, name);
    glUniform1f(loc, value);
}

void Shader::SetMatrixArrayUniform(const char *name, const std::vector<glm::mat4> &matrices4)
{
    GLuint loc = glGetUniformLocation(mShaderProgramID, name);
    glUniformMatrix4fv(loc, static_cast<GLsizei>(matrices4.size()), GL_FALSE, glm::value_ptr(matrices4.data()[0]));
}