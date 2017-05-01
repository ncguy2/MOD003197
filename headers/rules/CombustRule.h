//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_COMBUSTRULE_H
#define FIRESIM_COMBUSTRULE_H

#include "Rules.h"

/**
 * Responsible for controlling the combustion on dry cells, can only execute on a cell that is dry and when it's tree is not already burning
 */
class CombustRule : public Rule {
public:
    CombustRule();
    void Execute(Forest *forest, Cell self) override;
    bool CanExecute(Cell self) override;

};

#endif //FIRESIM_COMBUSTRULE_H
