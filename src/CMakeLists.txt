list(APPEND EXTRA_LIBS libJSBSim)
list(APPEND EXTRA_LIBS glfw)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/jsbsim/src")
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/glad/include/")
list(APPEND EXTRA_SOURCES "${PROJECT_SOURCE_DIR}/lib/glad/src/glad.c")

add_executable(JFP JFP.cpp)

# Extras
target_link_libraries(JFP PUBLIC ${EXTRA_LIBS})
target_include_directories(JFP PUBLIC "${PROJECT_BINARY_DIR}" ${EXTRA_INCLUDES})
target_sources(JFP PRIVATE ${EXTRA_SOURCES})

add_subdirectory(network)
add_subdirectory(utils)
add_subdirectory(sim)
add_subdirectory(3d-viz)