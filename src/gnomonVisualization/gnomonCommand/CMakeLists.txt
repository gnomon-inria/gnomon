## #################################################################
## Inputs
## #################################################################

add_subdirectory(gnomonCellImage)
add_subdirectory(gnomonDataFrame)

## #################################################################
## Sources
## #################################################################

ADD_GNOMON_HEADERS(
        gnomonAbstractVisualizationCommand
        gnomonAbstractVisualizationCommand.h
        gnomonAbstractMplVisualizationCommand
        gnomonAbstractMplVisualizationCommand.h
        gnomonAbstractVtkVisualizationCommand
        gnomonAbstractVtkVisualizationCommand.h
        ${${PROJECT_NAME}_COMMAND_CELLIMAGE_HEADERS}
        ${${PROJECT_NAME}_COMMAND_DATAFRAME_HEADERS}
)


ADD_GNOMON_SOURCES(
        gnomonAbstractVisualizationCommand.cpp
        gnomonAbstractMplVisualizationCommand.cpp
        gnomonAbstractVtkVisualizationCommand.cpp
        ${${PROJECT_NAME}_COMMAND_CELLIMAGE_SOURCES}
        ${${PROJECT_NAME}_COMMAND_DATAFRAME_SOURCES}
)


######################################################################
### CMakeLists.txt ends here
