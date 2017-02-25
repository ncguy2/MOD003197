//
// Created by Guy on 21/02/2017.
//

#include <render/TextRenderer.h>
#include <Utils.h>
#include <Config.h>

TextRenderer::TextRenderer() : BaseRenderer("Console Renderer", new EntityTextRenderer()) {
#if IS_WINDOWS
    cHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void TextRenderer::Render(Forest *forest) {
    utils::clearScreen();

    std::string str;

    int currentCol = -1;
    for(int x = 0; x < WORLD_SIZE_Y; x++) {
        for(int y = 0; y < WORLD_SIZE_X; y++) {
            Cell cell = forest->GetCell(y, x);
#if USE_COLOURS
            int attr = 0;
            if(Configuration::Instance().UseBlockRenderer())
                attr = utils::setConsoleColour(GetCellBackground(cell), GetCellForeground(cell));
            else attr = utils::setConsoleColour(GetCellForeground(cell), GetCellBackground(cell));

            if(currentCol != attr) {
                int fore = attr % 16;
                int back = (int) std::floor(attr / 16.f);
                utils::outputColouredText(str, false, fore, back, false);
                currentCol = attr;
                str.clear();
            }
            str.append(renderer->RenderCell(cell));
#else
            std::cout << renderer->RenderCell(cell);
#endif
        }
        str.append("\n");
    }

    int fore = currentCol % 16;
    int back = (int) std::floor(currentCol / 16.f);
    utils::outputColouredText(str, true, fore, back, true);


#if RENDER_DEBUG
    RenderDebug(forest);
#endif

}

int TextRenderer::GetCellForeground(Cell cell) {
    Tree* t = cell.tree;
#if USE_COLOURS
    if(!t->IsAlive()) {
        if(Configuration::Instance().UseBlockRenderer()) {
            if(cell.states->wall)
                return WALL_COLOUR;
            return DEFAULT_BACKGROUND;
        }
    }
    if(cell.states->wall)
        return WALL_COLOUR;

    if(t->IsIgnited() && !t->IsBurning())
        return IGNITED_TREE_COLOUR;
    if(t->IsBurning())
        return BURNING_TREE_COLOUR;
    if(t->IsAlive())
        return LIVING_TREE_COLOUR;
#endif

    if(Configuration::Instance().UseBlockRenderer()) {
        return DEFAULT_BACKGROUND;
    }
    return DEFAULT_TEXT_COLOUR;
}

int TextRenderer::GetCellBackground(Cell cell) {
#if USE_COLOURS
    if(cell.states->damp)
        return DAMP_GROUND_COLOUR;
    if(cell.states->dry)
        return DRY_GROUND_COLOUR;
    if(cell.states->wall)
        return WALL_COLOUR;
#endif
    if(Configuration::Instance().UseBlockRenderer())
        return DEFAULT_TEXT_COLOUR;
    return DEFAULT_BACKGROUND;
}

void TextRenderer::RenderDebug(Forest *forest) {
    for (std::string line : GetDebugLines(forest))
        std::cout << line << std::endl;
}

void TextRenderer::Dispose() {}

std::vector<std::string> TextRenderer::GetDebugLines(Forest *forest) {
    std::vector<std::string> lines = {
            std::string("Living Trees: " + utils::itos(forest->LivingTrees())),
            std::string("Ignited Trees: " + utils::itos(forest->IgnitedTrees())),
            std::string("Burning Trees: " + utils::itos(forest->BurningTrees())),
            std::string("Dead Trees: " + utils::itos(forest->DeadTrees())),
            std::string("Damp Cells: " + utils::itos(forest->DampCells())),
            std::string("Wind Direction: " + forest->GetWindManager().GetDirectionString()),
            std::string("Wind Speed: " + forest->GetWindManager().GetSpeedString()),
    };
    return lines;
}

std::string EntityTextRenderer::RenderCell(Cell cell) {
    if(cell.tree != nullptr)
        return RenderTree(cell, cell.tree);
    return TREE_DEAD;
}

std::string EntityTextRenderer::RenderTree(Cell cell, Tree *tree) {
    if(Configuration::Instance().UseBlockRenderer()) {
        if(cell.states->damp || cell.states->dry)
            return CELL_DAMP;
        return TREE_DEAD;
    }
    if(!tree->IsAlive()) return TREE_DEAD;
    if(tree->IsBurning()) return TREE_BURNING;
    if(tree->IsIgnited()) return TREE_IGNITED;
    return TREE_ALIVE;
}

void EntityTextRenderer::LateInitialization() {}
