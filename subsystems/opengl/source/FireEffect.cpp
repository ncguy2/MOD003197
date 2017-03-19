//
// Created by Guy on 19/03/2017.
//

#include <GL/glew.h>
#include <FireEffect.h>
#include <OpenGLRenderer.h>
#include <ResourceManager.h>
#include <Shaders.h>

FireRenderEffect::FireRenderEffect(int sWidth, int sHeight, OpenGLRenderer* renderer) : sWidth(sWidth), sHeight(sHeight), renderer(renderer), blurIterations(OPENGL_RENDERER_FIRE_SHADER_BLURS) {
    extractionFBO = framebuffer::CreateFramebuffer(sWidth, sHeight, 1);
    blurFBO1 = framebuffer::CreateFramebuffer(sWidth, sHeight, 1);
    blurFBO2 = framebuffer::CreateFramebuffer(sWidth, sHeight, 1);

    glm::mat4 proj = renderer->GetProjectionMatrix();

    extractionShader = ResourceManager::GetInstance().GetShader(EXTRACT_SHADER);
    blurShader = ResourceManager::GetInstance().GetShader(BLUR_SHADER);

    extractionShader.Use().SetMatrix4("projection", proj);
    blurShader.Use().SetMatrix4("projection", proj);

    if(blurIterations % 2 != 0)
        blurIterations++;

}

void FireRenderEffect::Extract(Forest* forest) {
    extractionShader.Use();
    glBindFramebuffer(GL_FRAMEBUFFER, extractionFBO->handle);
    glClearColor(1.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(int x = 0; x < forest->worldSizeX; x++) {
        for(int y = 0; y < forest->worldSizeY; y++) {
            Cell cell = forest->GetCell(x, y);
            if(cell.tree->IsBurning()) {
//                this->renderer->RenderTextureAtCell(this->renderer->GetEntityRenderer()->RenderTree(cell, cell.tree), x, y, 0);
                this->renderer->GetSpriteRenderer()->DrawSprite(this->renderer->GetEntityRenderer()->RenderTree(cell, cell.tree),
                                                                this->renderer->ProjectCellPositionToScreen(x, y),
                                                                {10, 10},
                                                                0.f,
                                                                glm::vec4(1.f),
                                                                &extractionShader);
            }
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FireRenderEffect::Blur() {
    blurShader.Use();
    blurShader.SetInteger("u_mask", 0);

    framebuffer::FBO* blurFBOs[2] = {blurFBO1, blurFBO2};

    for(int i = 0; i < blurIterations; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, blurFBOs[i % 2]->handle);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        blurShader.SetInteger("horizontal", i % 2 == 0);
        if(i == 0) {
            renderer->GetSpriteRenderer()->DrawSprite(GetExtractedMask(), {0,0}, {sWidth, sHeight}, 0.f, {1.f, 1.f, 1.f, 1.f}, &blurShader);
        }else{
            renderer->GetSpriteRenderer()->DrawSprite(GetBlurredMask((i + 1) % 2), {0,0}, {sWidth, sHeight}, 0.f, {1.f, 1.f, 1.f, 1.f}, &blurShader);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

GLuint FireRenderEffect::GetExtractedMask() {
    return extractionFBO->colourAttachments[0].texHandle;
}

GLuint FireRenderEffect::GetBlurredMask(int index) {
    if(index == 0)
        return blurFBO1->colourAttachments[0].texHandle;
    return blurFBO2->colourAttachments[0].texHandle;
}

FireRenderEffect::~FireRenderEffect() {
    delete extractionFBO;
    delete blurFBO1;
    delete blurFBO2;
}
