//
// Created by Guy on 25/02/2017.
//

#ifndef FIRESIM_ENTITYTEXTURERENDERER_H
#define FIRESIM_ENTITYTEXTURERENDERER_H

#include <render/BaseRenderer.h>
#include "Texture.h"
#include "SpriteRenderer.h"

class EntityTextureRenderer : public EntityRenderer<Texture> {
public:
    EntityTextureRenderer();

    void LateInitialization() override;

    Texture RenderCell(Cell cell);
    Texture RenderTree(Cell cell, Tree *tree);
};

#endif //FIRESIM_ENTITYTEXTURERENDERER_H
