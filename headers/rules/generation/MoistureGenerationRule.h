//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_MOISTUREGENERATIONRULE_H
#define FIRESIM_MOISTUREGENERATIONRULE_H

#include <file/FileHandler.h>
#include "GenerationRule.h"
#include <Utils.h>

/**
 * Lightweight cell representing the forest cells
 */
struct LandCell {
    bool damp;
    int x;
    int y;

    bool operator==(const LandCell& other);
};

/**
 * Generates either damp or dry cells
 */
class MoistureGenerationRule : public GenerationRule {
public:
    /**
     * @param dry Specifies whether to generate damp cells or dry cells
     */
    MoistureGenerationRule(bool dry = false);
    /**
     * Invokes the generation process
     * @param forest The forest to generate
     */
    void Generate(Forest *forest) override;

private:

    /**
     * Map for quick lookup of current cells
     */
    std::map<int, std::map<int, LandCell>> moistureMap;

    /**
     * Pick a single point and expand from it
     */
    void GenerateOriginal();
    /**
     * Expands from the specified point
     * @param ox The X origin
     * @param oy The Y origin
     * @param depth The current depth of generation, will not exceed the "Generation.Moisture.MaxSpawns" property
     * @param px The parent node X position
     * @param py The parent node Y position
     */
    void GenerateFromPoint(int ox, int oy, int depth = 1, int px = -1, int py = -1);
    /**
     * Smoothing step to reduce the amount of islands and make the mass more uniform in the center
     */
    void Step();

    /**
     * Whether dry cells should be generated instead
     */
    bool dry;

    /**
     * Converts the moistureMap to a vector, ready for output to a file
     * @return The moistureMap in a printable form
     */
    std::vector<std::vector<char>> MoistureMapToVector();
    /**
     * Maps the moistureMap to the forest
     * @param forest The forest to mutate
     */
    void MapToForest(Forest* forest);
};

#endif //FIRESIM_MOISTUREGENERATIONRULE_H
