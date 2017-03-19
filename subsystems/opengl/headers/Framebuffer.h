//
// Created by Guy on 19/03/2017.
//

#ifndef FIRESIM_FRAMEBUFFER_H
#define FIRESIM_FRAMEBUFFER_H

#include <GL/gl.h>
#include <vector>

namespace framebuffer {

    struct Attachment {
        GLuint texHandle;
        int attachmentOffset;

    };

    struct FBO {
        GLuint handle;
        std::vector<Attachment> colourAttachments;
        Attachment renderBuffer;

        // Experiment, no idea how well this will work
        void operator=(bool state);
    };

    FBO CreateFramebuffer(int width, int height, int attachments);

}

#endif //FIRESIM_FRAMEBUFFER_H
