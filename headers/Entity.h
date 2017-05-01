//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_CELL_H
#define FIRESIM_CELL_H


#include "Utils.h"

/**
 * The tree instance
 */
class Tree {
public:

    Tree();

    void Burn();
    /**
     * Updates the state of the tree, decrementing the current life if the tree is on fire
     */
    void Update();
    bool IsAlive();
    bool IsBurning();
    bool IsIgnited();
    /**
     * Ignites the tree, this is an intermediary state that should never be rendered
     */
    void Ignite();

    /**
     * Clears all states on the tree
     */
    void Clear();

    /**
     * Kills the tree, effectively removing it from non-regrowth calculations
     */
    void Kill();

    /**
     * Regrows the tree
     */
    void Grow();


    void Extinguish();

    float GetCurrentLife();
    /**
     * @return The percentage current life is of max life
     */
    float GetCurrentLifePercentage();

protected:
    bool burning;
    bool ignited;
    float currentLife;
};

/**
 * A lightweight bool collection for various states
 */
struct CellStates {
    bool damp = false;
    bool dry = false;
    bool wall = false;
    bool tokill = true;
};

/**
 * The cell struct, containing a tree pointer, the location within the forest, and a state collection
 */
struct Cell {
    Tree* tree;
    utils::Point point;
    CellStates* states;
};

#endif //FIRESIM_CELL_H
