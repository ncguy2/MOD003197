//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_RULE_H
#define FIRESIM_RULE_H

#include <Entity.h>
class Forest; // Forward declaration

/**
 * Contains all information required for a typical rule to execute
 */
struct Neighbours {
    Cell self;

    Cell north;
    Cell east;
    Cell south;
    Cell west;
};

/**
 * Abstract class declaring a rule to be executed upon the cells
 */
class Rule {
public:
    Rule(const int id) : id(id) {}
    const int id;
    /**
     * @param forest The owning forest object, in case the rule needs data beyond the scope of Neighbours
     * @param neighbours The calculated neighbours struct, populated with pointers associated with the surrounding cells
     */
    virtual void Execute(const Forest* forest, const Neighbours neighbours)=0;

    /**
     * An early check for if the rule can execute.
     * If this function returns false, the neighbours of the current cell are not calculated, and the next cell is checked.
     *
     * Defaults to true if the cell has no living tree
     *
     * @param self The Cell currently being checked for rule eligibility
     * @return Whether the cell is eligible for this rule
     */
    virtual bool CanExecute(Cell self) {
        if(self.tree != nullptr)
            return self.tree->IsAlive();
        return false;
    }
};


#endif //FIRESIM_RULE_H
