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
    gnomonCellImageConstructorCommand
    gnomonCellImageConstructorCommand.h
    gnomonCellImageFilterCommand
    gnomonCellImageFilterCommand.h
    gnomonCellImageQuantificationCommand
    gnomonCellImageQuantificationCommand.h
    gnomonCellImageReaderCommand
    gnomonCellImageReaderCommand.h
    gnomonCellImageTrackingCommand
    gnomonCellImageTrackingCommand.h
    gnomonCellImageWriterCommand
    gnomonCellImageWriterCommand.h
    gnomonSegmentationCommand
    gnomonSegmentationCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonCellImageConstructorCommand.cpp
    gnomonCellImageFilterCommand.cpp
    gnomonCellImageQuantificationCommand.cpp
    gnomonCellImageReaderCommand.cpp
    gnomonCellImageTrackingCommand.cpp
    gnomonCellImageWriterCommand.cpp
    gnomonSegmentationCommand.cpp)


######################################################################
### CMakeLists.txt ends here
