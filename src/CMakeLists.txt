list(APPEND EXTRA_LIBS libJSBSim)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/jsbsim/src")

add_executable(JFP JFP.cpp)

target_link_libraries(JFP PUBLIC ${EXTRA_LIBS})
target_link_libraries(JFP PUBLIC glfw)
target_include_directories(JFP PUBLIC "${PROJECT_BINARY_DIR}" ${EXTRA_INCLUDES})

add_subdirectory(network)
add_subdirectory(utils)
add_subdirectory(sim)
# add_subdirectory(control)
# add_subdirectory(viz)