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

    void DrawSprite(Texture texture, glm::vec2 position, glm::vec2 size = glm::vec2(10), GLfloat rotation = 0.f, glm::vec3 colour = glm::vec3(1));

private:
    void InitRenderData();
    Shader shader;
    GLuint quadVAO;

};

#endif //FIRESIM_SPRITERENDERER_H
