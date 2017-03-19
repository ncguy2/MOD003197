//
// Created by nick on 01/03/17.
//

#ifndef FIRESIM_TREEISLANDCLENAUPRULE_H
#define FIRESIM_TREEISLANDCLENAUPRULE_H

#include "GenerationRule.h"

class TreeIslandCleanupRule : public GenerationRule {
public:
    TreeIslandCleanupRule() : GenerationRule("TreeIslandCleanup") {}

    void Generate(Forest *forest) override {
        int neighbourCount = 0;
        utils::Point offsets[] = {
                utils::Point{-1,  0},
                utils::Point{ 1,  0},
                utils::Point{ 0,  1},
                utils::Point{ 0, -1},
                utils::Point{-1, -1},
                utils::Point{-1,  1},
                utils::Point{ 1,  1},
                utils::Point{ 1, -1}
        };
        for(int x = 1; x < WORLD_SIZE_X-1; x++) {
            for(int y = 1; y < WORLD_SIZE_Y-1; y++) {
                neighbourCount = 0;
                Cell self = forest->GetCell(x, y);
                for(utils::Point p : offsets) {
                    Cell cell = forest->GetCell(x + p.x, y + p.y);
                    if(cell.tree->IsAlive() || cell.states->wall) {
                        neighbourCount++;
                    }
                }
                if(neighbourCount > TREE_MINIMUM_CLUSTER) {
                    self.states->tokill = false;
                }
            }
        }

        int killCount = 0;
        for(int x = 0; x < WORLD_SIZE_X; x++) {
            for (int y = 0; y < WORLD_SIZE_Y; y++) {
                Cell cell = forest->GetCell(x, y);
                if(cell.states->tokill) {
                    cell.tree->Kill();
                    killCount++;
                }
            }
        }

        std::cout << "Killed " << killCount << " trees" << std::endl;

    }

};

#endif //FIRESIM_TREEISLANDCLENAUPRULE_H
