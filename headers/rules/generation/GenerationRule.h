//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_GENERATIONRULE_H
#define FIRESIM_GENERATIONRULE_H

#include "rules/Rules.h"

/**
 * Special rule type modified to execute across the entire forest in 1 pass
 */
class GenerationRule : public Rule {
public:
    /**
     * @param id Internal id used to identify each rule during debugging
     */
    GenerationRule(std::string id);

    /**
     * The generation process for this rule
     * @param forest
     */
    virtual void Generate(Forest* forest)=0;

    /**
     * Intermediary function to use existing rule functionality for the forest generation
     * @param forest The forest to generate
     */
    void Execute(Forest *forest, Cell) override;

    /**
     * Implementation to say that all generation rules should execute
     * @return true
     */
    bool CanExecute(Cell) override;

protected:
    Forest* forest;

};

#endif //FIRESIM_GENERATIONRULE_H
