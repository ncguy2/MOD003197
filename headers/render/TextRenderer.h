//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_TEXTRENDERER_H
#define FIRESIM_TEXTRENDERER_H

#include "BaseRenderer.h"

class TextRenderer : public BaseRenderer<std::string> {
public:
    TextRenderer();

    virtual void Render(Forest *forest);
    virtual void RenderDebug(Forest *forest);
    std::vector<std::string> GetDebugLines(Forest *forest);

    void Dispose() override;

    int GetCellForeground(Cell cell);
    int GetCellBackground(Cell cell);
private:
    HANDLE cHandle;
};

class EntityTextRenderer : public EntityRenderer<std::string> {
public:
    std::string RenderCell(Cell cell);
    std::string RenderTree(Cell cell, Tree* tree);
};

#endif //FIRESIM_TEXTRENDERER_H
