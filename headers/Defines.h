//
// Created by Guy on 21/02/2017.
//

#define WORLD_SIZE_X 101                               // World size, including single-width border
#define WORLD_SIZE_Y 51                             // World size, including single-width border
#define TREE_ALIVE "8"                              // Character to use for living, non-burning trees
#define TREE_IGNITED "&"                            // Character to use for ignited trees
#define TREE_BURNING "x"                            // Character to use for burning trees
#define TREE_DEAD " "                               // Character to use for dead trees/empty cells
#define MAX_TREE_LIFE 5                             // Amount of updated a burning tree should remain
#define TREE_BASE_GROWTH_CHANCE 0.01f               // The base chance for a tree to regrow on an empty cell
#define TREE_GROWTH_EFFECT 1.f                      // The amount a living tree affects the regrowth chance in adjacent cells
#define MAX_TREE_GROWTH 3                           // The maximum amount of trees that can regrow in a single update
#define RULES_USE_STACK false                       // Whether the simulation rules should be stored on the stack.
#define CLEAR_SAFELY                                // Whether the console should be cleared using "safer" methods of clearing the console. If undefined, system calls are used
#define RENDER_DEBUG                                // Whether the forest stats should be displayed underneath the forest render
#define RANDOM_STARTPOS true                        // Whether the start position should be randomised each iteration
#define BASE_IGNITE_CHANCE 45                       // The base ignition chance of an adjacent tree
#define BURNING_IGNITE_MODIFIER 5                   // The amount of influence a burning tree has on igniting adjacent trees
#define USE_COLOURS true                            // Whether the console should display in different colours, tested in windows only (uses the termcolour header library)
#define INITIAL_FIRE_POINTS 3
#define NEIGHBOUR_DETECTION_MODE 1
#define INITIAL_TREE_GENERATION_CHANCE 35
#define INITIAL_MOISTURE_GENERATION_CHANCE 35
#define ALLOW_TREE_ISLANDS true
#define TREE_GENERATION_ITERATIONS 3
#define MOISTURE_GENERATION_ITERATIONS 3

// CONSOLE COLOUR DEFINITIONS

#define IGNITED_TREE_COLOUR RED
#define BURNING_TREE_COLOUR LIGHT_RED
#define LIVING_TREE_COLOUR GREEN
#define DEFAULT_TEXT_COLOUR WHITE
#define DEFAULT_BACKGROUND BLACK
#define DAMP_GROUND_COLOUR AQUA

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

#define IS_WINDOWS defined(WIN32) || defined(_WIN32) || (defined(__WIN32) && !defined(__CYGWIN__))