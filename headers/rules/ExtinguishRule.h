//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_EXTINGUISHRULE_H
#define FIRESIM_EXTINGUISHRULE_H

#include "Rules.h"

/**
 * Controls whether the fire should be extinguished in damp cells, further slowing the spread of the fire in damp regions
 */
class ExtinguishRule : public Rule {
public:
    ExtinguishRule();

    void Execute(Forest *forest, Cell self) override;
    bool CanExecute(Cell self) override;

};

#endif //FIRESIM_EXTINGUISHRULE_H
