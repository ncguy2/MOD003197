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

    bool operator==(const LandCell& other) {
        if(this->x != other.x) return false;
        if(this->y != other.y) return false;
        if(this->damp != other.damp) return false;
        return true;
    }

};

class MoistureGenerationRule : public GenerationRule {
public:
    MoistureGenerationRule(bool dry = false) : GenerationRule(("moisture_generation" + std::string(dry ? ":dry" : ""))), dry(dry) {}

    void Generate(Forest *forest) override;

private:

//    LandCell moistureMap[WORLD_SIZE_X][WORLD_SIZE_Y];
    std::map<int, std::map<int, LandCell>> moistureMap;

    void GenerateOriginal();
    void GenerateFromPoint(int ox, int oy, int depth = 1, int px = -1, int py = -1);
    void Step();
    bool dry;

    std::vector<std::vector<char>> MoistureMapToVector();
    void MapToForest(Forest* forest);
};

#endif //FIRESIM_MOISTUREGENERATIONRULE_H
