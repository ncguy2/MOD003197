//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_WORLD_H
#define FIRESIM_WORLD_H


#include <iostream>
#include <rules/RuleSet.h>
#include "Defines.h"
#include "Utils.h"

class Forest {
public:
    Forest();
    void Update();
    void UpdateCell(Cell cell);
    Neighbours GetNeighbours(utils::Point point);

    Cell GetCell(utils::Point point) {
        return GetCell(point.x, point.y);
    }

    Cell GetCell(int x, int y);
    bool CanContinue();
    int LivingTrees();
    int IgnitedTrees();
    int BurningTrees();
    int DeadTrees();

    void ForEachCell(void (foreach)(utils::Point, Cell));

protected:
    void Populate();
    void RegisterRules();

    RuleSet ruleSet;
    Cell cells[WORLD_SIZE][WORLD_SIZE];
};

#endif //FIRESIM_WORLD_H
