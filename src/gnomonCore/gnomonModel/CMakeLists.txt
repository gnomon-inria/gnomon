## Version: $Id$
##
######################################################################
##
### Commentary:
##
######################################################################
##
### Change Log:
##
######################################################################
##
### Code:



## #################################################################
## Sources
## #################################################################

set(HEADERS
    gnomonAbstractEvolutionModel
    gnomonAbstractEvolutionModel.h
    gnomonAbstractModel
    gnomonAbstractModel.h
    gnomonAbstractSystemScenario
    gnomonAbstractSystemScenario.h
    gnomonAbstractTissueGrowthModel
    gnomonAbstractTissueGrowthModel.h
    gnomonSystem
    gnomonSystem.h)

get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
foreach (FILE ${HEADERS})
    list(APPEND FULL_HEADERS ${DIRECTORY}/${FILE})
endforeach()
string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
string(TOUPPER ${DIRECTORY} DIRECTORY)
set(${PROJECT_NAME}_${DIRECTORY}_HEADERS ${FULL_HEADERS} PARENT_SCOPE)

set(SOURCES
    gnomonAbstractEvolutionModel.cpp
    gnomonAbstractModel.cpp
    gnomonAbstractSystemScenario.cpp
    gnomonAbstractTissueGrowthModel.cpp
    gnomonSystem.cpp)

get_filename_component(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} NAME)
foreach (FILE ${SOURCES})
    list(APPEND FULL_SOURCES ${DIRECTORY}/${FILE})
endforeach()
string(REPLACE "gnomon" "" DIRECTORY ${DIRECTORY})
string(TOUPPER ${DIRECTORY} DIRECTORY)
set(${PROJECT_NAME}_${DIRECTORY}_SOURCES ${FULL_SOURCES} PARENT_SCOPE)

######################################################################
### CMakeLists.txt ends here
