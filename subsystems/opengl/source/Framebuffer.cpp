//
// Created by Guy on 19/05/1997 @ 10:25.
//

#include <lib/glad/glad.h>
#include <Framebuffer.h>
#include <iostream>
#include <Utils.h>

framebuffer::FBO::FBO(int width, int height, int attachments) {
    glGenFramebuffers(1, &handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    for(int i = 0; i < attachments; i++) {
        GLuint texId;
        glGenTextures(attachments, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);

        colourAttachments.push_back(Attachment{texId, i});
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    renderBuffer.attachmentOffset = -1;
    glGenRenderbuffers(1, &renderBuffer.texHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer.texHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer.texHandle);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void framebuffer::FBO::operator=(bool state) {
    if(state) glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
    else glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void framebuffer::FBO::Resize(int width, int height) {
    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    for(Attachment attachment : colourAttachments) {
        glBindTexture(GL_TEXTURE_2D, attachment.texHandle);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer.texHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint framebuffer::FBO::GetHandle() {
    return this->handle;
}

framebuffer::Attachment framebuffer::FBO::GetColourAttachment(int index) {
    return colourAttachments[utils::clamp(index, 0, colourAttachments.size()-1)];
}
