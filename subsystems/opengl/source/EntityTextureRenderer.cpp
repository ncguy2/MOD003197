//
// Created by Guy on 25/02/2017.
//

#include <Texture.h>
#include <Entity.h>
#include <EntityTextureRenderer.h>
#include <ResourceManager.h>
#include <Textures.h>
#include <Shaders.h>

EntityTextureRenderer::EntityTextureRenderer() {
    utils::rng::fast_srand(time(0));
}

Texture EntityTextureRenderer::RenderCell(Cell cell) {
    if(cell.states->wall)
        return ResourceManager::GetInstance().GetTexture(WALL_TEXTURE);
    if(cell.states->damp)
        return ResourceManager::GetInstance().GetTexture(DAMP_TEXTURE);
    if(cell.states->dry)
        return ResourceManager::GetInstance().GetTexture(DRY_TEXTURE);

    return ResourceManager::GetInstance().GetTexture(GROUND_TEXTURE);
}

Texture EntityTextureRenderer::RenderTree(Cell cell, Tree *tree) {
    if(tree->IsBurning()) {
        float p = (utils::rng::fastrand() % 1000) / 1000.f;
        float perc = p * 6;
        std::string key;
        if(perc >= 0 && perc < 1) key = BURNING_1_TEXTURE;
        else if(perc >= 1 && perc < 2) key = BURNING_2_TEXTURE;
        else if(perc >= 2 && perc < 3) key = BURNING_3_TEXTURE;
        else if(perc >= 3 && perc < 4) key = BURNING_4_TEXTURE;
        else if(perc >= 4 && perc < 5) key = BURNING_5_TEXTURE;
        else if(perc >= 5 && perc < 6) key = BURNING_6_TEXTURE;
        return ResourceManager::GetInstance().GetTexture(key);
    }
    if(tree->IsIgnited())
        return ResourceManager::GetInstance().GetTexture(IGNITED_TEXTURE);
    return ResourceManager::GetInstance().GetTexture(TREE_TEXTURE);
}

void EntityTextureRenderer::LateInitialization() {
    ResourceManager::GetInstance().LoadTexture(GROUND_TEXTURE_PATH, GL_TRUE, GROUND_TEXTURE);
    ResourceManager::GetInstance().LoadTexture(DAMP_TEXTURE_PATH, GL_TRUE, DAMP_TEXTURE);
    ResourceManager::GetInstance().LoadTexture(DRY_TEXTURE_PATH, GL_TRUE, DRY_TEXTURE);
    ResourceManager::GetInstance().LoadTexture(WALL_TEXTURE_PATH, GL_TRUE, WALL_TEXTURE);

    ResourceManager::GetInstance().LoadTexture(TREE_TEXTURE_PATH, GL_TRUE, TREE_TEXTURE);
    ResourceManager::GetInstance().LoadTexture(IGNITED_TEXTURE_PATH, GL_TRUE, IGNITED_TEXTURE);
    ResourceManager::GetInstance().LoadTexture(BURNING_1_TEXTURE_PATH, GL_TRUE, BURNING_1_TEXTURE);
    ResourceManager::GetInstance().LoadTexture(BURNING_2_TEXTURE_PATH, GL_TRUE, BURNING_2_TEXTURE);
    ResourceManager::GetInstance().LoadTexture(BURNING_3_TEXTURE_PATH, GL_TRUE, BURNING_3_TEXTURE);
    ResourceManager::GetInstance().LoadTexture(BURNING_4_TEXTURE_PATH, GL_TRUE, BURNING_4_TEXTURE);
    ResourceManager::GetInstance().LoadTexture(BURNING_5_TEXTURE_PATH, GL_TRUE, BURNING_5_TEXTURE);
    ResourceManager::GetInstance().LoadTexture(BURNING_6_TEXTURE_PATH, GL_TRUE, BURNING_6_TEXTURE);

    ResourceManager::GetInstance().LoadTexture(GPU_TEX_PATH, GL_TRUE, GPU_TEX);

    ResourceManager::GetInstance().LoadShader(TEXTURE_SHADER_VERT, TEXTURE_SHADER_FRAG, "", TEXTURE_SHADER);
    ResourceManager::GetInstance().LoadShader(CLOUD_SHADER_VERT, CLOUD_SHADER_FRAG, "", CLOUD_SHADER);
}
