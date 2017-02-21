//
// Created by Guy on 21/02/2017.
//


#define CPP_11
#define WORLD_SIZE 21

#define TREE_ALIVE '8'
#define TREE_IGNITED '&'
#define TREE_BURNING 'x'
#define TREE_DEAD ' '
#define MAX_TREE_LIFE 4

//#define RULES_USE_STACK

#define CLEAR_SAFELY
#define IS_WINDOWS defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#define RENDER_DEBUG