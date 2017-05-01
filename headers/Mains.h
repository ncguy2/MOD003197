//
// Created by Guy on 28/02/2017.
//

#ifndef FIRESIM_MAINS_H
#define FIRESIM_MAINS_H

#include <render/BaseRenderer.h>
#include <menu/MenuHost.h>

/**
 * A C-style object, accessible from anywhere to allow for other entities to register renderers without requiring another reference
 */
namespace Mains {

    /**
     * The renderer menu items
     */
    static std::vector<MenuItem> rendererItems;

    /**
     * Searches for any potential renderers using pre-processor definitions
     */
    void SearchForRenderers();
    /**
     * Creates the menu from the found renderers
     * @param forest The forest to host
     * @return The menu host instance
     */
    MenuHost CreateMenu(Forest *forest);

    /**
     * Invokes the provided renderers render function, this method can be used as an inline function pointer
     * @tparam T The renderer type, can be inferred from renderer
     * @param forest The forest
     * @param renderer The renderer
     * @param managesOwnLoop Whether the renderer manages it's own loop
     */
    template <typename T>
    void Render(Forest* forest, BaseRenderer<T>* renderer, bool managesOwnLoop = false);
}

#endif //FIRESIM_MAINS_H
