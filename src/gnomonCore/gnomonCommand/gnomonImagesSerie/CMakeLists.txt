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
    gnomonImagesFusionCommand
    gnomonImagesFusionCommand.h
    gnomonImagesRegistrationCommand
    gnomonImagesRegistrationCommand.h
    gnomonImagesSerieFilterCommand
    gnomonImagesSerieFilterCommand.h
    gnomonImagesSerieReaderCommand
    gnomonImagesSerieReaderCommand.h
    gnomonImagesSerieWriterCommand
    gnomonImagesSerieWriterCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonImagesFusionCommand.cpp
    gnomonImagesRegistrationCommand.cpp
    gnomonImagesSerieFilterCommand.cpp
    gnomonImagesSerieReaderCommand.cpp
    gnomonImagesSerieWriterCommand.cpp)


######################################################################
### CMakeLists.txt ends here
