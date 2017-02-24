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

    int originY = (WORLD_SIZE_Y * 24) + 10;

    mvaddstr(1, originY++,"Living trees: " + forest->LivingTrees());
    mvaddstr(1, originY++,"Ignited trees: " + forest->IgnitedTrees());
    mvaddstr(1, originY++,"Burning trees: " + forest->BurningTrees());
    mvaddstr(1, originY,  "Dead trees: " + forest->DeadTrees());

//    std::cout << " Living trees: " << forest->LivingTrees() << std::endl;
//    std::cout << " Ignited trees: " << forest->IgnitedTrees() << std::endl;
//    std::cout << " Burning trees: " << forest->BurningTrees() << std::endl;
//    std::cout << " Dead trees: " << forest->DeadTrees() << std::endl;
}

void CursesRenderer::Dispose() {
    TextRenderer::Dispose();
    delwin(win);

}
