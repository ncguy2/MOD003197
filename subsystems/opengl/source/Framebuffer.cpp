//
// Created by Guy on 19/03/2017.
//

#include <GL/glew.h>
#include <Framebuffer.h>
#include <iostream>

framebuffer::FBO framebuffer::CreateFramebuffer(int width, int height, int attachments) {
    framebuffer::FBO fbo;

    glGenFramebuffers(1, &fbo.handle);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo.handle);

    GLuint texIds[attachments];
    glGenTextures(attachments, texIds);
    for(int i = 0; i < attachments; i++) {
        glBindTexture(GL_TEXTURE_2D, texIds[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texIds[i], 0);

        fbo.colourAttachments.push_back({texIds[i], i});
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    fbo.renderBuffer.attachmentOffset = -1;
    glGenRenderbuffers(1, &fbo.renderBuffer.texHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, fbo.renderBuffer.texHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo.renderBuffer.texHandle);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return fbo;
}

void framebuffer::FBO::operator=(bool state) {
    if(state) glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
    else glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
