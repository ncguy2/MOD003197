//
// Created by Guy on 25/02/2017.
//

#ifndef FIRESIM_SPRITERENDERER_H
#define FIRESIM_SPRITERENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include "Shader.h"

class SpriteRenderer {
public:
    SpriteRenderer(const Shader &shader);

    virtual ~SpriteRenderer();

    void DrawQuad(glm::vec2 position, glm::vec2 size = glm::vec2(10), GLfloat rotation = 0.f, Shader* shdr = nullptr);
    void DrawSprite(Texture texture, glm::vec2 position, glm::vec2 size = glm::vec2(10), GLfloat rotation = 0.f, glm::vec4 colour = glm::vec4(1), Shader* shdr = nullptr);
    void DrawSprite(GLuint texture, glm::vec2 position, glm::vec2 size = glm::vec2(10), GLfloat rotation = 0.f, glm::vec4 colour = glm::vec4(1), Shader* shdr = nullptr);

private:
    void InitRenderData();
    Shader shader;
    Texture* gpuQuad = nullptr;
    GLuint quadVAO;
};

#endif //FIRESIM_SPRITERENDERER_H
