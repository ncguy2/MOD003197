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

/**
 * The main forest
 */
class Forest {
public:
    Forest(WindManager manager, int w, int h);

    /**
     * Updates the forest, invoking the iteration rules sequentially
     */
    void Update();
    /**
     * Updates a single cell, this is invoked for each rule
     * @param cell The cell focused on by the current rule
     */
    void UpdateCell(Cell cell);

    DEPRECATED(Neighbours GetNeighbours(Cell cell));

    /**
     * Helper to get a cell from a point instance
     * @param point The point to query
     * @return The cell at the provided point
     */
    Cell GetCell(utils::Point point);
    /**
     * Helper to get a cell pointer from the a point instance
     * @param point The point to query
     * @return The cell pointer at the provided point
     */
    Cell* p_GetCell(utils::Point point);

    /**
     * Helper to get a cell from a location
     * @param x The x coordinate
     * @param y The y coordinate
     * @return The cell at the provided location
     */
    Cell GetCell(int x, int y);
    /**
     * Helper to get a cell pointer from a location
     * @param x The x coordinate
     * @param y The y coordinate
     * @return The cell pointerat the provided location
     */
    Cell* p_GetCell(int x, int y);

    /**
     * @return Whether the simulation should keep going
     */
    bool CanContinue();

    /**
     * @return Flat vector of all cells
     */
    std::vector<Cell> AllCells();

    /** @return The amount of living trees */
    int LivingTrees();
    /** @return The amount of ignited trees */
    int IgnitedTrees();
    /** @return The amount of burning trees */
    int BurningTrees();
    /** @return The amount of dead trees */
    int DeadTrees();
    /** @return The amount of damp cells */
    int DampCells();

    /**
     * Helper function to iterate over all cells within the forest
     * @tparam Func The function signature, can be inferred from callback
     * @param callback The function
     */
    template <typename Func>
    void ForEachCell(Func callback);

    /**
     * Is used to process a command related to the forest directly
     * @param cmd The command id
     */
    void processCommand(int cmd);

    /**
     * Populates the generation ruleset with the default generation rules
     * @return The forest instance for method chaining
     */
    Forest RegisterDefaultGenerationRules();
    /**
     * Populates the generation ruleset with the provided generation rules
     * @param ruleset The custom generation ruleset
     * @return The forest instance for method chaining
     */
    Forest RegisterCustomGenerationRules(RuleSet* ruleset);

    /**
     * Populates the iteration ruleset with the default iteration rules
     * @return The forest instance for method chaining
     */
    Forest RegisterDefaultRules();
    /**
     * Populates the iteration ruleset with the provided iteration rules
     * @param ruleset The custom generation ruleset
     * @return The forest instance for method chaining
     */
    Forest RegisterCustomRules(RuleSet* ruleset);

    /**
     * Resets the forest to it's initial state
     */
    void Reset();

    WindManager GetWindManager();

    const int worldSizeX;
    const int worldSizeY;
protected:
    /**
     * Populates the forest with it's base cells and the walls
     */
    void Populate();

    bool firstPass;
    /** Whether the simulation should exit */
    bool exit;
    /** The iteration ruleset */
    RuleSet* ruleSet;
    /** The generation ruleset */
    RuleSet* genRuleSet;
    /** The forest cell map */
    std::map<int, std::map<int, Cell>> cells;
    WindManager windManager;

    /** Spawns a fire at a random location */
    void StartFire();

};

#endif //FIRESIM_WORLD_H
