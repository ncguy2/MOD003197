//
// Created by Guy on 23/02/2017.
//

#ifndef FIRESIM_CURSESRENDER_H
#define FIRESIM_CURSESRENDER_H

#include "TextRenderer.h"
#include <curses.h>

class CursesRenderer : public TextRenderer {
public:

    CursesRenderer();

    void Render(Forest *forest) override;
    void RenderDebug(Forest *forest) override;

    void Dispose() override;

protected:
    WINDOW* win;
    WINDOW* scr;
};

#endif //FIRESIM_CURSESRENDER_H
