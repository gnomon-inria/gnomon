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
    gnomonCellComplexConstructorCommand
    gnomonCellComplexConstructorCommand.h
    gnomonCellComplexFromCellImageCommand
    gnomonCellComplexFromCellImageCommand.h
    gnomonCellComplexReaderCommand
    gnomonCellComplexReaderCommand.h
    gnomonCellComplexWriterCommand
    gnomonCellComplexWriterCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonCellComplexConstructorCommand.cpp
    gnomonCellComplexFromCellImageCommand.cpp
    gnomonCellComplexReaderCommand.cpp
    gnomonCellComplexWriterCommand.cpp)


######################################################################
### CMakeLists.txt ends here
