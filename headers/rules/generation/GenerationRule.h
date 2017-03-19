//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_GENERATIONRULE_H
#define FIRESIM_GENERATIONRULE_H

#include "rules/Rules.h"

class GenerationRule : public Rule {
public:
    GenerationRule(std::string id) : Rule(id) {}

    virtual void Generate(Forest* forest)=0;

    void Execute(Forest *forest, Cell self) override {
        this->forest = forest;
        Generate(forest);
    }

    bool CanExecute(Cell self) override {
        return true;
    }

protected:
    Forest* forest;

};

#endif //FIRESIM_GENERATIONRULE_H
