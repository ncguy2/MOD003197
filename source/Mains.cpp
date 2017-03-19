//
// Created by Guy on 28/02/2017.
//

#include <menu/MenuHost.h>
#include <Mains.h>
#include <render/TextRenderer.h>
#ifdef WITH_OPENGL
#include <OpenGLRenderer.h>
#endif
#ifdef USE_BUFFERED_RENDERER
#include "../subsystems/win32_gcc/headers/render/BufferedRenderer.h"
#endif

template <typename T, typename U>
void UseRenderer(Forest* forest) {
    T* t;
    try {
        t = new T();
        Mains::Render(forest, t, t->ManageOwnLoop());
    }catch(ErrorException ee) {
        std::cerr << "Render initialization error occurred: " << std::endl;
        std::cerr << "\t" << ee.message << std::endl;
        utils::processInput();
        return;
    }
}

void Mains::SearchForRenderers() {
    rendererItems.push_back(MenuItem("Text Renderer").SetAction(UseRenderer<TextRenderer, std::string>));
#ifdef USE_BUFFERED_RENDERER
    rendererItems.push_back(MenuItem("Buffered Renderer").SetAction(UseRenderer<BufferedRenderer, std::string>));
#endif
#ifdef WITH_OPENGL
    rendererItems.push_back(MenuItem("OpenGL Renderer").SetAction(UseRenderer<OpenGLRenderer, Texture>));
#endif
}

MenuHost Mains::CreateMenu(Forest*forest){
    SearchForRenderers();
    MenuHost menu(forest);
    for(MenuItem item : rendererItems)
        menu.AddItem(item);
    return menu;
}

template <typename T>
void Mains::Render(Forest *forest, BaseRenderer<T>* renderer, bool managesOwnLoop) {
    if (managesOwnLoop) {
        renderer->Render(forest);
    } else {
        do {
            forest->Update();
            renderer->Render(forest);
            int cmd = utils::processInput();
            forest->processCommand(cmd);
        } while (forest->CanContinue());
    }
    renderer->Dispose();
}
