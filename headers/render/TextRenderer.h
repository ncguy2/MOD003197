//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_TEXTRENDERER_H
#define FIRESIM_TEXTRENDERER_H

#include "BaseRenderer.h"

class TextRenderer : public BaseRenderer<std::string> {
public:
    TextRenderer();

    void Render(Forest *forest);
    void RenderDebug(Forest *forest);
    std::vector<std::string> GetDebugLines(Forest *forest);

    void Dispose() override;

    bool ManageOwnLoop() override;

    int GetCellForeground(Cell cell);
    int GetCellBackground(Cell cell);
};

class EntityTextRenderer : public EntityRenderer<std::string> {
public:
    void LateInitialization() override;

    std::string RenderCell(Cell cell);
    std::string RenderTree(Cell cell, Tree* tree);
};

#endif //FIRESIM_TEXTRENDERER_H
