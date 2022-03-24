cmake_minimum_required(VERSION 3.13)

project(JFP VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Shader path
set(SHADER_PATH_ABSOLUTE "${PROJECT_BINARY_DIR}/shaders/")

configure_file(jfp_config.h.in ${CMAKE_BINARY_DIR}/generated/jfp_config.h)

# TinyXML
add_subdirectory(lib/tinyxml2)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw-3.3.6)

# GLM
add_subdirectory(lib/glm)

# Assimp
add_subdirectory(lib/assimp)

# JSBSim
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/jsbsim)

# JFP
add_subdirectory(src)

# JCS
add_subdirectory(JCS)