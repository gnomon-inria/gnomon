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
    gnomonImageFilterCommand
    gnomonImageFilterCommand.h
    gnomonImageFusionCommand
    gnomonImageFusionCommand.h
    gnomonImageReaderCommand
    gnomonImageReaderCommand.h
    gnomonImageRegistrationCommand
    gnomonImageRegistrationCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonImageFilterCommand.cpp
    gnomonImageFusionCommand.cpp
    gnomonImageReaderCommand.cpp
    gnomonImageRegistrationCommand.cpp)


######################################################################
### CMakeLists.txt ends here
