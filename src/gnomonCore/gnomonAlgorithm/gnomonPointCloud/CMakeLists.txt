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
    gnomonAbstractPointCloudConstructor
    gnomonAbstractPointCloudConstructor.h
    gnomonAbstractPointCloudFromImage
    gnomonAbstractPointCloudFromImage.h
    gnomonAbstractPointCloudQuantification
    gnomonAbstractPointCloudQuantification.h)

ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractPointCloudConstructor.cpp
    gnomonAbstractPointCloudFromImage.cpp
    gnomonAbstractPointCloudQuantification.cpp)


######################################################################
### CMakeLists.txt ends here
