//
// Created by Guy on 22/04/2017.
//

#include <rules/ExtinguishRule.h>

ExtinguishRule::ExtinguishRule() : Rule("extinguish") {}

void ExtinguishRule::Execute(Forest* forest, Cell self) {
    int r = utils::random(0, 100);
    if(r < DAMP_EXTINGUISH_CHANCE)
        self.tree->Extinguish();
}

bool ExtinguishRule::CanExecute(Cell self) {
    return self.states->damp && (self.tree->IsIgnited() || self.tree->IsBurning());
}
