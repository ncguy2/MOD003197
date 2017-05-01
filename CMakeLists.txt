CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(FireSim)


#ADD_DEFINITIONS(-DWORLD_SIZE_X=71)
#ADD_DEFINITIONS(-DWORLD_SIZE_Y=31)

SET(USE_OPENGL TRUEfastrand)
SET(IS_WINDOWS ${WIN32})
SET(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)

INCLUDE_DIRECTORIES(headers)

INCLUDE_DIRECTORIES(external/include)

SET(SRC_FILES external/cpp-properties/include/PropertiesUtils.h external/cpp-properties/include/PropertiesParser.h external/cpp-properties/include/Properties.h external/cpp-properties/src/Properties.cpp external/cpp-properties/src/PropertiesParser.cpp external/cpp-properties/src/PropertiesUtils.cpp)

IF(MSVC)
    # using Visual Studio C++
    SET(WIN32_VC_SUBSYSTEM_ROOT "subsystems/win32_vc")
    INCLUDE(${WIN32_VC_SUBSYSTEM_ROOT}/CMakeLists.txt)
ELSEIF(UNIX)
    # Using linux
    SET(LINUX_GCC_SUBSYSTEM_ROOT "subsystems/linux_gcc")
    INCLUDE(${LINUX_GCC_SUBSYSTEM_ROOT}/CMakeLists.txt)
ELSEIF(WIN32)
    # Using GCC under windows
    SET(WIN32_GCC_SUBSYSTEM_ROOT "subsystems/win32_gcc")
    INCLUDE(${WIN32_GCC_SUBSYSTEM_ROOT}/CMakeLists.txt)
ENDIF()

IF(IS_WINDOWS)
ELSE()
    SET(IS_WINDOWS FALSE)
ENDIF()

SET(SRC_FILES ${SRC_FILES} source/Forest.cpp source/render/TextRenderer.cpp source/rules/RuleSet.cpp source/Entity.cpp source/Utils.cpp
    source/menu/MenuItem.cpp source/menu/MenuHost.cpp source/rules/generation/MoistureGenerationRule.cpp
    source/file/FileHandler.cpp source/Config.cpp source/WindManager.cpp source/Mains.cpp source/rules/generation/GenerationRule.cpp source/rules/generation/TreeGenerationRule.cpp source/rules/generation/TreeIslandCleanupRule.cpp source/rules/CleanupRule.cpp source/rules/CombustRule.cpp source/rules/ExtinguishRule.cpp source/rules/GrowthRule.cpp source/rules/IgniteRule.cpp source/rules/Rules.cpp)

SET(HEAD_FILES)

IF(${USE_OPENGL})
    SET(OPENGL_SUBSYS_ROOT_ABS "${CMAKE_SOURCE_DIR}/subsystems/opengl")
    SET(OPENGL_SUBSYS_ROOT "subsystems/opengl")
    INCLUDE(subsystems/opengl/CMakeLists.txt)
ENDIF()

ADD_DEFINITIONS(-DIS_WINDOWS=${IS_WINDOWS})

ADD_EXECUTABLE(FireSim ${SRC_FILES} source/Main.cpp)

INCLUDE(CMakePostBuild.cmake)

MESSAGE("Source Files: ${SRC_FILES}")

FILE(COPY "${CMAKE_SOURCE_DIR}/external/lib/" DESTINATION "${CMAKE_BINARY_DIR}")
