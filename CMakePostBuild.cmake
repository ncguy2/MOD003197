
IF(${USE_OPENGL})
    ADD_CUSTOM_COMMAND(TARGET FireSim PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "subsystems/opengl/external/lib"
        $<TARGET_FILE_DIR:FireSim>)

    ADD_CUSTOM_COMMAND(TARGET FireSim POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "subsystems/opengl/assets/"
            $<TARGET_FILE_DIR:FireSim>/assets)

ENDIF()