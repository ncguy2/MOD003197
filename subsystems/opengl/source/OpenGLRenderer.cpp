//
// Created by Guy on 25/02/2017.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGLRenderer.h>
#include <ResourceManager.h>
#include <Shaders.h>

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

OpenGLRenderer::OpenGLRenderer() : BaseRenderer("OpenGLRenderer", new EntityTextureRenderer()), spriteRenderer(nullptr), cellSize(glm::vec2(16)) {
    newestOpenGLRendererInstance = this;
}

void OpenGLRenderer::InitWindow(GLuint width, GLuint height, std::string title, glm::vec2 cellSize) {

    this->width = width;
    this->height = height;
    this->cellSize = cellSize;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (this->window == nullptr) {
        this->Dispose();
        return;
    }
    glfwMakeContextCurrent(this->window);

    if (glewInit() != GLEW_OK) {
        this->Dispose();
        return;
    }
    glewInited = true;
//    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mode) {});
    glfwSetKeyCallback(window, glfwKeyCallback);

    glViewport(0, 0, width, height);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    renderer->LateInitialization();

    Shader spriteShader = ResourceManager::GetInstance().GetShader(TEXTURE_SHADER);
    spriteRenderer = new SpriteRenderer(spriteShader);


    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
    spriteShader.Use().SetInteger("image", 0);
    spriteShader.SetMatrix4("projection", projection);

}

void OpenGLRenderer::Render(Forest *forest) {
    this->forest = forest;
    InitWindow(WORLD_SIZE_X * cellSize.x, WORLD_SIZE_Y * cellSize.y, "Fire simulation");
//    InitWindow(800, 600, "Fire simulation");
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        this->RenderForest(forest);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void OpenGLRenderer::Dispose() {
    glfwTerminate();
}

// STATIC

std::pair<bool, std::string> OpenGLRenderer::Initialize() {
    if(glfwInit() == GL_FALSE)
        return std::pair<bool, std::string>(false, utils::ToString(glewGetErrorString(glGetError())));

    return std::pair<bool, std::string>(true, "");
}

void OpenGLRenderer::KeyHandler(int key, int scancode, int action, int mode) {
    if(action == GLFW_PRESS) return KeyPress(key, scancode, mode);
    if(action == GLFW_RELEASE) return KeyRelease(key, scancode, mode);
}

void OpenGLRenderer::KeyPress(int key, int scancode, int mode) {
    if (key == GLFW_KEY_ESCAPE) return glfwSetWindowShouldClose(window, GL_TRUE);
    forest->Update();

}

void OpenGLRenderer::KeyRelease(int key, int scancode, int mode) {

}

void OpenGLRenderer::RenderForest(Forest *forest) {
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            Cell cell = forest->GetCell(x, y);
            RenderTextureAtCell(this->renderer->RenderCell(cell), x, y);
            if(cell.tree->IsAlive())
                RenderTextureAtCell(this->renderer->RenderTree(cell, cell.tree), x, y);
        }
    }
}

void OpenGLRenderer::RenderTextureAtCell(Texture tex, int x, int y) {
    spriteRenderer->DrawSprite(tex, ProjectCellPositionToScreen(x, y), glm::vec2(16));
}

glm::vec2 OpenGLRenderer::ProjectCellPositionToScreen(int x, int y) {
    return glm::vec2(x * cellSize.x, y * cellSize.y);
}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if(newestOpenGLRendererInstance != nullptr) {
        newestOpenGLRendererInstance->KeyHandler(key, scancode, action, mode);
    }
}
