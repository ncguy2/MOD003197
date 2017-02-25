//
// Created by Guy on 25/02/2017.
//

#ifndef FIRESIM_RESOURCEMANAGER_H
#define FIRESIM_RESOURCEMANAGER_H

#include <string>
#include <map>
#include "Texture.h"
#include "Shader.h"

class ResourceManager {
public:
    static ResourceManager& GetInstance() {
        static ResourceManager instance;
        return instance;
    }

    std::map<std::string, Shader> shaders;
    std::map<std::string, Texture> textures;

    Shader LoadShader(std::string vertFile, std::string fragFile, std::string geomFile, std::string name);
    Shader GetShader(std::string name);

    Texture LoadTexture(std::string file, GLboolean alpha, std::string name);
    Texture& GetTexture(std::string name);

    void Clear();

    ResourceManager(ResourceManager const&) = delete;
    void operator=(ResourceManager const&) = delete;

private:
    ResourceManager() {}
    Texture LoadTextureFromFile(std::string file, GLboolean alpha);
    Shader LoadShaderFromFile(std::string vertFile, std::string fragFile, std::string geomFile);
};

#endif //FIRESIM_RESOURCEMANAGER_H
