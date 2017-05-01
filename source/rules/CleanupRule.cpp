//
// Created by Guy on 22/04/2017.
//

#include <rules/CleanupRule.h>

CleanupRule::CleanupRule() : Rule("cleanup") {}

void CleanupRule::Execute(Forest* forest, Cell self) {
    if(!self.tree->IsAlive()) {
        self.tree->Clear();
    }
}


