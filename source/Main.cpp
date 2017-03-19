//
// Created by Guy on 21/02/2017.
//

#include <Forest.h>
#include <Mains.h>
#include "../external/cpp-properties/include/PropertiesParser.h"
#include <Defines.h>
#include <sstream>

void ReadSafe(const char* file);
void Defaults();

int main() {
    // Load Properties
    ReadSafe("properties.props");
    WindManager manager;
    int worldX = Properties::Instance().GetIntProperty(PROP_WORLD_SIZE_X);
    int worldY = Properties::Instance().GetIntProperty(PROP_WORLD_SIZE_Y);
    Forest* forest = new Forest(manager, worldX, worldY);
    Mains::CreateMenu(forest).Process();
    return 0;
}



void ReadSafe(const char* file) {
    try{
        PropertiesParser::Read(file);
    }catch(PropertiesException pe) {
        // File unreadable, populate default values
        Defaults();
    }
}

void Defaults() {
    Properties::Instance().AddProperty(PROP_WORLD_SIZE_X, cpTOSTRING_INLINE(21));
    Properties::Instance().AddProperty(PROP_WORLD_SIZE_Y, cpTOSTRING_INLINE(21));
    Properties::Instance().AddProperty(PROP_MAX_TREE_LIFE, cpTOSTRING_INLINE(3.f));
    Properties::Instance().AddProperty(PROP_TREE_BASE_GROWTH_CHANCE, cpTOSTRING_INLINE(.01f));
    Properties::Instance().AddProperty(PROP_TREE_GROWTH_EFFECT, cpTOSTRING_INLINE(1.f));
    Properties::Instance().AddProperty(PROP_MAX_TREE_GROWTH, cpTOSTRING_INLINE(3));
    Properties::Instance().AddProperty(PROP_BASE_IGNITE_CHANCE, cpTOSTRING_INLINE(30));
    Properties::Instance().AddProperty(PROP_BURNING_IGNITE_MODIFIER, cpTOSTRING_INLINE(2));
    Properties::Instance().AddProperty(PROP_INITIAL_FIRE_POINTS, cpTOSTRING_INLINE(3));
    Properties::Instance().AddProperty(PROP_INITIAL_TREE_GENERATION_CHANCE, cpTOSTRING_INLINE(35));
    Properties::Instance().AddProperty(PROP_INITIAL_MOISTURE_GENERATION_CHANCE, cpTOSTRING_INLINE(100));
    Properties::Instance().AddProperty(PROP_MOISTURE_GENERATION_MAX_SPAWNS, cpTOSTRING_INLINE(5));
    Properties::Instance().AddProperty(PROP_MOISTURE_GENERATION_SPAWN_CHANCE, cpTOSTRING_INLINE(1));
    Properties::Instance().AddProperty(PROP_ALLOW_TREE_ISLANDS, cpTOSTRING_INLINE(true));
    Properties::Instance().AddProperty(PROP_TREE_GENERATION_ITERATIONS, cpTOSTRING_INLINE(4));
    Properties::Instance().AddProperty(PROP_MOISTURE_GENERATION_ITERATIONS, cpTOSTRING_INLINE(20));
    Properties::Instance().AddProperty(PROP_IGNITION_MAX_ATTEMPTS, cpTOSTRING_INLINE(5));
    Properties::Instance().AddProperty(PROP_MOISTURE_IGNITION_MITIGATION_FACTOR, cpTOSTRING_INLINE(.5f));
    Properties::Instance().AddProperty(PROP_DRY_IGNITION_AGGRAVATION_FACTOR, cpTOSTRING_INLINE(1.5f));
    Properties::Instance().AddProperty(PROP_WIND_FIRE_JUMP_CHANCE, cpTOSTRING_INLINE(1));
    Properties::Instance().AddProperty(PROP_DRY_COMBUST_CHANCE, cpTOSTRING_INLINE(1));
    Properties::Instance().AddProperty(PROP_ENABLE_WIND, cpTOSTRING_INLINE(true));
    Properties::Instance().AddProperty(PROP_DAMP_EXTINGUISH_CHANCE, cpTOSTRING_INLINE(1));
    Properties::Instance().AddProperty(PROP_TREE_MINIMUM_CLUSTER, cpTOSTRING_INLINE(5));
    Properties::Instance().AddProperty(PROP_OPENGL_RENDERER_CELL_SIZE, cpTOSTRING_INLINE(4));
    Properties::Instance().AddProperty(PROP_TREE_ALLOW_REGROWTH, cpTOSTRING_INLINE(true));
    Properties::Instance().AddProperty(PROP_OPENGL_RENDERER_USE_FIRE_SHADER, cpTOSTRING_INLINE(true));
    Properties::Instance().AddProperty(PROP_OPENGL_RENDERER_FIRE_SHADER_BLURS, cpTOSTRING_INLINE(32));
    Properties::Instance().AddProperty(PROP_OPENGL_RENDERER_FIRE_SHADER_SCALE, cpTOSTRING_INLINE(1));
    PropertiesParser::Write("./properties.props", Properties::Instance());
}