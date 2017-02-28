//
// Created by Guy on 21/02/2017.
//

#include <Forest.h>
#include <Mains.h>


//
//    MenuHost createMenu(Forest *forest) {
//        MenuHost menu(forest);
//        // Text renderer
//        menu.AddItem(MenuItem("Text Renderer").SetAction(TextRendererFunction));
//
//        // Buffered renderer
//        MenuItem bufferedRendererItem = MenuItem("Buffered Renderer").SetAction(BufferedRendererFunction);
//#if !IS_WINDOWS
//        bufferedRendererItem.SetDisabled("has a dependency on the win32 api, and will likely not work on non-win32 systems");
//#endif
//        menu.AddItem(bufferedRendererItem);
//
//        // OpenGL renderer
//        MenuItem openGLRendererItem = MenuItem("OpenGL Renderer").SetAction(OpenGLRendererFunction);
//#ifndef WITH_OPENGL
//        openGLRendererItem.SetDisabled("OpenGL subsystem not present within this build");
//#else
//        std::pair<bool, std::string> res = OpenGLRenderer::Initialize();
//        if (!res.first)
//            openGLRendererItem.SetDisabled(res.second);
//#endif
//        menu.AddItem(openGLRendererItem);
//
//        return menu;
//    }


int main() {
    WindManager manager;
    Forest forest = Forest(manager).RegisterDefaultRules();
    Mains::CreateMenu(&forest).Process();
    return 0;
}