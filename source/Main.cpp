//
// Created by Guy on 21/02/2017.
//

#include <iostream>
#include <Forest.h>
#include <render/TextRenderer.h>
#include <render/CursesRenderer.h>
#include <render/BufferedRenderer.h>
#include <menu/MenuHost.h>

template <typename T, typename U>
void RenderWithRenderer(Forest* forest) {
    BaseRenderer<U>* renderer = new T();
    do {
        forest->Update();
        renderer->Render(forest);
        int cmd = utils::processInput();
        forest->processCommand(cmd);
    }while(forest->CanContinue());
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
    return menu;
}

int main() {
    Forest forest = Forest().RegisterDefaultRules();
    createMenu(&forest).Process();
//    CursesRenderer renderer;
//    try{
//        do {
//            forest.Update();
//            renderer.Render(&forest);
//            int cmd = utils::processInput();
//            forest.processCommand(cmd);
//        }while(forest.CanContinue());
//    }catch(LPSTR win_error) {
//        std::cout << win_error << std::endl;
//    }
    return 0;
}