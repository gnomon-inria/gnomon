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
    gnomonAbstractPointCloudFromImage
    gnomonAbstractPointCloudFromImage.h
    gnomonAbstractPointCloudQuantification
    gnomonAbstractPointCloudQuantification.h
    gnomonAbstractPointCloudReader
    gnomonAbstractPointCloudReader.h
    gnomonAbstractPointCloudWriter
    gnomonAbstractPointCloudWriter.h)

ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractPointCloudFromImage.cpp
    gnomonAbstractPointCloudQuantification.cpp
    gnomonAbstractPointCloudReader.cpp
    gnomonAbstractPointCloudWriter.cpp)


######################################################################
### CMakeLists.txt ends here
