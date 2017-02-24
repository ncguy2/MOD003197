//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_EXTINGUISHRULE_H
#define FIRESIM_EXTINGUISHRULE_H

#include "Rules.h"

class ExtinguishRule : public Rule {
public:
    ExtinguishRule() : Rule("extinguish") {}

    void Execute(Forest *forest, Cell self) override {
        int r = utils::random(0, 100);
        if(r < DAMP_EXTINGUISH_CHANCE)
            self.tree->Extinguish();
    }

    bool CanExecute(Cell self) override {
        return self.states->damp && (self.tree->IsIgnited() || self.tree->IsBurning());
    }

};

#endif //FIRESIM_EXTINGUISHRULE_H
