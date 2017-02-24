//
// Created by Guy on 21/02/2017.
//

#include <render/TextRenderer.h>
#include <Utils.h>

TextRenderer::TextRenderer() : BaseRenderer("Console Renderer", new EntityTextRenderer()) {
    cHandle = GetStdHandle(STD_OUTPUT_HANDLE);
}

void TextRenderer::Render(Forest *forest) {
    utils::clearScreen();

    std::string str;

    int currentCol = -1;
    for(int x = 0; x < WORLD_SIZE_Y; x++) {
        for(int y = 0; y < WORLD_SIZE_X; y++) {
            Cell cell = forest->GetCell(y, x);
#if USE_COLOURS && IS_WINDOWS
            int attr = utils::setConsoleColour(GetCellForeground(cell), GetCellBackground(cell));
            if(currentCol != attr) {
                std::cout << str;
                currentCol = attr;
                str.clear();
                SetConsoleTextAttribute(cHandle, attr);
            }
            str.append(renderer->RenderCell(cell));
#else
            std::cout << renderer->RenderCell(cell);
#endif
        }
        str.append("\n");
    }
    std::cout << str << std::endl;


#ifdef RENDER_DEBUG
    RenderDebug(forest);
#endif

    SetConsoleTextAttribute(cHandle, utils::setConsoleColour(WHITE, BLACK));

}

int TextRenderer::GetCellForeground(Cell cell) {
#if USE_COLOURS
    Tree* t = cell.tree;
    if(t->IsIgnited() && !t->IsBurning())
        return IGNITED_TREE_COLOUR;
    if(t->IsBurning())
        return BURNING_TREE_COLOUR;
    if(t->IsAlive())
        return LIVING_TREE_COLOUR;
#endif
    return DEFAULT_TEXT_COLOUR;
}

void TextRenderer::RenderDebug(Forest *forest) {
    std::cout << " Living trees: " << forest->LivingTrees() << std::endl;
    std::cout << " Ignited trees: " << forest->IgnitedTrees() << std::endl;
    std::cout << " Burning trees: " << forest->BurningTrees() << std::endl;
    std::cout << " Dead trees: " << forest->DeadTrees() << std::endl;
}

void TextRenderer::Dispose() {}

int TextRenderer::GetCellBackground(Cell cell) {
#if USE_COLOURS
    if(cell.damp)
        return DAMP_GROUND_COLOUR;
#endif
    return DEFAULT_TEXT_COLOUR;
}

std::string EntityTextRenderer::RenderCell(Cell cell) {
    if(cell.tree != nullptr)
        return RenderTree(cell.tree);
    return TREE_DEAD;
}

std::string EntityTextRenderer::RenderTree(Tree *tree) {
    if(!tree->IsAlive()) return TREE_DEAD;
    if(tree->IsBurning()) return TREE_BURNING;
    if(tree->IsIgnited()) return TREE_IGNITED;
    return TREE_ALIVE;
}
