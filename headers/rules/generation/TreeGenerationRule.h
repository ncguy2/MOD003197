//
// Created by Guy on 24/02/2017.
//

#ifndef FIRESIM_TREEGENERATIONRULE_H
#define FIRESIM_TREEGENERATIONRULE_H

#include <file/FileHandler.h>
#include "GenerationRule.h"

class TreeGenerationRule : public GenerationRule {
public:
    TreeGenerationRule() : GenerationRule("tree_generation") {}

    void Generate(Forest *forest) override {
        GenerateOriginal();
        int treeSmoothingIterations = TREE_GENERATION_ITERATIONS;
        for(int i = 0; i < treeSmoothingIterations; i++)
            Step();
        if(!ALLOW_TREE_ISLANDS) RemoveIslands();
        MapToForest(forest);
    }

private:

//    bool treeMap[WORLD_SIZE_X][WORLD_SIZE_Y];
    std::map<int, std::map<int, bool>> treeMap;

    void GenerateOriginal() {
        std::vector<int> randomNums;
        int initialGenChance = INITIAL_TREE_GENERATION_CHANCE;
//        randomNums.push_back(initialGenChance);
        for(int x = 0; x < forest->worldSizeX; x++) {
            for(int y = 0; y < forest->worldSizeY; y++) {
                int r = utils::rng::fastrand() % 100 + 1;
//                randomNums.push_back(r);
                bool t = (r < initialGenChance);
                treeMap[x][y] = t;
            }
        }
//        files::WriteVectorToFile<int>("TreeGen.txt", randomNums);
    }

    void Step() {
        int counter = 0;
        utils::Point offsets[] = {
                utils::Point{ 1,  1},
                utils::Point{-1,  1},
                utils::Point{-1, -1},
                utils::Point{ 1, -1},
                utils::Point{ 0,  0},
                utils::Point{ 0, -1},
                utils::Point{ 0,  1},
                utils::Point{-1,  0},
                utils::Point{ 1,  0}
        };
        for(int x = 1; x < forest->worldSizeX-1; x++) {
            for(int y = 1; y < forest->worldSizeY-1; y++) {
                counter = 0;

                for(utils::Point point : offsets)
                    if (treeMap[x + point.x][y + point.y]) counter++;

                if(counter >= 5)
                    treeMap[x][y] = true;
            }
        }
    }

    void RemoveIslands() {

    }

    std::vector<std::vector<char>> TreeMapToVector() {
        std::vector<std::vector<char>> root = std::vector<std::vector<char>>();
        for(int x = 0; x < forest->worldSizeX; x++) {
            std::vector<char> current = std::vector<char>();
            for(int y = 0; y < forest->worldSizeY; y++) {
                current.push_back(treeMap[x][y] ? '#' : '.');
            }
            root.push_back(current);
        }
        return root;
    }

    void MapToForest(Forest* forest) {
        for(int x = 0; x < forest->worldSizeX; x++) {
            for(int y = 0; y < forest->worldSizeY; y++) {
                Cell cell = forest->GetCell(x, y);
                if(!treeMap[x][y]) {
                    cell.tree->Kill();
                    cell.tree->Clear();
                }
            }
        }
    }
};

#endif //FIRESIM_TREEGENERATIONRULE_H
