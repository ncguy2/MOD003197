//
// Created by Guy on 25/02/2017.
//

#include <Texture.h>
#include <string>
#include <ResourceManager.h>
#include <SOIL/SOIL.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader ResourceManager::LoadShader(std::string vertFile, std::string fragFile, std::string geomFile, std::string name) {
    shaders[name] = LoadShaderFromFile(vertFile, fragFile, geomFile);
    return shaders[name];
}

Shader ResourceManager::GetShader(std::string name) {
    return shaders[name];
}

Texture ResourceManager::LoadTexture(std::string file, GLboolean alpha, std::string name) {
    textures[name] = LoadTextureFromFile(file, alpha);
    return textures[name];
}

Texture &ResourceManager::GetTexture(std::string name) {
    return textures[name];
}

Texture ResourceManager::LoadTextureFromFile(std::string file, GLboolean alpha) {
    Texture tex = Texture();
    if(alpha) {
        tex.internalFormat = GL_RGBA;
        tex.imageFormat = GL_RGBA;
    }
    int w, h;
    unsigned char* image = SOIL_load_image(file.c_str(), &w, &h, 0, tex.imageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    TextureBinder::GetInstance().Generate(tex, w, h, image);
    SOIL_free_image_data(image);
    return tex;
}

Shader ResourceManager::LoadShaderFromFile(std::string vertFile, std::string fragFile, std::string geomFile) {
// 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // Open files
        std::ifstream vertexShaderFile(vertFile);
        std::ifstream fragmentShaderFile(fragFile);
        std::stringstream vShaderStream, fShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // If geometry shader path is present, also load a geometry shader
        if (!geomFile.empty()) {
            std::ifstream geometryShaderFile(geomFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, !geomFile.empty() ? gShaderCode : nullptr);
    return shader;
}

void ResourceManager::Clear() {
    for (auto iter : shaders)
        glDeleteProgram(iter.second.ID);
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.id);
}

