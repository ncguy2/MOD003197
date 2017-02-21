//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_CELL_H
#define FIRESIM_CELL_H


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

protected:
    bool burning;
    bool ignited;
    int currentLife;
};

struct Cell {
    Tree* tree;
};

#endif //FIRESIM_CELL_H
