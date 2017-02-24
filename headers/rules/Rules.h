//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_RULE_H
#define FIRESIM_RULE_H

#include <Entity.h>
#include <Defines.h>

class Forest;

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
    Rule(std::string id) : id(id) {}
    std::string id;
    /**
     * @param forest The owning forest object, in case the rule needs data beyond the scope of Neighbours
     * @param self The currently focused cell
     */
    virtual void Execute(Forest* forest, Cell self)=0;

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

    virtual void Reset() {}

};


#endif //FIRESIM_RULE_H
