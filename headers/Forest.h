//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_WORLD_H
#define FIRESIM_WORLD_H


#include <iostream>
#include <rules/RuleSet.h>
#include "Defines.h"
#include "Utils.h"
#include "WindManager.h"

class Forest {
public:
    Forest(WindManager manager, int w, int h);
    void Update();
    void UpdateCell(Cell cell);
    Neighbours GetNeighbours(Cell cell);

    Cell GetCell(utils::Point point) {
        return GetCell(point.x, point.y);
    }
    Cell* p_GetCell(utils::Point point) {
        return p_GetCell(point.x, point.y);
    }

    Cell GetCell(int x, int y);
    Cell* p_GetCell(int x, int y);
    bool CanContinue();

    std::vector<Cell> AllCells();

    int LivingTrees();
    int IgnitedTrees();
    int BurningTrees();
    int DeadTrees();
    int DampCells();


    template <typename Func>
    void ForEachCell(Func callback);

    void processCommand(int basic_string);

    Forest RegisterDefaultGenerationRules();
    Forest RegisterCustomGenerationRules(RuleSet* ruleset);

    Forest RegisterDefaultRules();
    Forest RegisterCustomRules(RuleSet* ruleset);

    void Reset();

    WindManager GetWindManager();

    const int worldSizeX;
    const int worldSizeY;
protected:
    void Populate();

    bool firstPass;
    bool exit;
    RuleSet* ruleSet;
    RuleSet* genRuleSet;
//    Cell cells[worldSizeX][worldSizeY];
    std::map<int, std::map<int, Cell>> cells;
    WindManager windManager;

    void StartFire();

};

#endif //FIRESIM_WORLD_H
