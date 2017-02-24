//
// Created by Guy on 24/02/2017.
//

#include <rules/generation/MoistureGenerationRule.h>
#include <Forest.h>

void MoistureGenerationRule::Generate(Forest *forest) {
    GenerateOriginal();
    for(int i = 0; i < MOISTURE_GENERATION_ITERATIONS; i++)
        Step();
    RemoveIslands();
    MapToForest(forest);
}

void MoistureGenerationRule::GenerateOriginal() {
    std::vector<int> randomNums;
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            int r = utils::random(1, 100);
            randomNums.push_back(r);
            bool t = (r < INITIAL_MOISTURE_GENERATION_CHANCE);
            moistureMap[x][y].x = x;
            moistureMap[x][y].y = y;
            moistureMap[x][y].damp = t;
        }
    }
}

void MoistureGenerationRule::Step() {
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
    for(int x = 1; x < WORLD_SIZE_X-1; x++) {
        for(int y = 1; y < WORLD_SIZE_Y-1; y++) {
            counter = 0;

            for(utils::Point point : offsets)
                if (moistureMap[x + point.x][y + point.y].damp) counter++;

            if(counter >= 5)
                moistureMap[x][y].damp = true;
        }
    }
}

void MoistureGenerationRule::RemoveIslands() {
    std::vector<LandMass*> landmasses;
    int rad = 5;

    utils::Point sampleOffset = utils::Point{30, 30};

    // Iterate over sample cells
    for(int x = 0; x < WORLD_SIZE_X; x+=sampleOffset.x) {
        for(int y = 0; y < WORLD_SIZE_Y; y+=sampleOffset.y) {
            // Get connected cells within radius
            LandMass* sampleLandMass = new LandMass();
            LandCell sampleHost = moistureMap[x][y];
            GetConnectedCells(sampleHost, false, *sampleLandMass);
            landmasses.push_back(sampleLandMass);
        }
    }

    // Check landmasses for shared cells
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            LandCell cell = moistureMap[x][y];
            if(cell.connectedMasses.size() == 0) continue;
            LandMass* main = cell.connectedMasses[0];
            if(cell.connectedMasses.size() > 1) {
                for(int i = 1; i < cell.connectedMasses.size(); i++) {
                    cell.connectedMasses[i]->Combine(*main);
                }
            }
            landmasses.push_back(main);
        }
    }

    LandMass* largest = nullptr;
    for(LandMass* mass : landmasses) {
        if(largest == nullptr) {
            largest = mass;
            continue;
        }
        if(mass->Size() > largest->Size())
            largest = mass;
    }

    for(LandMass* mass : landmasses) {
        if(mass == largest) continue;
        for(LandCell cell : mass->cells)
            cell.damp = false;
    }

}

std::vector<std::vector<char>> MoistureGenerationRule::MoistureMapToVector() {
    std::vector<std::vector<char>> root = std::vector<std::vector<char>>();
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        std::vector<char> current = std::vector<char>();
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            current.push_back(moistureMap[x][y].damp ? '#' : '.');
        }
        root.push_back(current);
    }
    return root;
}


void MoistureGenerationRule::MapToForest(Forest *forest) {
//    files::Write2DVectorToFile("MoistureMap.txt", MoistureMapToVector());

    std::ofstream f;
    f.open("MoistureMap.txt");
    for(std::vector<char> v : MoistureMapToVector()) {
        for(char t : v) {
            f << t;
        }
        f << std::endl;
    }
    f.close();

    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            Cell* cell = forest->p_GetCell(x, y);
            cell->damp = moistureMap[x][y].damp;
        }
    }
}

bool MoistureGenerationRule::AreLandCellsConnected(LandCell a, LandCell b, bool allowDiagonal, std::vector<LandCell> visited) {
    if(a == b) return true;

    utils::Point origin = utils::Point{a.x, a.y};
    utils::Point target = utils::Point{b.x, b.y};

    LandCell* current = &a;

    int offsetCount = 4;
    if(allowDiagonal) offsetCount = 8;

    utils::Point offsets[offsetCount];
    offsets[0] = utils::Point{-1,  0};
    offsets[1] = utils::Point{ 1,  0};
    offsets[2] = utils::Point{ 0, -1};
    offsets[3] = utils::Point{ 0,  1};
    if(allowDiagonal) {
        offsets[4] = utils::Point{-1, -1};
        offsets[5] = utils::Point{-1,  1};
        offsets[6] = utils::Point{ 1, -1};
        offsets[7] = utils::Point{ 1,  1};
    }

    for(utils::Point offset : offsets) {
        LandCell cell = moistureMap[origin.x + offset.x][origin.y + offset.y];
        if(!cell.damp) return false;
        if(cell == b) return true;
        if(std::find(visited.begin(), visited.end(), cell) != visited.end()) continue;
        visited.push_back(cell);
        if(AreLandCellsConnected(cell, b)) return true;
    }

    return false;
}

void MoistureGenerationRule::GetConnectedCells(LandCell a, bool allowDiagonal, LandMass visited, int maxDistance) {
    int offsetCount = 4;
    if(allowDiagonal) offsetCount = 8;

    utils::Point origin = utils::Point{a.x, a.y};


    utils::Point offsets[offsetCount];
    offsets[0] = utils::Point{-1,  0};
    offsets[1] = utils::Point{ 1,  0};
    offsets[2] = utils::Point{ 0, -1};
    offsets[3] = utils::Point{ 0,  1};
    if(allowDiagonal) {
        offsets[4] = utils::Point{-1, -1};
        offsets[5] = utils::Point{-1,  1};
        offsets[6] = utils::Point{ 1, -1};
        offsets[7] = utils::Point{ 1,  1};
    }

    for(utils::Point offset : offsets) {
        LandCell cell = moistureMap[utils::clamp(origin.x + offset.x, 0, WORLD_SIZE_X-1)][utils::clamp(origin.y + offset.y, 0, WORLD_SIZE_Y-1)];
        if(std::find(visited.cells.begin(), visited.cells.end(), cell) != visited.cells.end()) continue;
        visited.cells.push_back(cell);
        if(!cell.damp) return;
        if(utils::distanceTo(a.x, a.y, cell.x, cell.y) > maxDistance) return;
        std::cout << "Added cell to landmass: [" << cell.x << ", " << cell.y << "]" << std::endl;
        cell.connectedMasses.push_back(&visited);
        GetConnectedCells(cell, allowDiagonal, visited, maxDistance);
    }

}
