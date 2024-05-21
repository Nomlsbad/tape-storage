add_executable(${APP_NAME}
        ${APP_MAIN}
        ${APP_SOURCES}
)
set_target_properties(${APP_NAME}
        PROPERTIES
        ENABLE_EXPORTS 1
        OUTPUT_NAME "tape-sorter"
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

target_sources(${APP_NAME} INTERFACE ${APP_SOURCES} PUBLIC ${APP_INCLUDE})
target_include_directories(${APP_NAME} PUBLIC ${APP_INCLUDE_PATH})

target_link_libraries(${APP_NAME}
        PUBLIC
        TBB::tbb
        nlohmann_json::nlohmann_json
)

target_compile_options(${APP_NAME}
        PRIVATE
        -DPROJECT_DIR="${APP_PATH}"
)