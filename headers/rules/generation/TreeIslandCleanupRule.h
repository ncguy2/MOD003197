//
// Created by nick on 01/03/17.
//

#ifndef FIRESIM_TREEISLANDCLENAUPRULE_H
#define FIRESIM_TREEISLANDCLENAUPRULE_H

#include "GenerationRule.h"

/**
 * Removes any small clustered islands missed by the smoothing process
 */
class TreeIslandCleanupRule : public GenerationRule {
public:
    TreeIslandCleanupRule();
    /**
     * Marks trees as "to kill" and removes all with the mark
     * @param forest The forest to clean up
     */
    void Generate(Forest *forest) override;
};

#endif //FIRESIM_TREEISLANDCLENAUPRULE_H
