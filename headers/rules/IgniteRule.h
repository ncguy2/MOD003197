//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_IGNITERULE_H
#define FIRESIM_IGNITERULE_H

#include "Rules.h"

class IgniteRule : public Rule {
public:
    IgniteRule() : Rule(1) {}

    virtual void Execute(const Forest *forest, const Neighbours neighbours) {

        Tree* t = neighbours.self.tree;

        if(t->IsIgnited()) t->Burn();

        if(t->IsBurning() || !t->IsAlive()) return;

        int burningNeighbours = 0;
        if(neighbours.north.tree->IsIgnited()) burningNeighbours++;
        if(neighbours.east.tree->IsIgnited())  burningNeighbours++;
        if(neighbours.south.tree->IsIgnited()) burningNeighbours++;
        if(neighbours.west.tree->IsIgnited())  burningNeighbours++;

        // TODO improve algorithm for proportional probability

        if(burningNeighbours == 0) return;

        int chance = 50 + (burningNeighbours * 5);
        int val = utils::random(0, 100);
        if(val < chance) {
            (*t).Ignite();
        }
    }
};

#endif //FIRESIM_IGNITERULE_H
