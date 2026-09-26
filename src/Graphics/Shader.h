#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Shader
{
public:
    bool Load(const std::string &vertName, const std::string &fragName);
    void Unload();

    void SetActive();

    void SetMatrixUniform(const char *name, const glm::mat4 &matrix4);
    void SetVectorUniform(const char *name, const glm::vec3 &vector3);
    void SetFloatUniform(const char *name, float value);
    void SetMatrixArrayUniform(const char *name, const std::vector<glm::mat4> &matrix4);

private:
    bool CompileShader(const std::string &fileName, GLenum shaderType, GLuint &outShader);
    bool IsCompiled(GLuint shader);
    bool IsValidProgram();

    GLuint mShaderProgramID;
    GLuint mVertexShaderID;
    GLuint mFragmentShaderID;
};