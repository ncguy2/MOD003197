//
// Created by Guy on 25/02/2017.
//

#ifndef FIRESIM_TEXTURE_H
#define FIRESIM_TEXTURE_H

#include <GL/glew.h>

struct Texture {

    Texture();

    GLuint id;
    GLuint width, height;
    GLuint internalFormat, imageFormat;
    GLuint wrapS, wrapT;
    GLuint filterMin, filterMag;
};

class TextureBinder {
public:
    static TextureBinder& GetInstance() {
        static TextureBinder instance;
        return instance;
    }

    void Generate(Texture tex, GLuint width, GLuint height, unsigned char* data);
    void Bind(Texture texture) const;
};

#endif //FIRESIM_TEXTURE_H