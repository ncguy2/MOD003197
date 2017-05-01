//
// Created by Guy on 25/02/2017.
//

#ifndef FIRESIM_OPENGLRENDERER_H
#define FIRESIM_OPENGLRENDERER_H

#include <lib/glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <glm/glm.hpp>
#include <thread>
#include <render/BaseRenderer.h>
#include "Texture.h"
#include "EntityTextureRenderer.h"
#include "SpriteRenderer.h"

#include "FireEffect.h"

class TextureBatch {
public:
    Texture texture;

    std::vector<glm::vec4> positions;
    int layer;

    TextureBatch(Texture texture, int layer)
            : texture(texture), positions(std::vector<glm::vec4>()), layer(layer) {}

    void Flush() {
        positions.clear();
    }
};

void ThreadedForestUpdate(Forest* forest);

class OpenGLRenderer : public BaseRenderer<Texture> {
public:

    OpenGLRenderer();

    void InitWindow(GLuint width, GLuint height, std::string title, glm::vec2 cellSize = glm::vec2(16));
    void Resize(GLuint width, GLuint height);
    void Render(Forest *forest) override;
    void RenderForest(Forest *forest);
    void RenderTextureAtCell(Texture tex, int x, int y, int layer, glm::vec4 offsets = glm::vec4(0, 0, 1, 1));
    void RenderBatches();
    void RenderBatch(TextureBatch* batch, Shader* shdr = nullptr);
    glm::vec2 ProjectCellPositionToScreen(int x, int y);
    void Dispose() override;

    void KeyHandler(int key, int scancode, int action, int mode);
    void KeyPress(int key, int scancode, int mode);
    void KeyRelease(int key, int scancode, int mode);

    glm::mat4 GetProjectionMatrix();

    bool ManageOwnLoop() override;

    GLfloat SecondTimer();
    void SecondTimer(GLfloat seconds);
    GLfloat GetInterval();
    bool ThreadAlive();

    SpriteRenderer* GetSpriteRenderer();
    EntityRenderer<Texture>* GetEntityRenderer();

    glm::vec2 GetCellSize();

protected:

    TextureBatch* GetBatch(Texture tex, int layer);

    std::map<GLuint, TextureBatch*> batches;
    Forest* forest;
    GLuint width, height;
    GLuint forestWidth, forestHeight;
    glm::vec2 cellSize;
    glm::mat4 projectionMatrix;
    GLFWwindow* window;
    bool glewInited = false;
    SpriteRenderer* spriteRenderer;
    bool doTimestep = false;
    bool threadAlive = false;
    GLfloat secondTimer = 0.f;
    FireRenderEffect* fireEffect;
    Shader spriteShader;
    Shader screenShader;
    bool usingScreenShader = false;

    GLuint GenerateAttachmentTexture(GLsizei width, GLsizei height, GLboolean depth = GL_FALSE, GLboolean stencil = GL_FALSE);

    framebuffer::FBO* forestFBO;

private:
    long frameCount = 0;
    long timeElapsed = 0;
    GLfloat interval = .3f;
};

static std::shared_ptr<OpenGLRenderer> newestOpenGLRendererInstance;

#endif //FIRESIM_OPENGLRENDERER_H
