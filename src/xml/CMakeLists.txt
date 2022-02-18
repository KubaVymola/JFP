# set(SOURCES
#     JFPParser.cpp
#     tinyxml2.cpp
# )

# set(HEADERS
#     JFPParser.h
#     tinyxml2.h
# )

# add_library(Xml OBJECT ${HEADERS} ${SOURCES})

target_sources(JFP
    PRIVATE
        JFPParser.cpp
        JFPParser.h
        tinyxml2.cpp
        tinyxml2.h
)