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

    class FBO {
    public:
        FBO(int width, int height, int attachments);
        // Experiment, no idea how well this will work
        void operator=(bool state);
        void Resize(int width, int height);

        GLuint GetHandle();
        Attachment GetColourAttachment(int index);

    protected:
        GLuint handle;
        std::vector<Attachment> colourAttachments;
        Attachment renderBuffer;
    };

}

#endif //FIRESIM_FRAMEBUFFER_H
