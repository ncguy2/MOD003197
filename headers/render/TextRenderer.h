//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_TEXTRENDERER_H
#define FIRESIM_TEXTRENDERER_H

#include "BaseRenderer.h"

class TextRenderer : public BaseRenderer<char> {
public:
    TextRenderer();

    virtual void Render(Forest *forest);
    virtual void RenderDebug(Forest *forest);
};

class EntityTextRenderer : public EntityRenderer<char> {
public:
    char RenderCell(Cell cell);
    char RenderTree(Tree* tree);
};

#endif //FIRESIM_TEXTRENDERER_H
