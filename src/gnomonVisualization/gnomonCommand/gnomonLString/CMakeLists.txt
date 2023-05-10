## #################################################################
## Sources
## #################################################################

ADD_GNOMON_SUBDIRECTORY_HEADERS(
        gnomonLStringMplVisualizationCommand
        gnomonLStringMplVisualizationCommand.h
        gnomonLStringQmlVisualizationCommand
        gnomonLStringQmlVisualizationCommand.h
        gnomonLStringVtkVisualizationCommand
        gnomonLStringVtkVisualizationCommand.h
)

ADD_GNOMON_SUBDIRECTORY_SOURCES(
        gnomonLStringMplVisualizationCommand.cpp
        gnomonLStringQmlVisualizationCommand.cpp
        gnomonLStringVtkVisualizationCommand.cpp
)

######################################################################
### CMakeLists.txt ends here
