//
// Created by Guy on 22/04/2017.
//

#include <rules/CombustRule.h>

CombustRule::CombustRule() : Rule("combust") {}

void CombustRule::Execute(Forest* forest, Cell self) {
    int combustChance = utils::random(0, 50000);
    if(combustChance < DRY_COMBUST_CHANCE) {
        (*self.tree).Ignite();
        return;
    }
}

bool CombustRule::CanExecute(Cell self) {
    return self.states->dry && (self.tree->IsAlive() && !(self.tree->IsBurning() || self.tree->IsIgnited()));
}
