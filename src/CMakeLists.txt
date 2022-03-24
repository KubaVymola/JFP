list(APPEND JFP_EXTRA_LIBS libJSBSim)
list(APPEND JFP_EXTRA_LIBS glfw)
list(APPEND JFP_EXTRA_LIBS glm)
list(APPEND JFP_EXTRA_LIBS assimp)
list(APPEND JFP_EXTRA_LIBS tinyxml2)
list(APPEND JFP_EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/src")
list(APPEND JFP_EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/tinyxml2")
list(APPEND JFP_EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/jsbsim/src")
list(APPEND JFP_EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/glad/include")
list(APPEND JFP_EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/glm/glm")
list(APPEND JFP_EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/assimp/include")
list(APPEND JFP_EXTRA_INCLUDES "${PROJECT_BINARY_DIR}/generated")
list(APPEND JFP_EXTRA_SOURCES "${PROJECT_SOURCE_DIR}/lib/glad/src/glad.c")

add_executable(JFP JFP.cpp)

# Extras
target_link_libraries(JFP PRIVATE ${JFP_EXTRA_LIBS})
target_include_directories(JFP PRIVATE "${PROJECT_BINARY_DIR}" ${JFP_EXTRA_INCLUDES})
target_sources(JFP PRIVATE ${JFP_EXTRA_SOURCES})

# Shaders
add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/src/shaders
            ${CMAKE_BINARY_DIR}/shaders
)

add_subdirectory(network)
add_subdirectory(utils)
add_subdirectory(sim)
add_subdirectory(3d-viz)