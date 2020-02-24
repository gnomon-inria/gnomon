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
    gnomonActor2DImage
    gnomonActor2DImage.h
    gnomonActor2DImageChannelBlendingWidget
    gnomonActor2DImageChannelBlendingWidget.h
    gnomonActor2DImageRGBA
    gnomonActor2DImageRGBA.h
    gnomonActor2DImageRGBAWidget
    gnomonActor2DImageRGBAWidget.h
    gnomonActor2DImageWidget
    gnomonActor2DImageWidget.h
    gnomonActorImageRGBAVolume
    gnomonActorImageRGBAVolume.h
    gnomonActorImageVolume
    gnomonActorImageVolume.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonActor2DImage.cpp
    gnomonActor2DImageChannelBlendingWidget.cpp
    gnomonActor2DImageRGBA.cpp
    gnomonActor2DImageRGBAWidget.cpp
    gnomonActor2DImageWidget.cpp
    gnomonActorImageRGBAVolume.cpp
    gnomonActorImageVolume.cpp)


######################################################################
### CMakeLists.txt ends here
