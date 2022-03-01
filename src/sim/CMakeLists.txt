# set(SOURCES
#     JSBSimInterface.cpp
# )

# set(HEADERS
#     IFDM.h
#     JSBSimInterface.h
# )

# add_library(Sim OBJECT ${HEADERS} ${SOURCES})

target_sources(JFP
    PRIVATE
        IFDM.h
        JSBSimInterface.cpp
        JSBSimInterface.h
        FDMData.h
)