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
    gnomonAbstractImagesFusion
    gnomonAbstractImagesFusion.h
    gnomonAbstractImagesRegistration
    gnomonAbstractImagesRegistration.h
    gnomonAbstractImagesSerieFilter
    gnomonAbstractImagesSerieFilter.h
    gnomonAbstractImagesSerieReader
    gnomonAbstractImagesSerieReader.h
    gnomonAbstractImagesSerieWriter
    gnomonAbstractImagesSerieWriter.h
    gnomonAbstractSurfaceImageFromImage
    gnomonAbstractSurfaceImageFromImage.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonAbstractImagesFusion.cpp
    gnomonAbstractImagesRegistration.cpp
    gnomonAbstractImagesSerieFilter.cpp
    gnomonAbstractImagesSerieReader.cpp
    gnomonAbstractImagesSerieWriter.cpp
    gnomonAbstractSurfaceImageFromImage.cpp)


######################################################################
### CMakeLists.txt ends here
