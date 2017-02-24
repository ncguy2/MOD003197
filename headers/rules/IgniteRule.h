//
// Created by Guy on 21/02/2017.
//

#ifndef FIRESIM_IGNITERULE_H
#define FIRESIM_IGNITERULE_H

#include <WindManager.h>
#include "Rules.h"

class IgniteRule : public Rule {
public:
    IgniteRule(bool allowBurn = true) : Rule(std::string("ignition:")+(allowBurn?"burn":"")), allowBurn(allowBurn) {}

    void Execute(Forest *forest, Cell self) override {

        Tree* t = self.tree;

        if(allowBurn && t->IsIgnited()) {
            t->Burn();
            return;
        }

        int burningNeighbours = 0;

        std::vector<utils::Point> offsets = {
                // Adjacent
                utils::Point{ 0, -1},
                utils::Point{ 0,  1},
                utils::Point{-1,  0},
                utils::Point{ 1,  0},
                // Diagonals
                utils::Point{ 1,  1},
                utils::Point{-1,  1},
                utils::Point{-1, -1},
                utils::Point{ 1, -1}
        };

        if(self.states->dry) {
            // Extra north row
            offsets.push_back(utils::Point{-1, -2});
            offsets.push_back(utils::Point{ 0, -2});
            offsets.push_back(utils::Point{ 1, -2});
            // Extra south row
            offsets.push_back(utils::Point{-1,  2});
            offsets.push_back(utils::Point{ 0,  2});
            offsets.push_back(utils::Point{ 1,  2});
            // Extra east column
            offsets.push_back(utils::Point{ 2, -1});
            offsets.push_back(utils::Point{ 2,  0});
            offsets.push_back(utils::Point{ 2,  1});
            // Extra west column
            offsets.push_back(utils::Point{-2, -1});
            offsets.push_back(utils::Point{-2,  0});
            offsets.push_back(utils::Point{-2,  1});
        }

        utils::Point local = self.point;
        for(utils::Point offset : offsets) {
            Cell* cell = forest->p_GetCell(local.x + offset.x, local.y + offset.y);
            if(cell->tree != nullptr) {
                if(cell->tree->IsBurning()) burningNeighbours++;
            }
        }

        if(burningNeighbours == 0) return;

        // TODO improve algorithm for proportional probability
        int modifier = BURNING_IGNITE_MODIFIER;
        for(int i = 0; i < burningNeighbours; i++)
            modifier *= modifier;

        int chance = BASE_IGNITE_CHANCE + (modifier);
        if(self.states->damp)
            chance *= MOISTURE_IGNITION_MITIGATION_FACTOR;
        if(self.states->dry)
            chance *= DRY_IGNITION_AGGRAVATION_FACTOR;

        int val = utils::random(0, 100);


        WindManager windMgr = forest->GetWindManager();
        if(windMgr.GetSpeed() != WindSpeed::NONE) {
            Cell *windSource = nullptr;
            Cell *windTarget = nullptr;
            switch (windMgr.GetDirection()) {
                case WindDirection::NORTH:
                    windSource = forest->p_GetCell(local.x, local.y + 1);
                    windTarget = forest->p_GetCell(local.x, local.y - 1);
                    break;
                case WindDirection::NORTH_EAST:
                    windSource = forest->p_GetCell(local.x + 1, local.y + 1);
                    windTarget = forest->p_GetCell(local.x - 1, local.y - 1);
                    break;
                case WindDirection::EAST:
                    windSource = forest->p_GetCell(local.x + 1, local.y);
                    windTarget = forest->p_GetCell(local.x - 1, local.y);
                    break;
                case WindDirection::SOUTH_EAST:
                    windSource = forest->p_GetCell(local.x + 1, local.y - 1);
                    windTarget = forest->p_GetCell(local.x - 1, local.y + 1);
                    break;
                case WindDirection::SOUTH:
                    windSource = forest->p_GetCell(local.x, local.y - 1);
                    windTarget = forest->p_GetCell(local.x, local.y + 1);
                    break;
                case WindDirection::SOUTH_WEST:
                    windSource = forest->p_GetCell(local.x - 1, local.y - 1);
                    windTarget = forest->p_GetCell(local.x + 1, local.y + 1);
                    break;
                case WindDirection::WEST:
                    windSource = forest->p_GetCell(local.x - 1, local.y);
                    windTarget = forest->p_GetCell(local.x + 1, local.y);
                    break;
                case WindDirection::NORTH_WEST:
                    windSource = forest->p_GetCell(local.x - 1, local.y + 1);
                    windTarget = forest->p_GetCell(local.x + 1, local.y - 1);
                    break;
            }


            if (windSource != nullptr) {
                if (windMgr.GetSpeed() >= 1) {
                    if (windSource->tree->IsBurning())
                        chance *= 5.f;
                }
                if (windTarget != nullptr) {
                    if (windMgr.GetSpeed() >= 2) {
                        if (windSource->tree->IsBurning())
                            if (!windTarget->tree->IsBurning() && !windTarget->tree->IsIgnited()) {
                                if (val < WIND_FIRE_JUMP_CHANCE)
                                    windTarget->tree->Ignite();
                            }
                    }
                }
            }
        }
        if(val < chance) {
            (*t).Ignite();
        }
    }

    bool CanExecute(Cell self) override {
        if(self.tree == nullptr || self.tree->IsBurning() || (self.tree->IsIgnited() && !allowBurn)) return false;
        return Rule::CanExecute(self);
    }

protected:
    bool allowBurn;

};

#endif //FIRESIM_IGNITERULE_H
