//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_TREEGENERATIONRULE_H
#define FIRESIM_TREEGENERATIONRULE_H

#include <file/FileHandler.h>
#include "GenerationRule.h"

/**
 * Initial generation of the trees
 */
class TreeGenerationRule : public GenerationRule {
public:
    TreeGenerationRule();
    /**
     * Invokes the generation process
     * @param forest The forest to generate
     */
    void Generate(Forest *forest) override;

private:
    /**
     * The tree states in map form for rapid lookup
     */
    std::map<int, std::map<int, bool>> treeMap;

    /**
     * Initial generation pass, randomly selects cells to make trees
     */
    void GenerateOriginal();

    /**
     * Smooths the gaps between trees, creating larger clusters and removing small islands
     */
    void Step();

    /**
     * Converts the treemap to a 2D vector
     * @return The treemap in 2D vector form
     */
    std::vector<std::vector<char>> TreeMapToVector();

    /**
     * Maps the tree states to the cells in the forest
     * @param forest The forest to map to
     */
    void MapToForest(Forest* forest);
};

#endif //FIRESIM_TREEGENERATIONRULE_H
