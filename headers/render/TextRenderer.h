//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_TEXTRENDERER_H
#define FIRESIM_TEXTRENDERER_H

#include "BaseRenderer.h"

/**
 * A basic cross-platform text renderer, using strings for rendering.
 *
 * Outputs to the host terminal.
 *
 * Can be used as a foundation for other terminal-based renderers
 */
class TextRenderer : public BaseRenderer<std::string> {
public:
    TextRenderer();

    /**
     * Renders the forest to the host terminal
     * @param forest
     */
    virtual void Render(Forest *forest);
    /**
     * Writes forest statistics to the host terminal
     * @param forest
     */
    virtual void RenderDebug(Forest *forest);
    /**
     * Formats the forest statistics into lines of text
     * @param forest
     * @return The forest statistics in string form
     */
    std::vector<std::string> GetDebugLines(Forest *forest);

    /**
     * Nothing to clean up in this renderer
     */
    void Dispose() override;

    /**
     * Does not manage it's own loop
     * @return false
     */
    bool ManageOwnLoop() override;


};

class EntityTextRenderer : public EntityRenderer<std::string> {
public:
    /**
     * Does nothing
     */
    void LateInitialization() override;

    /**
     * Single-layer renderer, renderCell invokes renderTree unless the tree is dead
     * @param cell The cell
     * @return The string to draw for the cell
     */
    std::string RenderCell(Cell cell);
    /**
     * Single-layer renderer, renderCell invokes renderTree unless the tree is dead
     * @param cell The cell
     * @param tree The tree pointer
     * @return The string to draw for the tree
     */
    std::string RenderTree(Cell cell, Tree* tree);
};

#endif //FIRESIM_TEXTRENDERER_H
