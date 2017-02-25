//
// Created by Guy on 25/02/2017.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGLRenderer.h>
#include <ResourceManager.h>
#include <Shaders.h>
#include <Textures.h>

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

    projectionMatrix = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
    spriteShader.Use().SetInteger("image", 0);
    spriteShader.SetMatrix4("projection", projectionMatrix);

}

void OpenGLRenderer::Render(Forest *forest) {
    this->forest = forest;
    InitWindow(WORLD_SIZE_X * cellSize.x, WORLD_SIZE_Y * cellSize.y, "Fire simulation");
//    InitWindow(800, 600, "Fire simulation");
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    GLfloat secondTimer = 0.f;
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if(doTimestep) {
            secondTimer += deltaTime;
        }
        if(secondTimer >= 0.3) {
            secondTimer = 0;
            forest->Update();
        }

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
    if (key == GLFW_KEY_SPACE) {
        doTimestep = !doTimestep;
        return;
    }
    if(!doTimestep)
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

glm::mat4 OpenGLRenderer::GetProjectionMatrix() {
    return projectionMatrix;
}

void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if(newestOpenGLRendererInstance != nullptr) {
        newestOpenGLRendererInstance->KeyHandler(key, scancode, action, mode);
    }
}

GLuint OpenGLRenderer::GenerateAttachmentTexture(GLsizei width, GLsizei height, GLboolean depth, GLboolean stencil) {
    GLenum type;
    if(!depth && !stencil)
        type = GL_RGB;
    else if(depth && !stencil)
        type = GL_DEPTH_COMPONENT;
    else if(!depth && stencil)
        type = GL_STENCIL_ATTACHMENT;

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    if(!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, NULL);
    else glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texId;
}

/*

 void MetaballController::GenerateFramebuffer(LightingPass &pass, GLsizei width, GLsizei height, std::string shaderName, int attachments) {
    glGenFramebuffers(1, &pass.fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, pass.fbo);
    for(int i = 0; i < attachments; i++) {
        pass.texture = this->GenerateAttachment(width, height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, pass.texture, 0);
    }

    glGenRenderbuffers(1, &pass.rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, pass.rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pass.rbo);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    pass.shader = ResourceManager::GetInstance().GetShader(shaderName);

    glm::mat4 proj = renderHost.GetProjectionMatrix();
    pass.shader.Use();
    pass.shader.SetInteger("image", 0);
    pass.shader.SetMatrix4("projection", proj);
}

GLuint MetaballController::GenerateAttachment(GLsizei width, GLsizei height, GLboolean depth, GLboolean stencil) {
    GLenum type;
    if(!depth && !stencil)
        type = GL_RGB;
    else if(depth && !stencil)
        type = GL_DEPTH_COMPONENT;
    else if(!depth && stencil)
        type = GL_STENCIL_ATTACHMENT;

    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    if(!depth && !stencil)
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, NULL);
    else glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texId;
}

 */