//
// Created by Guy on 25/02/2017.
//


#include <SpriteRenderer.h>

SpriteRenderer::SpriteRenderer(const Shader &shader) : shader(shader) {
    InitRenderData();
}

void SpriteRenderer::DrawSprite(Texture texture, glm::vec2 position, glm::vec2 size, GLfloat rotation, glm::vec3 colour) {
    this->shader.Use();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
    model = glm::rotate(model, rotation, glm::vec3(0.f, 0.f, 1.f));
    model = glm::translate(model, glm::vec3(-.5f * size.x, -.5f * size.y, 0.f));
    model = glm::scale(model, glm::vec3(size, 1.f));

    this->shader.SetMatrix4("model", model);
    this->shader.SetVector3f("spriteColour", colour);

    glActiveTexture(GL_TEXTURE0);
    TextureBinder::GetInstance().Bind(texture);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::InitRenderData() {
    GLuint VBO;
    GLfloat vertices[] = {
            // Pos      // Tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quadVAO);
}
