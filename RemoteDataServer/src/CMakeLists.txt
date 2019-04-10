SET(TARGET_SRC 
    server.cpp
)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )

SET(TARGET_COMMON_LIBRARIES 
    ${Boost_LIBRARIES}
)

#### end var setup  ###
SETUP_EXAMPLE( remoteDataServer ) 

