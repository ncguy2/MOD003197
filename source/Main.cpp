//
// Created by Guy on 21/02/2017.
//

#include <iostream>
#include <Forest.h>
#include <render/TextRenderer.h>
#include <render/CursesRenderer.h>
#include <render/BufferedRenderer.h>
#include <menu/MenuHost.h>

#ifdef WITH_OPENGL
#include <Texture.h>
#include <OpenGLRenderer.h>

#endif

template <typename T, typename U>
void RenderWithRenderer(Forest* forest, bool managesOwnLoop = false) {
    BaseRenderer<U>* renderer = new T();
    if(managesOwnLoop) {
        renderer->Render(forest);
    }else {
        do {
            forest->Update();
            renderer->Render(forest);
            int cmd = utils::processInput();
            forest->processCommand(cmd);
        } while (forest->CanContinue());
    }
    renderer->Dispose();
}

void* TextRendererFunction(Forest* forest) {
    RenderWithRenderer<TextRenderer, std::string>(forest);
    return nullptr;
}

void* CursesRendererFunction(Forest* forest) {
    RenderWithRenderer<CursesRenderer, std::string>(forest);
    return nullptr;
}

void* BufferedRendererFunction(Forest* forest) {
    RenderWithRenderer<BufferedRenderer, std::string>(forest);
    return nullptr;
}

void* OpenGLRendererFunction(Forest* forest) {
#ifdef WITH_OPENGL
    RenderWithRenderer<OpenGLRenderer, Texture>(forest, true);
#endif
    return nullptr;
}

MenuHost createMenu(Forest* forest) {
    MenuHost menu(forest);
    // Text renderer
    menu.AddItem(MenuItem("Text Renderer").SetAction(TextRendererFunction));

    // Curses renderer
    menu.AddItem(MenuItem("Curses Renderer").SetAction(CursesRendererFunction));

    // Buffered renderer
    MenuItem bufferedRendererItem = MenuItem("Buffered Renderer").SetAction(BufferedRendererFunction);
#ifndef _WIN32
    bufferedRendererItem.SetDisabled("has a dependency on the win32 api, and will likely not work on non-win32 systems");
#endif
    menu.AddItem(bufferedRendererItem);

    // OpenGL renderer
    MenuItem openGLRendererItem = MenuItem("OpenGL Renderer").SetAction(OpenGLRendererFunction);
#ifndef WITH_OPENGL
    openGLRendererItem.SetDisabled("OpenGL subsystem not present within this build");
#else
    std::pair<bool, std::string> res = OpenGLRenderer::Initialize();
    if(!res.first)
        openGLRendererItem.SetDisabled(res.second);
#endif
    menu.AddItem(openGLRendererItem);

    return menu;
}

int main() {
    WindManager manager;
    Forest forest = Forest(manager).RegisterDefaultRules();
    createMenu(&forest).Process();
    return 0;
}