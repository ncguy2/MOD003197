//
// Created by Guy on 21/02/2017.
//

#include <Forest.h>
#include <Utils.h>
#include <rules/IgniteRule.h>
#include <rules/CleanupRule.h>
#include <rules/GrowthRule.h>
#include <rules/generation/TreeGenerationRule.h>
#include <rules/generation/MoistureGenerationRule.h>
#include <rules/CombustRule.h>
#include <rules/ExtinguishRule.h>

// PUBLIC

Forest::Forest(WindManager manager) : firstPass(true), exit(false), windManager(manager) {
    RegisterDefaultRules();
    RegisterDefaultGenerationRules();
    Populate();
}

void Forest::Update() {

    Rule* r = ruleSet.Reset();
    if(r != nullptr) {
        do {
            ForEachCell([this, r](Cell cell) {
                if(r->CanExecute(cell)) {
                    r->Execute(this, cell);
                }
            });
        }while((r = ruleSet.Next()) != nullptr);
    }

    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            UpdateCell(GetCell(x, y));
        }
    }

    if(firstPass) {
        windManager.Randomise();
        firstPass = false;
        for(int i = 0; i < INITIAL_FIRE_POINTS; i++)
            StartFire();
    }
}

void Forest::StartFire() {
    int attempt = 0;
    Cell cell = GetCell(START_POS_X, START_POS_Y);
    do {
        attempt++;
        if(cell.tree->IsAlive()) cell.tree->Ignite();
        else cell = GetCell(START_POS_X, START_POS_Y);
    }while(attempt < IGNITION_MAX_ATTEMPTS);
}

void Forest::UpdateCell(Cell cell) {
    if(cell.tree != nullptr)
        cell.tree->Update();
}

Cell Forest::GetCell(int x, int y) {
    x = utils::clamp(x, 0, WORLD_SIZE_X-1);
    y = utils::clamp(y, 0, WORLD_SIZE_Y-1);
    return (cells[x][y]);
}

// PROTECTED

/**
 * Populates the initial forest, placing each Tree object onto the heap
 */
void Forest::Populate() {
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            Cell c = Cell{new Tree(), utils::Point{x, y}, new CellStates};
            if(x == 0 || x == WORLD_SIZE_X-1 || y == 0 || y == WORLD_SIZE_Y-1) {
                c.tree->Kill();
                c.tree->Clear();
                c.states->wall = true;
            }
            cells[x][y] = c;
        }
    }


    Rule* r = genRuleSet.Reset();
    if(r != nullptr) {
        do {
            r->Execute(this, cells[0][0]);
        }while((r = genRuleSet.Next()) != nullptr);
    }

}

/**
 * Registers the simulation rules
 */
Forest Forest::RegisterDefaultRules() {
#if RULES_USE_STACK
    IgniteRule burnRule;
    CleanupRule cleanupRule;
    IgniteRule igniteRule(true);
    GrowthRule growthRule;

    ruleSet.AddRule(&burnRule);
    ruleSet.AddRule(&cleanupRule);
    ruleSet.AddRule(&igniteRule);
    ruleSet.AddRule(&growthRule);
#else // RULES_USE_HEAP
    ruleSet.AddRule(new IgniteRule());
    ruleSet.AddRule(new CleanupRule());
    ruleSet.AddRule(new IgniteRule(false));
    ruleSet.AddRule(new GrowthRule());
    ruleSet.AddRule(new CombustRule());
    ruleSet.AddRule(new ExtinguishRule());
#endif
    return *this;
}

bool Forest::CanContinue() {
    if(exit) return false;
    Cell cell;
    if(!firstPass) return true;
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            cell = cells[x][y];
            if(cell.tree != nullptr)
                if(cell.tree->IsBurning() || cell.tree->IsIgnited())
                    return true;
        }
    }
    return false;
}

Neighbours Forest::GetNeighbours(Cell cell) {
    Neighbours n;

    utils::Point p = cell.point;

//    Cell north = GetCell(p.x,     p.y - 1);
//    Cell south = GetCell(p.x,     p.y + 1);
//    Cell east  = GetCell(p.x + 1, p.y);
//    Cell west  = GetCell(p.x - 1, p.y);

    n.self  = cell;
//    n.north = north;
//    n.south = south;
//    n.east  = east;
//    n.west  = west;

    return n;
}

int Forest::LivingTrees() {
    int living = 0;
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            Cell cell = GetCell(x, y);
            if(cell.tree != nullptr) if(cell.tree->IsAlive()) living++;
        }
    }
    return living;
}

int Forest::IgnitedTrees() {
    int ignited = 0;
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            Cell cell = GetCell(x, y);
            if(cell.tree != nullptr) if(cell.tree->IsAlive() && cell.tree->IsIgnited()) ignited++;
        }
    }
    return ignited;
}

int Forest::BurningTrees() {
    int burning = 0;
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            Cell cell = GetCell(x, y);
            if(cell.tree != nullptr) if(cell.tree->IsBurning()) burning++;
        }
    }
    return burning;
}

int Forest::DeadTrees() {
    int dead = 0;
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            Cell cell = GetCell(x, y);
            if(cell.tree != nullptr) if(!cell.tree->IsAlive()) dead++;
        }
    }
    dead -= BORDER_COUNT;
    return dead;
}

int Forest::DampCells() {
    int damp = 0;
    for(int x = 0; x < WORLD_SIZE_X; x++) {
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            Cell cell = GetCell(x, y);
            if(cell.states->damp) damp++;
        }
    }
    return damp;
}

void Forest::processCommand(int cmd) {
    if(cmd == 0x46 || cmd == 102) { // F
        StartFire();
    }else if(cmd == 0x1B) { // Escape (VK_ESCAPE)
        this->exit = true;
    }
}

std::vector<Cell> Forest::AllCells() {
    std::vector<Cell> cellVector;
    for(int x = 0; x < WORLD_SIZE_X; x++)
        for(int y = 0; y < WORLD_SIZE_Y; y++)
            cellVector.push_back(GetCell(x, y));
    return cellVector;
}



template<typename Func>
void Forest::ForEachCell(Func callback) {
    for(int x = 0; x < WORLD_SIZE_X; x++)
        for(int y = 0; y < WORLD_SIZE_Y; y++) {
            try{
                callback(GetCell(x, y));
            }catch(...) {
                std::cout << "Unhandled error detected" << std::endl;
            }
        }
}

Forest Forest::RegisterCustomRules(RuleSet ruleset) {
    this->ruleSet = ruleset;
    return *this;
}

void Forest::Reset() {
    firstPass = true;
    exit = false;
    Populate();
}

Forest Forest::RegisterDefaultGenerationRules() {
#if GENERATION_RULES_USE_STACK

    TreeGenerationRule treeGen;
    MoistureGenerationRule dampGen;
    MoistureGenerationRule dryGen(true);

    genRuleSet.AddRule(&treeGen);
    genRuleSet.AddRule(&dampGen);
    genRuleSet.AddRule(&dryGen);

#else
    genRuleSet.AddRule(new TreeGenerationRule());
    genRuleSet.AddRule(new MoistureGenerationRule());
    genRuleSet.AddRule(new MoistureGenerationRule(true));
#endif
    return *this;
}

Forest Forest::RegisterCustomGenerationRules(RuleSet ruleset) {
    this->genRuleSet = ruleset;
    return *this;
}

Cell *Forest::p_GetCell(int x, int y) {
    Cell c = GetCell(x, y);
    return &c;
}

WindManager Forest::GetWindManager() {
    return this->windManager;
}