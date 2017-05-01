//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_BASERENDERER_H
#define FIRESIM_BASERENDERER_H

#include <string>
#include <Forest.h>

/**
 * Simple error exception, can be thrown to distinguish between internal std::exception throws and custom exceptions
 */
struct ErrorException {
    std::string message;
};

/**
 * Responsible for getting the object to render
 * @tparam T The type of object to supply for rendering
 */
template <class T>
class EntityRenderer {
public:
    /**
     * Provides a method of late initialization, executes between forest+renderer preparation and rendering
     */
    virtual void LateInitialization()=0;

    /**
     * @param cell The cell to render
     * @return The T instance to render for the Cell
     */
    virtual T RenderCell(Cell cell)= 0;
    /**
     * Helper method to separate cell and tree render item processing
     * @param cell The current cell to render
     * @param tree The tree pointer within the cell, (slightly redundant as the cell contains the same tree pointer)
     * @return The T instance to render for the Tree
     */
    virtual T RenderTree(Cell cell, Tree* tree)= 0;
};

/**
 * The core renderer. This is paired with an entity renderer and is responsible for rendering the forest
 * @tparam T The type of object expected from the entity renderer
 */
template <class T>
class BaseRenderer {
public:
    /**
     * This will typically be overridden by each renderer implementation, filling in both parameters internally
     * @param name The renderer name
     * @param renderer The entity renderer instance pointer
     */
    BaseRenderer(std::string name, EntityRenderer<T>* renderer) : rendererName(name), renderer(renderer) {}

    const std::string rendererName;

    /**
     * The render process
     * @param forest The forest to render
     */
    virtual void Render(Forest* forest)=0;
    /**
     * Cleanup to prevent memory leaks or dangling pointers
     */
    virtual void Dispose()=0;

    /**
     * @return Whether the renderer manages it's own loop
     */
    virtual bool ManageOwnLoop()=0;

protected:
    EntityRenderer<T>* renderer;
};


#endif //FIRESIM_BASERENDERER_H
