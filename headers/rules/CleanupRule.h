//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_CLEANUPRULE_H
#define FIRESIM_CLEANUPRULE_H

#include "Rules.h"

/**
 * Rule to clean up references to trees that are no longer alive, removing their references as to prevent
 * future rules from executing on a dead tree
 */
class CleanupRule : public Rule {
public:
    CleanupRule();
    void Execute(Forest *forest, Cell self) override;
};

#endif //FIRESIM_CLEANUPRULE_H
