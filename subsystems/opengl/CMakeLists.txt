
INCLUDE_DIRECTORIES("${OPENGL_SUBSYS_ROOT}/external/include")
INCLUDE_DIRECTORIES("${OPENGL_SUBSYS_ROOT}/headers")

SET(GLEW "${OPENGL_SUBSYS_ROOT_ABS}/external/lib/glew32s.lib")
SET(GLFW "${OPENGL_SUBSYS_ROOT_ABS}/external/lib/mingw/libglfw3dll.a")
SET(SOIL "${OPENGL_SUBSYS_ROOT_ABS}/external/lib/libsoil.a")
SET(FREETYPE "${OPENGL_SUBSYS_ROOT_ABS}/external/lib/freetype6.dll")

LINK_LIBRARIES(${GLFW})
LINK_LIBRARIES(${GLEW})
LINK_LIBRARIES(${SOIL})
LINK_LIBRARIES(${FREETYPE})

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    link_libraries(${OPENGL_LIBRARIES})
    MESSAGE("OPENGL: ${OPENGL_LIBRARIES}")
endif()

ADD_DEFINITIONS(-DGLEW_STATIC)
ADD_DEFINITIONS(-DWITH_OPENGL)

#FILE(COPY "${CMAKE_SOURCE_DIR}/subsystems/opengl/assets" DESTINATION "${CMAKE_BINARY_DIR}")


SET(GL_SUBSYSTEM_FILES
        ${OPENGL_SUBSYS_ROOT}/source/OpenGLRenderer.cpp
        ${OPENGL_SUBSYS_ROOT}/source/Texture.cpp
        ${OPENGL_SUBSYS_ROOT}/source/ResourceManager.cpp
        ${OPENGL_SUBSYS_ROOT}/source/SpriteRenderer.cpp
        ${OPENGL_SUBSYS_ROOT}/source/EntityTextureRenderer.cpp
        ${OPENGL_SUBSYS_ROOT}/source/Shader.cpp
        )

SET(SRC_FILES ${SRC_FILES} ${GL_SUBSYSTEM_FILES})