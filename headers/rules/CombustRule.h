//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_COMBUSTRULE_H
#define FIRESIM_COMBUSTRULE_H

#include "Rules.h"

class CombustRule : public Rule {
public:
    CombustRule() : Rule("combust") {}

    void Execute(Forest *forest, Cell self) override {
        int combustChance = utils::random(0, 50000);
        if(combustChance < DRY_COMBUST_CHANCE) {
            (*self.tree).Ignite();
            return;
        }
    }

    bool CanExecute(Cell self) override {
        return self.states->dry && (self.tree->IsAlive() && !(self.tree->IsBurning() || self.tree->IsIgnited()));
    }


};

#endif //FIRESIM_COMBUSTRULE_H
