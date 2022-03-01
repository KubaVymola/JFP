# set(SOURCES SocketOutput.cpp)

# set(HEADERS SocketOutput.hpp)

# add_library(Network OBJECT ${HEADERS} ${SOURCES})

target_sources(JFP
    PRIVATE
        SocketOutput.h
        SocketOutput.cpp
        SocketOutputFG.h
        SocketOutputFG.cpp
)

# target_include_directories(myLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})