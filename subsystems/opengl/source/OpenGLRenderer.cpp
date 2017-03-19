//
// Created by Guy on 25/02/2017.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGLRenderer.h>
#include <ResourceManager.h>
#include <Shaders.h>
#include <Textures.h>
#include <sstream>

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

OpenGLRenderer::OpenGLRenderer() : BaseRenderer("OpenGLRenderer", new EntityTextureRenderer()), spriteRenderer(nullptr), cellSize(glm::vec2(16)) {
    newestOpenGLRendererInstance = this;
}

void OpenGLRenderer::InitWindow(GLuint width, GLuint height, std::string title, glm::vec2 cellSize) {

    this->cellSize = cellSize;

    width *= cellSize.x;
    height *= cellSize.y;

    this->width = width;
    this->height = height;

    glfwSetErrorCallback([](int code, const char* msg) {
        std::cerr << "GLFW error " << code << "\t| " << msg << std::endl;
    });

    GLenum glfwInited = glfwInit();
    if(glfwInited == GL_FALSE)
        throw ErrorException{"GLFW Failed to initialize"};

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (this->window == nullptr) {
        this->Dispose();
        throw ErrorException{"GLFW window failed to initialize"};
        return;
    }
    glfwMakeContextCurrent(this->window);

    GLenum init = glewInit();
    if (init != GLEW_OK) {
        this->Dispose();
        std::stringstream ss;
        ss << "GLFW Failed to initialize: ";
        ss << glewGetErrorString(init);
        throw ErrorException{ss.str()};
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

    fireEffect = new FireRenderEffect(width, height, this);

    projectionMatrix = glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
    spriteShader.Use().SetInteger("image", 0);
    spriteShader.SetInteger("mask", 1);
    spriteShader.SetMatrix4("projection", projectionMatrix);


    Shader cloudShader = ResourceManager::GetInstance().GetShader(CLOUD_SHADER);

    cloudShader.Use().SetInteger("image", 0);
    cloudShader.SetMatrix4("projection", projectionMatrix);

}

void OpenGLRenderer::Render(Forest *forest) {
    this->forest = forest;
    InitWindow(WORLD_SIZE_X, WORLD_SIZE_Y, "Fire simulation", glm::vec2(OPENGL_RENDERER_CELL_SIZE));
//    InitWindow(800, 600, "Fire simulation");
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;
    GLfloat fps = -1;
    std::string fpsStr;
    std::string deltaStr;
    threadAlive = true;
    #if OPENGL_USE_THREADS
    std::thread task(ThreadedForestUpdate, forest).detach();
    #endif
    while (!glfwWindowShouldClose(window)) {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        timeElapsed += deltaTime;
        frameCount++;
//        if(timeElapsed > .25) {
            fps = 1 / deltaTime;
//            frameCount = 0;
//            timeElapsed = 0;
//        }
        #if !OPENGL_USE_THREADS
        ThreadedForestUpdate(forest);
        #endif

        cpTOSTRING(fps, fpsStr)
        cpTOSTRING(deltaTime * 1000, deltaStr)

        std::string title = "Fire Simulation | FPS: " + fpsStr + " delta: " + deltaStr;
        glfwSetWindowTitle(this->window, title.c_str());

        if(doTimestep) {
            secondTimer += deltaTime;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        this->RenderForest(forest);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    #if OPENGL_USE_THREADS
    threadAlive = false;
    task.detach();
    #endif
}

void OpenGLRenderer::Dispose() {
    glfwTerminate();
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
    forest->processCommand(key);
    if(!doTimestep)
        forest->Update();
}

void OpenGLRenderer::KeyRelease(int key, int scancode, int mode) {

}

void OpenGLRenderer::RenderForest(Forest *forest) {
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            Cell cell = forest->GetCell(x, y);
            if(cell.states->wall) {
                RenderTextureAtCell(this->renderer->RenderCell(cell), x, y, 2, glm::vec4(0, 0, 0, 0));
            }else{
                RenderTextureAtCell(this->renderer->RenderCell(cell), x, y, 0, glm::vec4(0, 0, 0, 0));
            }
            if(cell.tree->IsAlive()) {
                RenderTextureAtCell(this->renderer->RenderTree(cell, cell.tree), x, y, 1, glm::vec4(0, 0, 6.f, 6.f));
            }
        }
    }

    fireEffect->Extract(forest);
    fireEffect->Blur();

    spriteRenderer->DrawSprite(fireEffect->GetExtractedMask(), {0, 0}, {128, 96});
    spriteRenderer->DrawSprite(fireEffect->GetBlurredMask(1), {0, 0}, {128, 96});
    spriteRenderer->DrawSprite(fireEffect->GetBlurredMask(2), {0, 0}, {128, 96});

    RenderBatches();
}

void OpenGLRenderer::RenderTextureAtCell(Texture tex, int x, int y, int layer, glm::vec4 offsets) {
    TextureBatch* batch = GetBatch(tex, layer);
    batch->positions.push_back(glm::vec4(x + offsets.x, y + offsets.y, offsets.z, offsets.w));
//    spriteRenderer->DrawSprite(tex, ProjectCellPositionToScreen(x, y), cellSize);
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

bool OpenGLRenderer::ManageOwnLoop() {
    return true;
}

void OpenGLRenderer::RenderBatches() {
    for (std::pair<const GLuint, TextureBatch*> batch : batches) {
        if (batch.second->layer != 0) continue;
        this->RenderBatch(batch.second);
    }
//    Shader shader = ResourceManager::GetInstance().GetShader(CLOUD_SHADER);
    for (std::pair<const GLuint, TextureBatch*> batch : batches) {
        if (batch.second->layer != 1) continue;
        this->RenderBatch(batch.second);
    }

    for (std::pair<const GLuint, TextureBatch*> batch : batches) {
        if (batch.second->layer != 2) continue;
        this->RenderBatch(batch.second);
    }

    for (std::pair<const GLuint, TextureBatch*> batch : batches) batch.second->Flush();
}


void OpenGLRenderer::RenderBatch(TextureBatch* batch, Shader* shdr) {
    for(glm::vec4 pos : batch->positions) {
        spriteRenderer->DrawSprite(batch->texture, ProjectCellPositionToScreen(pos.x, pos.y), glm::vec2(cellSize.x + pos.z, cellSize.y + pos.w), 0.f, glm::vec4(1), shdr);
    }
}

TextureBatch* OpenGLRenderer::GetBatch(Texture tex, int layer) {
    for(std::pair<const GLuint, TextureBatch*> batch : batches)
        if(batch.first == tex.id) return batch.second;
    TextureBatch* batch = new TextureBatch(tex, layer);
    batches[tex.id] = batch;
    return batch;
}

SpriteRenderer* OpenGLRenderer::GetSpriteRenderer() {
    return spriteRenderer;
}

EntityRenderer<Texture>* OpenGLRenderer::GetEntityRenderer() {
    return renderer;
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
void ThreadedForestUpdate(Forest *forest) {
    #if OPENGL_USE_THREADS
    while(newestOpenGLRendererInstance->ThreadAlive()) {
    #endif
        if (newestOpenGLRendererInstance->SecondTimer() >= 0.3) {
            newestOpenGLRendererInstance->SecondTimer(0);
            forest->Update();
        }
    #if OPENGL_USE_THREADS
    }
    #endif
}
