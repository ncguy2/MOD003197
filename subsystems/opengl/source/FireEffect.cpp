//
// Created by Guy on 19/03/2017.
//


#include <GL/glew.h>
#include <FireEffect.h>
#include <OpenGLRenderer.h>
#include <ResourceManager.h>
#include <Shaders.h>

FireRenderEffect::FireRenderEffect(int sWidth, int sHeight, OpenGLRenderer* renderer) : sWidth(sWidth), sHeight(sHeight), renderer(renderer) {
    extractionFBO = framebuffer::CreateFramebuffer(sWidth, sHeight, 1);
    blurFBO1 = framebuffer::CreateFramebuffer(sWidth, sHeight, 1);
    blurFBO2 = framebuffer::CreateFramebuffer(sWidth, sHeight, 1);

    extractionShader = ResourceManager::GetInstance().GetShader(EXTRACT_SHADER);
    blurShader = ResourceManager::GetInstance().GetShader(BLUR_SHADER);
}

void FireRenderEffect::Extract(Forest* forest) {
    extractionFBO = true;
    extractionShader.Use();
    glm::mat4 proj = renderer->GetProjectionMatrix();
    extractionShader.SetMatrix4("projection", proj);

    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            Cell cell = forest->GetCell(x, y);
            if(cell.tree->IsBurning())
                this->renderer->RenderTextureAtCell(this->renderer->GetEntityRenderer()->RenderTree(cell, cell.tree), x, y);
        }
    }
    extractionFBO = false;
}

void FireRenderEffect::Blur() {
    blurShader.Use();
    blurShader.SetInteger("u_mask", 0);
    glm::mat4 proj = renderer->GetProjectionMatrix();
    blurShader.SetMatrix4("projection", proj);

    framebuffer::FBO blurFBOs[2] = {blurFBO1, blurFBO2};

    for(int i = 0; i < 2; i++) {
        blurFBOs[i % 2] = true;
        blurShader.SetInteger("horizontal", i % 2 == 0);
        if(i == 0){
            renderer->GetSpriteRenderer()->DrawSprite(GetExtractedMask(), {0,0}, {sWidth, sHeight}, 0.f, {1.f, 1.f, 1.f, 1.f}, &blurShader);
        }else{
            renderer->GetSpriteRenderer()->DrawSprite(GetBlurredMask((i + 1) % 2), {0,0}, {sWidth, sHeight}, 0.f, {1.f, 1.f, 1.f, 1.f}, &blurShader);
        }
        blurFBOs[i % 2] = false;
    }

}

GLuint FireRenderEffect::GetExtractedMask() {
    return extractionFBO.handle;
}

GLuint FireRenderEffect::GetBlurredMask(int index) {
    if(index == 1)
        return blurFBO1.handle;
    return blurFBO2.handle;
}
