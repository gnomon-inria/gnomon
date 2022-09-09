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

ADD_GNOMON_HEADERS(
    gnomonAbstractEvolutionModel
    gnomonAbstractEvolutionModel.h
    gnomonAbstractLStringEvolutionModel
    gnomonAbstractLStringEvolutionModel.h
    gnomonAbstractModel
    gnomonAbstractModel.h
    gnomonAbstractSystemScenario
    gnomonAbstractSystemScenario.h
    gnomonAbstractTissueGrowthModel
    gnomonAbstractTissueGrowthModel.h
    gnomonSystem
    gnomonSystem.h)


ADD_GNOMON_SOURCES(
    gnomonAbstractEvolutionModel.cpp
    gnomonAbstractLStringEvolutionModel.cpp
    gnomonAbstractModel.cpp
    gnomonAbstractSystemScenario.cpp
    gnomonAbstractTissueGrowthModel.cpp
    gnomonSystem.cpp)


######################################################################
### CMakeLists.txt ends here
