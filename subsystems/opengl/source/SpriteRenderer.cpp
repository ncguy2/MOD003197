//
// Created by Guy on 25/02/2017.
//


#include <SpriteRenderer.h>
#include <ResourceManager.h>
#include <Textures.h>

SpriteRenderer::SpriteRenderer(const Shader &shader) : shader(shader) {
    InitRenderData();
}

void SpriteRenderer::DrawQuad(glm::vec2 position, glm::vec2 size, GLfloat rotation, Shader* shdr) {
    if(gpuQuad == nullptr)
        gpuQuad = &ResourceManager::GetInstance().GetTexture(GPU_TEX);
    this->DrawSprite(*gpuQuad, position, size, rotation, glm::vec4(1), shdr == nullptr ? nullptr : shdr);
}

void SpriteRenderer::DrawSprite(Texture texture, glm::vec2 position, glm::vec2 size, GLfloat rotation, glm::vec4 colour, Shader* shader) {
    if(shader == nullptr) shader = &this->shader;
    shader->Use();
    shader->SetVector4f("spriteColour", colour);

    glActiveTexture(GL_TEXTURE0);
    TextureBinder::GetInstance().Bind(texture);

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
    model = glm::rotate(model, rotation, glm::vec3(0.f, 0.f, 1.f));
    model = glm::translate(model, glm::vec3(-.5f * size.x, -.5f * size.y, 0.f));
    model = glm::scale(model, glm::vec3(size, 1.f));

    shader->SetMatrix4("model", model);

    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::DrawSprite(GLuint texture, glm::vec2 position, glm::vec2 size, GLfloat rotation, glm::vec4 colour, Shader* shader) {
    if(shader == nullptr) shader = &this->shader;
    shader->Use();
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
    model = glm::rotate(model, rotation, glm::vec3(0.f, 0.f, 1.f));
    model = glm::translate(model, glm::vec3(-.5f * size.x, -.5f * size.y, 0.f));
    model = glm::scale(model, glm::vec3(size, 1.f));

    shader->SetMatrix4("model", model);
    shader->SetVector4f("spriteColour", colour);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

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
