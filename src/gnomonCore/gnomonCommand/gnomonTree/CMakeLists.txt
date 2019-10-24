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
    gnomonTreeFromLStringCommand
    gnomonTreeFromLStringCommand.h
    gnomonTreeReaderCommand
    gnomonTreeReaderCommand.h
    gnomonTreeTransformCommand
    gnomonTreeTransformCommand.h
    gnomonTreeWriterCommand
    gnomonTreeWriterCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonTreeFromLStringCommand.cpp
    gnomonTreeReaderCommand.cpp
    gnomonTreeTransformCommand.cpp
    gnomonTreeWriterCommand.cpp)


######################################################################
### CMakeLists.txt ends here
