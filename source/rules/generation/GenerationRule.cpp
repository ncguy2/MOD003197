//
// Created by Guy on 22/04/2017.
//

#include <rules/generation/GenerationRule.h>

GenerationRule::GenerationRule(std::string id) : Rule(id) {}

void GenerationRule::Execute(Forest* forest, Cell) {
    this->forest = forest;
    Generate(forest);
}

bool GenerationRule::CanExecute(Cell) {
    return true;
}
