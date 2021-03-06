//
// Created by Guy on 24/02/2017.
//

#include <rules/generation/MoistureGenerationRule.h>
#include <Forest.h>

MoistureGenerationRule::MoistureGenerationRule(bool dry) : GenerationRule(("moisture_generation" + std::string(dry ? ":dry" : ""))), dry(dry) {}

void MoistureGenerationRule::Generate(Forest *forest) {
    GenerateOriginal();
    for(int i = 0; i < MOISTURE_GENERATION_ITERATIONS; i++)
        Step();
    MapToForest(forest);
}

void MoistureGenerationRule::GenerateOriginal() {
    utils::Point moistureOrigin = utils::Point{utils::random(1, forest->worldSizeX-1), utils::random(1, forest->worldSizeY-1)};
    std::cerr << "Moisture Origin: [" << moistureOrigin.x << ", " << moistureOrigin.y << "]" << std::endl;
    GenerateFromPoint(moistureOrigin.x, moistureOrigin.y);
}

void MoistureGenerationRule::GenerateFromPoint(int ox, int oy, int depth, int px, int py) {
    if(depth > MOISTURE_GENERATION_MAX_SPAWNS) return;
    for(int x = 0; x < forest->worldSizeX; x++) {
        for(int y = 0; y < forest->worldSizeY; y++) {
            moistureMap[x][y].x = x;
            moistureMap[x][y].y = y;
            moistureMap[x][y].damp = false;

            float distance = utils::distanceTo(ox, oy, x, y);
            float radius = MOISTURE_GENERATION_RADIUS / depth;
            if(distance > radius) continue;
            int r = utils::random(0, 100);
            // Linear falloff of initial moisture spawn
            float chance = (INITIAL_MOISTURE_GENERATION_CHANCE * ((radius - distance) / radius));

            if(px > 0 && py > 0) {
                utils::Angle angToOrigin = utils::angle(x, y, ox, oy);
                utils::Angle angToParent = utils::angle(ox, oy, px, py);

                if(angToOrigin.degrees > angToParent.degrees-20 && angToOrigin.degrees < angToParent.degrees+20) {
                    r /= 2;
                }
            }
            bool t = (r < chance);

//            bool t = (r < (INITIAL_MOISTURE_GENERATION_CHANCE));
            moistureMap[x][y].damp = t;

            if(t && distance > MOISTURE_GENERATION_MIN_SPAWN_RANGE && utils::random(0, 100) < MOISTURE_GENERATION_SPAWN_CHANCE) {
                GenerateFromPoint(x, y, depth+1, ox, oy);
            }
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
    for(int x = 1; x < forest->worldSizeX-1; x++) {
        for(int y = 1; y < forest->worldSizeY-1; y++) {
            counter = 0;

            for(utils::Point point : offsets)
                if (moistureMap[x + point.x][y + point.y].damp) counter++;

            if(counter >= 5)
                moistureMap[x][y].damp = true;
        }
    }
}


std::vector<std::vector<char>> MoistureGenerationRule::MoistureMapToVector() {
    std::vector<std::vector<char>> root = std::vector<std::vector<char>>();
    for(int x = 0; x < forest->worldSizeX; x++) {
        std::vector<char> current = std::vector<char>();
        for(int y = 0; y < forest->worldSizeY; y++) {
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

    for(int x = 0; x < forest->worldSizeX; x++) {
        for(int y = 0; y < forest->worldSizeY; y++) {
            Cell cell = forest->GetCell(x, y);
            if(moistureMap[x][y].damp && !cell.states->wall) {
                if(dry) {
                    if(!cell.states->damp)
                    cell.states->dry = true;
                }else{
                    if(!cell.states->dry)
                    cell.states->damp = true;
                }
            }
        }
    }
}

bool LandCell::operator==(const LandCell& other) {
    if(this->x != other.x) return false;
    if(this->y != other.y) return false;
    if(this->damp != other.damp) return false;
    return true;
}
