//
// Created by Guy on 23/02/2017.
//

#include <Forest.h>
#include <render/TextRenderer.h>
#include <render/CursesRenderer.h>

CursesRenderer::CursesRenderer() {
    initscr();
    win = newwin(150, 100, 150, 100);
    wresize(win, WORLD_SIZE_Y, WORLD_SIZE_X);
    start_color();

    init_pair(DEFAULT_TEXT_COLOUR, DEFAULT_TEXT_COLOUR, BLACK);
    init_pair(LIVING_TREE_COLOUR, LIVING_TREE_COLOUR, BLACK);
    init_pair(IGNITED_TREE_COLOUR, IGNITED_TREE_COLOUR, BLACK);
    init_pair(BURNING_TREE_COLOUR, BURNING_TREE_COLOUR, BLACK);
}

void CursesRenderer::Render(Forest *forest) {
    move(0, 0);
    wclear(win);
    for(int x = 0; x < WORLD_SIZE_Y; x++) {
        for(int y = 0; y < WORLD_SIZE_X; y++) {
            Cell cell = forest->GetCell(y, x);
//            std::cout << renderer->RenderCell(cell);

            chtype col = COLOR_PAIR(GetCellForeground(cell));
            attrset(col);
//            color_set(utils::setConsoleColour(GetCellColour(cell)));
            mvaddstr(x, y, renderer->RenderCell(cell).c_str());
        }
//        std::cout << std::endl;
    }

#ifdef RENDER_DEBUG
    RenderDebug(forest);
#endif

    refresh();
}

void CursesRenderer::RenderDebug(Forest *forest) {
    int originY = (WORLD_SIZE_Y) + 10;
    for(std::string line : GetDebugLines(forest))
        mvaddstr(1, originY++, line.c_str());
}

void CursesRenderer::Dispose() {
    TextRenderer::Dispose();
    delwin(win);

}
