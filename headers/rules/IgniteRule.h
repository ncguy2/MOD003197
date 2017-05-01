//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_IGNITERULE_H
#define FIRESIM_IGNITERULE_H

#include <WindManager.h>
#include "Rules.h"

/**
 * Controls whether trees should ignite, taking into account neighbour states, wind speed and direction, and ground states
 */
class IgniteRule : public Rule {
public:

    IgniteRule(bool allowBurn = true);
    void Execute(Forest *forest, Cell self) override;
    bool CanExecute(Cell self) override;

protected:
    /**
     * Whether to burn ignited trees
     */
    bool allowBurn;

};

#endif //FIRESIM_IGNITERULE_H
