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
    gnomonPointCloudConstructorCommand
    gnomonPointCloudConstructorCommand.h
    gnomonPointCloudFromImageCommand
    gnomonPointCloudFromImageCommand.h
    gnomonPointCloudQuantificationCommand
    gnomonPointCloudQuantificationCommand.h
    gnomonPointCloudReaderCommand
    gnomonPointCloudReaderCommand.h
    gnomonPointCloudWriterCommand
    gnomonPointCloudWriterCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonPointCloudConstructorCommand.cpp
    gnomonPointCloudFromImageCommand.cpp
    gnomonPointCloudQuantificationCommand.cpp
    gnomonPointCloudReaderCommand.cpp
    gnomonPointCloudWriterCommand.cpp)


######################################################################
### CMakeLists.txt ends here
