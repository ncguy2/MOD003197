//
// Created by Guy on 25/02/2017.
//

#ifndef FIRESIM_OPENGLRENDERER_H
#define FIRESIM_OPENGLRENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <render/BaseRenderer.h>
#include "Texture.h"
#include "EntityTextureRenderer.h"
#include "SpriteRenderer.h"

class MetaballController;


class OpenGLRenderer : public BaseRenderer<Texture> {
public:

    OpenGLRenderer();

    void InitWindow(GLuint width, GLuint height, std::string title, glm::vec2 cellSize = glm::vec2(16));
    void Render(Forest *forest) override;
    void RenderForest(Forest *forest);
    void RenderTextureAtCell(Texture tex, int x, int y);
    glm::vec2 ProjectCellPositionToScreen(int x, int y);
    void Dispose() override;

    static std::pair<bool, std::string> Initialize();

    void KeyHandler(int key, int scancode, int action, int mode);
    void KeyPress(int key, int scancode, int mode);
    void KeyRelease(int key, int scancode, int mode);

    glm::mat4 GetProjectionMatrix();

    bool ManageOwnLoop() override;

protected:
    Forest* forest;
    GLuint width, height;
    glm::vec2 cellSize;
    glm::mat4 projectionMatrix;
    GLFWwindow* window;
    bool glewInited = false;
    SpriteRenderer* spriteRenderer;
    MetaballController* metaball;
    bool doTimestep = false;

    GLuint GenerateAttachmentTexture(GLsizei width, GLsizei height, GLboolean depth = GL_FALSE, GLboolean stencil = GL_FALSE);
};

static OpenGLRenderer* newestOpenGLRendererInstance;

#endif //FIRESIM_OPENGLRENDERER_H
