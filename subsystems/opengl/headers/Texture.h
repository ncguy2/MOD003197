//
// Created by Guy on 25/02/2017.
//

#ifndef FIRESIM_TEXTURE_H
#define FIRESIM_TEXTURE_H

#include <lib/glad/glad.h>

struct Texture {

    Texture();

    GLuint id;
    GLuint width, height;
    GLuint internalFormat, imageFormat;
    GLuint wrapS, wrapT;
    GLuint filterMin, filterMag;

    bool operator==(Texture& other) const {
        return id == other.id;
    }

};

class TextureBinder {
public:
    static TextureBinder& GetInstance() {
        static TextureBinder instance;
        return instance;
    }

    void Generate(Texture tex, GLuint width, GLuint height, unsigned char* data);
    void Bind(Texture texture);
    void Bind(GLuint id) const;


protected:
    GLuint currentTexture = (GLuint) -1;
};

#endif //FIRESIM_TEXTURE_H
