add_executable(${APP_NAME}
        ${APP_MAIN}
        ${APP_SOURCES}
)
set_target_properties(${APP_NAME} PROPERTIES ENABLE_EXPORTS 1)

target_sources(${APP_NAME} PUBLIC ${APP_SOURCES})
target_include_directories(${APP_NAME} PUBLIC ${APP_INCLUDE_PATH} ${CMAKE_HOME_DIRECTORY})