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
    gnomonCellImageFilterCommand
    gnomonCellImageFilterCommand.h
    gnomonCellImageQuantificationCommand
    gnomonCellImageQuantificationCommand.h
    gnomonCellImageReaderCommand
    gnomonCellImageReaderCommand.h
    gnomonCellImageWriterCommand
    gnomonCellImageWriterCommand.h
    gnomonSegmentationCommand
    gnomonSegmentationCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonCellImageFilterCommand.cpp
    gnomonCellImageQuantificationCommand.cpp
    gnomonCellImageReaderCommand.cpp
    gnomonCellImageWriterCommand.cpp
    gnomonSegmentationCommand.cpp)


######################################################################
### CMakeLists.txt ends here
