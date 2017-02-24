//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_MOISTUREGENERATIONRULE_H
#define FIRESIM_MOISTUREGENERATIONRULE_H

#include <file/FileHandler.h>
#include "GenerationRule.h"
#include <Utils.h>

struct LandMass;

struct LandCell {
    bool damp;
    int x;
    int y;
    std::vector<LandMass*> connectedMasses;

    void AddLandMass(LandMass* mass) {
        connectedMasses.push_back(mass);
    }

    bool operator==(const LandCell& other) {
        if(this->x != other.x) return false;
        if(this->y != other.y) return false;
        if(this->damp != other.damp) return false;
        return true;
    }

};

struct LandMass {
    std::vector<LandCell> cells;
    void Combine(LandMass other) {
        for(LandCell cell : cells) {
            if(!(std::find(other.cells.begin(), other.cells.end(), cell) != other.cells.end()))
                other.cells.push_back(cell);
        }
        cells.clear();
        delete this;
    }
    int Size() {
        return cells.size();
    }
};

class MoistureGenerationRule : public GenerationRule {
public:
    MoistureGenerationRule() : GenerationRule("moisture_generation") {
        for(int x = 0; x < WORLD_SIZE_X; x++) {
            for(int y = 0; y < WORLD_SIZE_Y; y++) {
                LandCell cell;
                cell.x = x;
                cell.y = y;
                cell.damp = false;
                cell.connectedMasses = std::vector<LandMass*>();
                moistureMap[x][y] = cell;
            }
        }

    }

    void Generate(Forest *forest) override;

private:

    LandCell moistureMap[WORLD_SIZE_X][WORLD_SIZE_Y];

    void GenerateOriginal();

    void Step();

    /**
     * \Procedure Iterate over sample cells
     * \Procedure Get connected cells within radius of 5(?)
     * \Procedure Check landmasses for shared cells
     * \Procedure If a cell is shared, merge both landmasses into one
     * \Procedure Get size of landmasses by amount of unique cells
     * \Procedure Remove all but the largest
     */
    void RemoveIslands();

    bool AreLandCellsConnected(LandCell a, LandCell b, bool allowDiagonal = false, std::vector<LandCell> visited = std::vector<LandCell>());
    void GetConnectedCells(LandCell a, bool allowDiagonal = false, LandMass visited = LandMass(), int maxDistance = 5);

    std::vector<std::vector<char>> MoistureMapToVector();
    void MapToForest(Forest* forest);
};

#endif //FIRESIM_MOISTUREGENERATIONRULE_H
