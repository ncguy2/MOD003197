//
// Created by Guy on 21/02/2017.
//

#include <Forest.h>
#include <Mains.h>
#include "../external/cpp-properties/include/PropertiesParser.h"
#include <Defines.h>


int main() {
    // Load Properties
    try{
        PropertiesParser::Read("properties.props");
    }catch(PropertiesException pe) {
        // File unreadable, populate default values
        Properties::Instance().AddProperty(PROP_WORLD_SIZE_X, std::to_string(21));
        Properties::Instance().AddProperty(PROP_WORLD_SIZE_Y, std::to_string(21));
        Properties::Instance().AddProperty(PROP_MAX_TREE_LIFE, std::to_string(3.f));
        Properties::Instance().AddProperty(PROP_TREE_BASE_GROWTH_CHANCE, std::to_string(.01f));
        Properties::Instance().AddProperty(PROP_TREE_GROWTH_EFFECT, std::to_string(1.f));
        Properties::Instance().AddProperty(PROP_MAX_TREE_GROWTH, std::to_string(3));
        Properties::Instance().AddProperty(PROP_BASE_IGNITE_CHANCE, std::to_string(30));
        Properties::Instance().AddProperty(PROP_BURNING_IGNITE_MODIFIER, std::to_string(2));
        Properties::Instance().AddProperty(PROP_INITIAL_FIRE_POINTS, std::to_string(3));
        Properties::Instance().AddProperty(PROP_INITIAL_TREE_GENERATION_CHANCE, std::to_string(35));
        Properties::Instance().AddProperty(PROP_INITIAL_MOISTURE_GENERATION_CHANCE, std::to_string(100));
        Properties::Instance().AddProperty(PROP_MOISTURE_GENERATION_MAX_SPAWNS, std::to_string(5));
        Properties::Instance().AddProperty(PROP_MOISTURE_GENERATION_SPAWN_CHANCE, std::to_string(1));
        Properties::Instance().AddProperty(PROP_ALLOW_TREE_ISLANDS, std::to_string(true));
        Properties::Instance().AddProperty(PROP_TREE_GENERATION_ITERATIONS, std::to_string(4));
        Properties::Instance().AddProperty(PROP_MOISTURE_GENERATION_ITERATIONS, std::to_string(20));
        Properties::Instance().AddProperty(PROP_IGNITION_MAX_ATTEMPTS, std::to_string(5));
        Properties::Instance().AddProperty(PROP_MOISTURE_IGNITION_MITIGATION_FACTOR, std::to_string(.5f));
        Properties::Instance().AddProperty(PROP_DRY_IGNITION_AGGRAVATION_FACTOR, std::to_string(1.5f));
        Properties::Instance().AddProperty(PROP_WIND_FIRE_JUMP_CHANCE, std::to_string(1));
        Properties::Instance().AddProperty(PROP_DRY_COMBUST_CHANCE, std::to_string(1));
        Properties::Instance().AddProperty(PROP_ENABLE_WIND, std::to_string(true));
        Properties::Instance().AddProperty(PROP_DAMP_EXTINGUISH_CHANCE, std::to_string(1));
        Properties::Instance().AddProperty(PROP_TREE_MINIMUM_CLUSTER, std::to_string(5));
        Properties::Instance().AddProperty(PROP_OPENGL_RENDERER_CELL_SIZE, std::to_string(4));
        PropertiesParser::Write("./properties.props", Properties::Instance());
    }

    Properties props = Properties::Instance();

    WindManager manager;
    int worldX = Properties::Instance().GetIntProperty(PROP_WORLD_SIZE_X);
    int worldY = Properties::Instance().GetIntProperty(PROP_WORLD_SIZE_Y);
    Forest* forest = new Forest(manager, worldX, worldY);
    Mains::CreateMenu(forest).Process();
    return 0;
}