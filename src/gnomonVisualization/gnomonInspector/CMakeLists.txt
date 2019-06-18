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

ADD_GNOMON_HEADERS(
    gnomonInspector
    gnomonInspector.h
    gnomonInspectorCellGraph
    gnomonInspectorCellGraph.h
    gnomonInspectorImage
    gnomonInspectorImage.h
    gnomonInspectorImageProperties
    gnomonInspectorImageProperties.h
    gnomonInspectorMain
    gnomonInspectorMesh
    gnomonInspectorMesh.h
    gnomonInspectorSlicePlanes
    gnomonInspectorSlicePlanes.h
    gnomonInspectorViewTree
    gnomonInspectorViewTree.h
    gnomonInspectorViewWidget
    gnomonInspectorViewWidget.h
    gnomonInspectorVolume
    gnomonInspectorVolume.h)


ADD_GNOMON_SOURCES(
    gnomonInspector.cpp
    gnomonInspectorCellGraph.cpp
    gnomonInspectorImage.cpp
    gnomonInspectorImageProperties.cpp
    gnomonInspectorMesh.cpp
    gnomonInspectorSlicePlanes.cpp
    gnomonInspectorViewTree.cpp
    gnomonInspectorViewWidget.cpp
    gnomonInspectorVolume.cpp)


######################################################################
### CMakeLists.txt ends here
