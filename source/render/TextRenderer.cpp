//
// Created by Guy on 21/02/2017.
//

#include <render/TextRenderer.h>
#include <Utils.h>

TextRenderer::TextRenderer() : BaseRenderer("Console Renderer", new EntityTextRenderer()) {
}

void TextRenderer::Render(Forest *forest) {
    utils::clearScreen();

    for(int x = 0; x < WORLD_SIZE; x++) {
        for(int y = 0; y < WORLD_SIZE; y++) {
            Cell cell = forest->GetCell(x, y);
            std::cout << renderer->RenderCell(cell);
        }
        std::cout << std::endl;
    }


#ifdef RENDER_DEBUG
    RenderDebug(forest);
#endif
}

void TextRenderer::RenderDebug(Forest *forest) {
    std::cout << "Living trees: " << forest->LivingTrees() << std::endl;
    std::cout << "Ignited trees: " << forest->IgnitedTrees() << std::endl;
    std::cout << "Burning trees: " << forest->BurningTrees() << std::endl;
    std::cout << "Dead trees: " << forest->DeadTrees() << std::endl;
}

char EntityTextRenderer::RenderCell(Cell cell) {
    if(cell.tree != nullptr)
        return RenderTree(cell.tree);
    return TREE_DEAD;
}

char EntityTextRenderer::RenderTree(Tree *tree) {
    if(!tree->IsAlive()) return TREE_DEAD;
    if(tree->IsBurning()) return TREE_BURNING;
    if(tree->IsIgnited()) return TREE_IGNITED;
    return TREE_ALIVE;
}
