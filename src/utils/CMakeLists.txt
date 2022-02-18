# set(SOURCES
#     InputParser.cpp
# )

# set(HEADERS
#     InputParser.h
# )

# add_library(Utils OBJECT ${HEADERS} ${SOURCES})

target_sources(JFP
    PRIVATE
        InputParser.cpp
        InputParser.h
)