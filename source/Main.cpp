//
// Created by Guy on 21/02/2017.
//

#include <iostream>
#include <Forest.h>
#include <render/TextRenderer.h>

int main() {
    Forest forest;
    TextRenderer renderer;
    while(forest.CanContinue()) {
        forest.Update();
        renderer.Render(&forest);
        utils::waitForKeypress();
    }
    return 0;
}