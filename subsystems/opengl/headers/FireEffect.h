//
// Created by Guy on 19/03/2017.
//


#ifndef FIRESIM_FIREEFFECT_H
#define FIRESIM_FIREEFFECT_H

#include <GL/gl.h>
#include <Forest.h>
#include "Framebuffer.h"
#include "Shader.h"
#include "EntityTextureRenderer.h"
#include "SpriteRenderer.h"

class OpenGLRenderer;

class FireRenderEffect {
public:
    FireRenderEffect(int sWidth, int sHeight, OpenGLRenderer* renderer);

    void Extract(Forest* forest);
    void Blur();

    GLuint GetExtractedMask();
    GLuint GetBlurredMask(int index = 0);

protected:
    framebuffer::FBO extractionFBO;
    framebuffer::FBO blurFBO1;
    framebuffer::FBO blurFBO2;

    Shader extractionShader;
    Shader blurShader;

    OpenGLRenderer* renderer;

    int sWidth, sHeight;
};


#endif //FIRESIM_FIREEFFECT_H
