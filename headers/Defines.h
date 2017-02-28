//
// Created by Guy on 21/02/2017.
//

#include <PropertiesManager.h>

#ifndef cpmax
#define cpmax(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef cpmin
#define cpmin(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES true
#endif
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE true
#endif


#ifndef M_PI
#define M_PI 3.14
#endif

#define WORLD_SIZE_X 71                               // World size, including single-width border
#define WORLD_SIZE_Y 31                             // World size, including single-width border
#define TREE_ALIVE "8"                              // Character to use for living, non-burning trees
#define TREE_IGNITED "&"                            // Character to use for ignited trees
#define TREE_BURNING "x"                            // Character to use for burning trees
#define TREE_DEAD " "                               // Character to use for dead trees/empty cells
#define CELL_DAMP "I"                               // Character to use for damp cells, only used with the block renderer
#define MAX_TREE_LIFE 3.f                             // Amount of updated a burning tree should remain
#define TREE_BASE_GROWTH_CHANCE 0.01f               // The base chance for a tree to regrow on an empty cell
#define TREE_GROWTH_EFFECT 1.f                      // The amount a living tree affects the regrowth chance in adjacent cells
#define MAX_TREE_GROWTH 3                           // The maximum amount of trees that can regrow in a single update
#define GENERATION_RULES_USE_STACK false                       // Whether the simulation rules should be stored on the stack.
#define RULES_USE_STACK false                        // Whether the simulation rules should be stored on the stack.
#define CLEAR_SAFELY false                               // Whether the console should be cleared using "safer" methods of clearing the console. If undefined, system calls are used
#define RENDER_DEBUG true                                // Whether the forest stats should be displayed underneath the forest render
#define RANDOM_STARTPOS true                        // Whether the start position should be randomised each iteration
#define BASE_IGNITE_CHANCE 30                       // The base ignition chance of an adjacent tree
#define BURNING_IGNITE_MODIFIER 2                   // The amount of influence a burning tree has on igniting adjacent trees
#define USE_COLOURS true                            // Whether the console should display in different colours, tested in windows only
#define INITIAL_FIRE_POINTS 3
#define NEIGHBOUR_DETECTION_MODE 1
#define INITIAL_TREE_GENERATION_CHANCE 35
#define INITIAL_MOISTURE_GENERATION_CHANCE 100
#define MOISTURE_GENERATION_RADIUS (cpmin(WORLD_SIZE_X, WORLD_SIZE_Y)*0.6f)
#define MOISTURE_GENERATION_MIN_SPAWN_RANGE MOISTURE_GENERATION_RADIUS * 0.9f
#define MOISTURE_GENERATION_MAX_SPAWNS 5
#define MOISTURE_GENERATION_SPAWN_CHANCE 1
#define ALLOW_TREE_ISLANDS true
#define TREE_GENERATION_ITERATIONS 3
#define MOISTURE_GENERATION_ITERATIONS 20
#define IGNITION_MAX_ATTEMPTS 5
#define MOISTURE_IGNITION_MITIGATION_FACTOR 0.5f
#define DRY_IGNITION_AGGRAVATION_FACTOR 1.5f
#define WIND_FIRE_JUMP_CHANCE 1
#define DRY_COMBUST_CHANCE 1
#define ENABLE_WIND true
#define DAMP_EXTINGUISH_CHANCE 1

// CONSOLE COLOUR DEFINITIONS

#define IGNITED_TREE_COLOUR RED
#define BURNING_TREE_COLOUR LIGHT_RED
#define LIVING_TREE_COLOUR GREEN
#define DEFAULT_TEXT_COLOUR WHITE
#define DEFAULT_BACKGROUND BLACK
#define DAMP_GROUND_COLOUR LIGHT_AQUA
#define DRY_GROUND_COLOUR LIGHT_YELLOW
#define WALL_COLOUR GREY

// CALCULATED DEFINITIONS

#define BORDER_COUNT ((WORLD_SIZE_X-1)*2)+((WORLD_SIZE_Y-1)*2)               // Amount of border cells, Autocalculated

#if RANDOM_STARTPOS
    #define START_POS_X utils::random(1, WORLD_SIZE_X-2)    // The initial fire position, selects a random location in the grid to start the fire
    #define START_POS_Y utils::random(1, WORLD_SIZE_Y-2)    // The initial fire position, selects a random location in the grid to start the fire
#else
    /* The initial fire position, starts the fire in the approximate center of the grid */
    #define START_POS (WORLD_SIZE % 2 != 0 ? (WORLD_SIZE + 1) / 2 : WORLD_SIZE / 2)
#endif


// OS DETECTION

//#define IS_WINDOWS defined(WIN32) || defined(_WIN32) || (defined(__WIN32) && !defined(__CYGWIN__))