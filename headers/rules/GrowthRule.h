//
// Created by 354584 on 22/02/2017.
//

#ifndef FIRESIM_GROWTHRULE_H
#define FIRESIM_GROWTHRULE_H

#include "Rules.h"

class GrowthRule : public Rule {
public:
    GrowthRule() : Rule("growth"), treeGrown(0) {}

    void Execute(Forest *forest, Cell self) override {

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

    bool CanExecute(Cell self) override {
        utils::Point p = self.point;
        if(p.x == 0 || p.x == WORLD_SIZE_X-1) return false;
        if(p.y == 0 || p.y == WORLD_SIZE_Y-1) return false;
        return !self.tree->IsAlive();
    }

    void Reset() override {
        treeGrown = 0;
    }

protected:
    bool treeGrown;

};

#endif //FIRESIM_GROWTHRULE_H
