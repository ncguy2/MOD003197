//
// Created by Guy on 21/02/2017.
//

#include <Forest.h>
#include <Utils.h>
#include <rules/IgniteRule.h>
#include <rules/CleanupRule.h>
#include <rules/BurnRule.h>

// PUBLIC

Forest::Forest() {
    Populate();
    RegisterRules();
}

void Forest::Update() {
    utils::Point point{0, 0};

    Rule* r = ruleSet.Reset();
    if(r != nullptr) {
        do {
            for(int x = 0; x < WORLD_SIZE; x++) {
                for(int y = 0; y < WORLD_SIZE; y++) {
                    Cell cell = GetCell(x, y);
                    point.x = x; point.y = y;
                    if(r->CanExecute(cell)) {
                        Neighbours n = GetNeighbours(point);
                        r->Execute(this, n);
                    }
                    UpdateCell(cell);
                }
            }
        }while((r = ruleSet.Next()) != nullptr);
    }
}

void Forest::UpdateCell(Cell cell) {
    if(cell.tree != nullptr)
        cell.tree->Update();
}

Cell Forest::GetCell(int x, int y) {
    x = utils::clamp(x, 0, WORLD_SIZE-1);
    y = utils::clamp(y, 0, WORLD_SIZE-1);
    return (cells[x][y]);
}

// PROTECTED

/**
 * Populates the initial forest, placing each Tree object onto the heap
 */
void Forest::Populate() {
    for(int x = 0; x < WORLD_SIZE; x++) {
        for(int y = 0; y < WORLD_SIZE; y++) {
            if(x == 0 || x == WORLD_SIZE-1 || y == 0 || y == WORLD_SIZE-1) {
                cells[x][y].tree = new Tree();
                cells[x][y].tree->Kill();
            }else
                cells[x][y].tree = new Tree();

        }
    }

    int midPoint = WORLD_SIZE;
    if(midPoint % 2 != 0)
        midPoint += 1;
    midPoint /= 2;

    GetCell(midPoint, midPoint).tree->Ignite();
}

/**
 * Registers the simulation rules
 */
void Forest::RegisterRules() {
#ifdef RULES_USE_STACK
    IgniteRule burnRule;
    CleanupRule cleanupRule;

    ruleSet.AddRule(&burnRule);
    ruleSet.AddRule(&cleanupRule);
#else // RULES_USE_HEAP
    ruleSet.AddRule(new IgniteRule());
    ruleSet.AddRule(new CleanupRule());
//    ruleSet.AddRule(new BurnRule());
#endif
}

bool Forest::CanContinue() {
    Cell cell;
    for(int x = 0; x < WORLD_SIZE; x++) {
        for(int y = 0; y < WORLD_SIZE; y++) {
            cell = cells[x][y];
            if(cell.tree != nullptr)
                if(cell.tree->IsBurning() || cell.tree->IsIgnited())
                    return true;
        }
    }
    return false;
}

Neighbours Forest::GetNeighbours(utils::Point p) {
    Neighbours n;

    Cell north = GetCell(p.x,     p.y - 1);
    Cell south = GetCell(p.x,     p.y + 1);
    Cell east  = GetCell(p.x + 1, p.y);
    Cell west  = GetCell(p.x - 1, p.y);

    n.self  = GetCell(p);
    n.north = north;
    n.south = south;
    n.east  = east;
    n.west  = west;

    return n;
}

int Forest::LivingTrees() {
    int living = 0;
    for(int x = 0; x < WORLD_SIZE; x++) {
        for(int y = 0; y < WORLD_SIZE; y++) {
            Cell cell = GetCell(x, y);
            if(cell.tree != nullptr) if(cell.tree->IsAlive()) living++;
        }
    }
    return living;
}

int Forest::IgnitedTrees() {
    int ignited = 0;
    for(int x = 0; x < WORLD_SIZE; x++) {
        for(int y = 0; y < WORLD_SIZE; y++) {
            Cell cell = GetCell(x, y);
            if(cell.tree != nullptr) if(cell.tree->IsIgnited()) ignited++;
        }
    }
    return ignited;
}

int Forest::BurningTrees() {
    int burning = 0;
    for(int x = 0; x < WORLD_SIZE; x++) {
        for(int y = 0; y < WORLD_SIZE; y++) {
            Cell cell = GetCell(x, y);
            if(cell.tree != nullptr) if(cell.tree->IsBurning()) burning++;
        }
    }
    return burning;
}

int Forest::DeadTrees() {
    int dead = 0;
    for(int x = 0; x < WORLD_SIZE; x++) {
        for(int y = 0; y < WORLD_SIZE; y++) {
            Cell cell = GetCell(x, y);
            if(cell.tree != nullptr) if(!cell.tree->IsAlive()) dead++;
        }
    }
    return dead;
}

void Forest::ForEachCell(void (foreach)(utils::Point, Cell)) {
    utils::Point point{0, 0};
    for(int x = 0; x < WORLD_SIZE; x++) {
        for(int y = 0; y < WORLD_SIZE; y++) {
            point.x = x; point.y = y;
            foreach(point, GetCell(point));
        }
    }
}
