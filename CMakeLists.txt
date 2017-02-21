CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(FireSim)

SET(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(headers)

SET(SRC_FILES source/Forest.cpp source/render/TextRenderer.cpp source/rules/RuleSet.cpp source/Entity.cpp source/Utils.cpp)
SET(HEAD_FILES)

ADD_EXECUTABLE(FireSim ${SRC_FILES} source/Main.cpp headers/Defines.h)