
INCLUDE_DIRECTORIES("${OPENGL_SUBSYS_ROOT}/external/include")
INCLUDE_DIRECTORIES("${OPENGL_SUBSYS_ROOT}/headers")

# WINDOWS
IF(WIN32)
    SET(GLEW "${OPENGL_SUBSYS_ROOT_ABS}/external/lib/glew32s.lib")
    SET(GLFW "${OPENGL_SUBSYS_ROOT_ABS}/external/lib/mingw/libglfw3dll.a")
ELSEIF(UNIX)
    SET(GLEW "${OPENGL_SUBSYS_ROOT_ABS}/external/lib/libGLEW.a")
    SET(GLFW "${OPENGL_SUBSYS_ROOT_ABS}/external/lib/libglfw3.a")

    find_package(X11 REQUIRED)
    IF(${X11_FOUND})
        LINK_LIBRARIES(${X11_LIBRARIES})
        INCLUDE_DIRECTORIES(${X11_INCLUDE_DIR})
        MESSAGE("X11: ${X11_Xrandr_FOUND} | ${X11_Xrandr_LIB}")
    ENDIF()

    LINK_LIBRARIES(dl)
    LINK_LIBRARIES(pthread)
    LINK_LIBRARIES(Xrandr)
    LINK_LIBRARIES(Xi)
    LINK_LIBRARIES(Xxf86vm)
    LINK_LIBRARIES(Xinerama)
    LINK_LIBRARIES(Xcursor)

ENDIF()


LINK_LIBRARIES(${GLFW})
LINK_LIBRARIES(${GLEW})
#LINK_LIBRARIES(${FREETYPE})



find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    link_libraries(${OPENGL_LIBRARIES})
    MESSAGE("OPENGL: ${OPENGL_LIBRARIES}")
endif()

ADD_DEFINITIONS(-DGLEW_STATIC)
ADD_DEFINITIONS(-DWITH_OPENGL)

#FILE(COPY "${CMAKE_SOURCE_DIR}/subsystems/opengl/assets" DESTINATION "${CMAKE_BINARY_DIR}")

SET(SOIL_SRC
        ${OPENGL_SUBSYS_ROOT}/libs/SOIL/SOIL.h
        ${OPENGL_SUBSYS_ROOT}/libs/SOIL/SOIL.c
        ${OPENGL_SUBSYS_ROOT}/libs/SOIL/image_DXT.c
        ${OPENGL_SUBSYS_ROOT}/libs/SOIL/image_DXT.h
        ${OPENGL_SUBSYS_ROOT}/libs/SOIL/image_helper.c
        ${OPENGL_SUBSYS_ROOT}/libs/SOIL/image_helper.h
        ${OPENGL_SUBSYS_ROOT}/libs/SOIL/stb_image_aug.c
        ${OPENGL_SUBSYS_ROOT}/libs/SOIL/stb_image_aug.h
        ${OPENGL_SUBSYS_ROOT}/libs/SOIL/stbi_DDS_aug.h
        ${OPENGL_SUBSYS_ROOT}/libs/SOIL/stbi_DDS_aug_c.h
        )

SET(GL_SUBSYSTEM_FILES ${SOIL_SRC}
        ${OPENGL_SUBSYS_ROOT}/source/OpenGLRenderer.cpp
        ${OPENGL_SUBSYS_ROOT}/source/Texture.cpp
        ${OPENGL_SUBSYS_ROOT}/source/ResourceManager.cpp
        ${OPENGL_SUBSYS_ROOT}/source/SpriteRenderer.cpp
        ${OPENGL_SUBSYS_ROOT}/source/EntityTextureRenderer.cpp
        ${OPENGL_SUBSYS_ROOT}/source/Shader.cpp
        ${OPENGL_SUBSYS_ROOT}/source/FireEffect.cpp
        )

SET(SRC_FILES ${SRC_FILES} ${GL_SUBSYSTEM_FILES})