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


ADD_GNOMON_SUBDIRECTORY_HEADERS(
    gnomonAbstractFemSolver
    gnomonAbstractFemSolver.h
    gnomonAbstractMeshFromImage
    gnomonAbstractMeshFromImage.h
    gnomonAbstractMeshReader
    gnomonAbstractMeshReader.h
    gnomonAbstractMeshWriter
    gnomonAbstractMeshWriter.h)

ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractFemSolver.cpp
    gnomonAbstractMeshFromImage.cpp
    gnomonAbstractMeshReader.cpp
    gnomonAbstractMeshWriter.cpp)


######################################################################
### CMakeLists.txt ends here
