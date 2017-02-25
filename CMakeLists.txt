CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
PROJECT(FireSim)


SET(USE_OPENGL TRUE)

SET(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(headers)


INCLUDE_DIRECTORIES(external/include)

IF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    LINK_LIBRARIES(${CMAKE_SOURCE_DIR}/external/lib/panel_win32_mingw.a)
    LINK_LIBRARIES(${CMAKE_SOURCE_DIR}/external/lib/pdcurses_win32_mingw.a)
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
    LINK_LIBRARIES(${CMAKE_SOURCE_DIR}/external/lib/panel_win32_vcc.lib)
    LINK_LIBRARIES(${CMAKE_SOURCE_DIR}/external/lib/pdcurses_win32_vcc.lib)
ELSEIF("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
    LINK_LIBRARIES(${CMAKE_SOURCE_DIR}/external/lib/panel_win32_vcc.lib)
    LINK_LIBRARIES(${CMAKE_SOURCE_DIR}/external/lib/pdcurses_win32_vcc.lib)
ENDIF()


set(CMAKE_BUILD_TYPE RelWithDebInfo)
SET(SRC_FILES source/Forest.cpp source/render/TextRenderer.cpp source/rules/RuleSet.cpp source/Entity.cpp source/Utils.cpp source/render/CursesRenderer.cpp source/render/BufferedRenderer.cpp source/menu/MenuItem.cpp source/menu/MenuHost.cpp source/rules/generation/MoistureGenerationRule.cpp source/file/FileHandler.cpp source/Config.cpp source/WindManager.cpp)

SET(HEAD_FILES)

IF(${USE_OPENGL})
    SET(OPENGL_SUBSYS_ROOT_ABS "${CMAKE_SOURCE_DIR}/subsystems/opengl")
    SET(OPENGL_SUBSYS_ROOT "subsystems/opengl")
    INCLUDE(subsystems/opengl/CMakeLists.txt)
ENDIF()

ADD_EXECUTABLE(FireSim ${SRC_FILES} source/Main.cpp)

INCLUDE(CMakePostBuild.cmake)

MESSAGE("Source Files: ${SRC_FILES}")

FILE(COPY "${CMAKE_SOURCE_DIR}/external/lib/" DESTINATION "${CMAKE_BINARY_DIR}")
