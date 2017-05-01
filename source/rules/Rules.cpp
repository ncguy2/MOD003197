//
// Created by Guy on 22/04/2017.
//

#include <rules/Rules.h>

Rule::Rule(std::string id) : id(id) {}

bool Rule::CanExecute(Cell self) {
    if(self.tree != nullptr)
        return self.tree->IsAlive();
    return false;
}

void Rule::Reset() {}
