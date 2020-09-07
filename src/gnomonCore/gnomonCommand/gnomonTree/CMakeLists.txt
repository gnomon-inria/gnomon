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
    gnomonTreeAdapterCommand
    gnomonTreeAdapterCommand.h
    gnomonTreeConstructorCommand
    gnomonTreeConstructorCommand.h
    gnomonTreeFromLStringCommand
    gnomonTreeFromLStringCommand.h
    gnomonTreeReaderCommand
    gnomonTreeReaderCommand.h
    gnomonTreeTransformCommand
    gnomonTreeTransformCommand.h
    gnomonTreeWriterCommand
    gnomonTreeWriterCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonTreeAdapterCommand.cpp
    gnomonTreeConstructorCommand.cpp
    gnomonTreeFromLStringCommand.cpp
    gnomonTreeReaderCommand.cpp
    gnomonTreeTransformCommand.cpp
    gnomonTreeWriterCommand.cpp)


######################################################################
### CMakeLists.txt ends here
