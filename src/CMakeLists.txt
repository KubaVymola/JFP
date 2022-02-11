# add_subdirectory(control)
add_subdirectory(network)
add_subdirectory(utils)
# add_subdirectory(sim)
# add_subdirectory(viz)

list(APPEND EXTRA_LIBS libJSBSim)
list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/jsbsim/src")
# list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/jsbsim/src/initialization")
# list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/jsbsim/src/input_output")
# list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/jsbsim/src/math")
# list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/jsbsim/src/models")
# list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/jsbsim/src/simgear")
# list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib/jsbsim/src/utilities")

add_executable(JFP
    JFP.cpp
    $<TARGET_OBJECTS:Network>
    $<TARGET_OBJECTS:Utils>
)

target_link_libraries(JFP PUBLIC ${EXTRA_LIBS})
target_include_directories(JFP PUBLIC "${PROJECT_BINARY_DIR}" ${EXTRA_INCLUDES})
