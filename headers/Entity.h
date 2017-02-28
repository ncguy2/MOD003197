//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_CELL_H
#define FIRESIM_CELL_H


#include "Utils.h"

class Tree {
public:

    Tree();

    void Burn();
    void Update();
    bool IsAlive();
    bool IsBurning();
    bool IsIgnited();
    void Ignite();

    void Clear();

    void Kill();

    void Grow();

    void Extinguish();

    float GetCurrentLife();
    float GetCurrentLifePercentage();

protected:
    bool burning;
    bool ignited;
    float currentLife;
};

struct CellStates {
    bool damp = false;
    bool dry = false;
    bool wall = false;
};

struct Cell {
    Tree* tree;
    utils::Point point;
    CellStates* states;
};

#endif //FIRESIM_CELL_H
