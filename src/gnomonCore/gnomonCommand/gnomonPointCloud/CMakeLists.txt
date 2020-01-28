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
    gnomonPointCloudFromImageCommand
    gnomonPointCloudFromImageCommand.h
    gnomonPointCloudQuantificationCommand
    gnomonPointCloudQuantificationCommand.h
    gnomonPointCloudReaderCommand
    gnomonPointCloudReaderCommand.h
    gnomonPointCloudWriterCommand
    gnomonPointCloudWriterCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonPointCloudFromImageCommand.cpp
    gnomonPointCloudQuantificationCommand.cpp
    gnomonPointCloudReaderCommand.cpp
    gnomonPointCloudReaderCommand.cpp)


######################################################################
### CMakeLists.txt ends here
