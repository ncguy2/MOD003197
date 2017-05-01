//
// Created by Guy on 21/02/2017.
//

#include <Defines.h>
#include <render/TextRenderer.h>
#include <Utils.h>
#include <Config.h>

TextRenderer::TextRenderer() : BaseRenderer("Text Renderer", new EntityTextRenderer()) {}

void TextRenderer::Render(Forest *forest) {
    utils::clearScreen();

    std::string str;

    for(int x = 0; x < WORLD_SIZE_Y; x++) {
        for(int y = 0; y < WORLD_SIZE_X; y++) {
            Cell cell = forest->GetCell(y, x);
            std::cout << renderer->RenderCell(cell);
        }
        std::cout << std::endl;
    }

#if RENDER_DEBUG
    RenderDebug(forest);
#endif

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

bool TextRenderer::ManageOwnLoop() {
    return false;
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
