set(SOURCES
    JFPParser.cpp
    tinyxml2.cpp
)

set(HEADERS
    JFPParser.h
    tinyxml2.h
)

add_library(Xml OBJECT ${HEADERS} ${SOURCES})