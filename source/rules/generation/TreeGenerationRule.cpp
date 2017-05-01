//
// Created by Guy on 22/04/2017.
//

#include <rules/generation/TreeGenerationRule.h>
#include <Forest.h>

TreeGenerationRule::TreeGenerationRule() : GenerationRule("tree_generation") {}

void TreeGenerationRule::Generate(Forest* forest) {
    GenerateOriginal();
    int treeSmoothingIterations = TREE_GENERATION_ITERATIONS;
    for(int i = 0; i < treeSmoothingIterations; i++)
        Step();
    MapToForest(forest);
}

void TreeGenerationRule::GenerateOriginal() {
    std::vector<int> randomNums;
    int initialGenChance = INITIAL_TREE_GENERATION_CHANCE;
    for(int x = 0; x < forest->worldSizeX; x++) {
        for(int y = 0; y < forest->worldSizeY; y++) {
            int r = utils::rng::fastrand() % 100 + 1;
            bool t = (r < initialGenChance);
            treeMap[x][y] = t;
        }
    }
}

void TreeGenerationRule::Step() {
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

std::vector<std::vector<char>> TreeGenerationRule::TreeMapToVector() {
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

void TreeGenerationRule::MapToForest(Forest* forest) {
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
