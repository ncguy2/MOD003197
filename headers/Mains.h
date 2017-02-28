//
// Created by Guy on 28/02/2017.
//

#ifndef FIRESIM_MAINS_H
#define FIRESIM_MAINS_H

#include <render/BaseRenderer.h>
#include <menu/MenuHost.h>

namespace Mains {

//    static std::map<BaseRenderer *, std::pair<bool, std::string>> renderers;
    static std::vector<MenuItem> rendererItems;

    void SearchForRenderers();
    MenuHost CreateMenu(Forest *forest);
    template <typename T>
    void Render(Forest* forest, BaseRenderer<T>* renderer, bool managesOwnLoop = false);
}

#endif //FIRESIM_MAINS_H
