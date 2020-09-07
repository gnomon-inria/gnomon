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
    gnomonFemSolverCommand
    gnomonFemSolverCommand.h
    gnomonMeshAdapterCommand
    gnomonMeshAdapterCommand.h
    gnomonMeshConstructorCommand
    gnomonMeshConstructorCommand.h
    gnomonMeshFilterCommand
    gnomonMeshFilterCommand.h
    gnomonMeshFromImageCommand
    gnomonMeshFromImageCommand.h
    gnomonMeshReaderCommand
    gnomonMeshReaderCommand.h
    gnomonMeshWriterCommand
    gnomonMeshWriterCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonFemSolverCommand.cpp
    gnomonMeshAdapterCommand.cpp
    gnomonMeshConstructorCommand.cpp
    gnomonMeshFilterCommand.cpp
    gnomonMeshFromImageCommand.cpp
    gnomonMeshReaderCommand.cpp
    gnomonMeshWriterCommand.cpp)


######################################################################
### CMakeLists.txt ends here
