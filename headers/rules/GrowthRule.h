//
// Created by 354584 on 22/02/2017.
//

#ifndef FIRESIM_GROWTHRULE_H
#define FIRESIM_GROWTHRULE_H

#include "Rules.h"

/**
 * Manages the tree regrowth after a tree death, can only grow up to "Tree.Growth.Max" trees per iteration
 */
class GrowthRule : public Rule {
public:
    GrowthRule();
    void Execute(Forest *forest, Cell self) override;
    bool CanExecute(Cell self) override;
    /**
     * Resets the trees grown counter
     */
    void Reset() override;

protected:
    /**
     * The amount of trees grown this iteration
     */
    int treeGrown;
};

#endif //FIRESIM_GROWTHRULE_H
