add_library(JCS SHARED src/JCS.cpp)

set_target_properties(JCS PROPERTIES SOVERSION 1)
target_include_directories(JCS PRIVATE include)

add_subdirectory(src)
add_subdirectory(include)