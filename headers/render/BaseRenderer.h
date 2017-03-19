//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_BASERENDERER_H
#define FIRESIM_BASERENDERER_H

#include <string>
#include <Forest.h>

struct ErrorException {
    std::string message;
};

template <class T>
class EntityRenderer {
public:
    virtual void LateInitialization()=0;
    virtual T RenderCell(Cell cell)= 0;
    virtual T RenderTree(Cell cell, Tree* tree)= 0;
};

template <class T>
class BaseRenderer {
public:
    BaseRenderer(std::string name, EntityRenderer<T>* renderer) : rendererName(name), renderer(renderer) {}
    const std::string rendererName;
    virtual void Render(Forest* forest)=0;
    virtual void Dispose()=0;

    virtual bool ManageOwnLoop()=0;

protected:
    EntityRenderer<T>* renderer;
};


#endif //FIRESIM_BASERENDERER_H
