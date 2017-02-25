//
// Created by Guy on 25/02/2017.
//

#ifndef FIRESIM_SHADER_H
#define FIRESIM_SHADER_H

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    GLuint ID;
    Shader() {}
    Shader &Use();

    void Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);

    void SetFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
    void SetInteger(const GLchar *name, GLint value, GLboolean useShader = false);
    void SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void SetVector2f(const GLchar *name, glm::vec2 &value, GLboolean useShader = false);
    void SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void SetVector3f(const GLchar *name, glm::vec3 &value, GLboolean useShader = false);
    void SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void SetVector4f(const GLchar *name, glm::vec4 &value, GLboolean useShader = false);
    void SetMatrix4(const GLchar *name, glm::mat4 &matrix, GLboolean useShader = false);

private:
    void CheckCompileErrors(GLuint object, std::string type);
    std::string vertSource;
    std::string fragSource;
    std::string geomSource;
};

static Shader* activeShader = nullptr;

#endif //FIRESIM_SHADER_H
