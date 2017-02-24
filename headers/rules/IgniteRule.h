//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_IGNITERULE_H
#define FIRESIM_IGNITERULE_H

#include "Rules.h"

class IgniteRule : public Rule {
public:
    IgniteRule(bool allowBurn = true) : Rule(std::string("ignition:")+(allowBurn?"burn":"")), allowBurn(allowBurn) {}

    void Execute(Forest *forest, Cell self) override {

        Tree* t = self.tree;

        if(allowBurn && t->IsIgnited()) {
            t->Burn();
            return;
        }

        if(t->IsBurning() || !t->IsAlive()) return;

        int burningNeighbours = 0;

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
                if(cell.tree->IsBurning()) burningNeighbours++;
            }
        }

        // TODO improve algorithm for proportional probability

        if(burningNeighbours == 0) return;

        int chance = BASE_IGNITE_CHANCE + (burningNeighbours * BURNING_IGNITE_MODIFIER);
        int val = utils::random(0, 100);
        if(val < chance) {
            (*t).Ignite();
        }
    }

protected:
    bool allowBurn;

};

#endif //FIRESIM_IGNITERULE_H
