add_executable(${APP_NAME}
        ${APP_MAIN}
        ${APP_SOURCES}
)

set_target_properties(${APP_NAME}
        PROPERTIES
        OUTPUT_NAME "tape-formatter"
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

target_sources(${APP_NAME} INTERFACE ${APP_SOURCES} PUBLIC ${APP_INCLUDE})

target_link_libraries(${APP_NAME}
        PRIVATE
        tape-storage
)