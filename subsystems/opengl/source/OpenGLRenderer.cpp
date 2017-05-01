//
// Created by Guy on 25/02/2017.
//

#include <OpenGLRenderer.h>
#include <ResourceManager.h>
#include <Shaders.h>
#include <Textures.h>
#include <sstream>

#ifndef ASFLOAT
#define ASFLOAT(var) ((float)var)
#endif


void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfwResizeCallback(GLFWwindow* window, int width, int height);

OpenGLRenderer::OpenGLRenderer() : BaseRenderer("OpenGLRenderer", new EntityTextureRenderer()), spriteRenderer(nullptr), cellSize(glm::vec2(16)) {
    newestOpenGLRendererInstance = std::shared_ptr<OpenGLRenderer>(this);
}

void OpenGLRenderer::InitWindow(GLuint width, GLuint height, std::string title, glm::vec2 cellSize) {

    this->cellSize = cellSize;

    this->forestWidth = width;
    this->forestHeight = height;

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
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (this->window == nullptr) {
        this->Dispose();
        throw ErrorException{"GLFW window failed to initialize"};
    }
    glfwMakeContextCurrent(this->window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        throw ErrorException{"Failed to initialize OpenGL context"};
    glewInited = true;
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetWindowSizeCallback(window, glfwResizeCallback);

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
    this->spriteShader = spriteShader;


    if(OPENGL_RENDERER_USE_FIRE_SHADER) {
        Shader screenShader = ResourceManager::GetInstance().GetShader(SCREEN_SHADER);
        screenShader.Use().SetInteger("image", 0);
        screenShader.SetInteger("blur", 1);
        screenShader.SetMatrix4("projection", projectionMatrix);
        screenShader.SetFloat("fireScale", OPENGL_RENDERER_FIRE_SHADER_SCALE);
        this->screenShader = screenShader;
        this->usingScreenShader = true;

        fireEffect = new FireRenderEffect(width, height, this);

        forestFBO = new framebuffer::FBO(width, height, 1);
    }
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
    std::string intervalStr;
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
        cpTOSTRING(interval * 1000, intervalStr)

        std::string title = "Fire Simulation | FPS: " + fpsStr + " delta: " + deltaStr + " interval: " + intervalStr;
        glfwSetWindowTitle(this->window, title.c_str());

        if(doTimestep) {
            secondTimer += deltaTime;
        }

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
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
    if(usingScreenShader) delete fireEffect;
    delete forestFBO;
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

    if(key == GLFW_KEY_X && interval < 10.f)
        interval += .05f;
    if(key == GLFW_KEY_Z && interval > 0.f)
        interval -= .05f;

    forest->processCommand(key);
    if(!doTimestep)
        forest->Update();
}

void OpenGLRenderer::KeyRelease(int key, int scancode, int mode) {

}

void OpenGLRenderer::RenderForest(Forest *forest) {
    if(usingScreenShader) {
        glBindFramebuffer(GL_FRAMEBUFFER, forestFBO->GetHandle());
        glClearColor(0.f, 1.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    for(int x = 0; x < forest->worldSizeX; x++) {
        for(int y = 0; y < forest->worldSizeY; y++) {
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
    RenderBatches();
    if(usingScreenShader) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        fireEffect->Extract(forest);
        fireEffect->Blur();

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, fireEffect->GetBlurredMask(1));
        spriteRenderer->DrawSprite(forestFBO->GetColourAttachment(0).texHandle, {0, 0}, {width, height}, 0.f,
                                   {1.f, 1.f, 1.f, 1.f}, &screenShader);
    }

//    spriteRenderer->DrawSprite(fireEffect->GetExtractedMask(), {0, 0}, {256, 192});
//    spriteRenderer->DrawSprite(fireEffect->GetBlurredMask(0), {0, 192}, {256, 192});
//    spriteRenderer->DrawSprite(fireEffect->GetBlurredMask(1), {256, 192}, {256, 192});

}

void OpenGLRenderer::RenderTextureAtCell(Texture tex, int x, int y, int layer, glm::vec4 offsets) {
    TextureBatch* batch = GetBatch(tex, layer);
    batch->positions.push_back(glm::vec4(x + offsets.x, y + offsets.y, offsets.z, offsets.w));
//    spriteRenderer->DrawSprite(tex, ProjectCellPositionToScreen(x, y), cellSize);
}

glm::vec2 OpenGLRenderer::ProjectCellPositionToScreen(int x, int y) {
    return glm::vec2(ASFLOAT(x) * cellSize.x, ASFLOAT(y) * cellSize.y);
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

GLfloat OpenGLRenderer::SecondTimer() {
    return secondTimer;
}

void OpenGLRenderer::SecondTimer(GLfloat seconds) {
    secondTimer = seconds;
}

bool OpenGLRenderer::ThreadAlive() {
    return threadAlive;
}

SpriteRenderer* OpenGLRenderer::GetSpriteRenderer() {
    return spriteRenderer;
}

EntityRenderer<Texture>* OpenGLRenderer::GetEntityRenderer() {
    return renderer;
}

void OpenGLRenderer::Resize(GLuint width, GLuint height) {
    this->width = width;
    this->height = height;
    this->cellSize.x = ASFLOAT(width)  / ASFLOAT(this->forestWidth);
    this->cellSize.y = ASFLOAT(height) / ASFLOAT(this->forestHeight);

    projectionMatrix = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);

    forestFBO->Resize(width, height);
    fireEffect->Resize(width, height);

    this->spriteShader.Use().SetMatrix4("projection", projectionMatrix);
    if(usingScreenShader)
        this->screenShader.Use().SetMatrix4("projection", projectionMatrix);
    glViewport(0, 0, width, height);
}

glm::vec2 OpenGLRenderer::GetCellSize() {
    return cellSize;
}

GLfloat OpenGLRenderer::GetInterval() {
    return interval;
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
        if (newestOpenGLRendererInstance->SecondTimer() >= newestOpenGLRendererInstance->GetInterval()) {
            newestOpenGLRendererInstance->SecondTimer(0);
            forest->Update();
        }
    #if OPENGL_USE_THREADS
    }
    #endif
}

void glfwResizeCallback(GLFWwindow* window, int width, int height) {
    if(newestOpenGLRendererInstance != nullptr)
        newestOpenGLRendererInstance->Resize(width, height);
}
