//
// Created by Guy on 22/04/2017.
//

#include <rules/GrowthRule.h>
#include <Forest.h>

GrowthRule::GrowthRule() : Rule("growth"), treeGrown(0) {}

void GrowthRule::Execute(Forest* forest, Cell self) {

    if(treeGrown > MAX_TREE_GROWTH) return;

    float growthChance = TREE_BASE_GROWTH_CHANCE;

    utils::Point offsets[] = {
#if NEIGHBOUR_DETECTION_MODE >= 1
    utils::Point{ 1,  1},
    utils::Point{-1,  1},
    utils::Point{-1, -1},
    utils::Point{ 1, -1},
#endif
    utils::Point{ 0, -1},
    utils::Point{ 0,  1},
    utils::Point{-1,  0},
    utils::Point{ 1,  0}
    };

    utils::Point local = self.point;
    for(utils::Point offset : offsets) {
        Cell cell = forest->GetCell(local.x + offset.x, local.y + offset.y);
        if(cell.tree != nullptr) {
            if(cell.tree->IsAlive()) {
                if (cell.tree->IsBurning() || cell.tree->IsIgnited())
                    return;
                growthChance += (TREE_GROWTH_EFFECT);
            }
        }
    }

    int val = utils::random(0, 500);
    if(val < growthChance) {
        self.tree->Grow();
        treeGrown++;
    }

}

bool GrowthRule::CanExecute(Cell self) {
    return !self.tree->IsAlive();
}

void GrowthRule::Reset() {
    treeGrown = 0;
}
