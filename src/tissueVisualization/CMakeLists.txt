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

project(tissueVisualization)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  tissueActor
  tissueActor.h
  tissueActorImage
  tissueActorImage.h
  tissueActorMesh
  tissueActorMesh.h
  tissueActorVolume
  tissueActorVolume.h
  tissueClutEditor
  tissueClutEditor.h
  tissueView
  tissueView.h
  tissueViewManager
  tissueViewManager.h
  tissueVisualization)

set(${PROJECT_NAME}_SOURCES
  tissueActor.cpp
  tissueActorImage.cpp
  tissueActorMesh.cpp
  tissueActorVolume.cpp
  tissueClutEditor.cpp
  tissueView.cpp
  tissueViewManager.cpp)

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Gui)
qt5_use_modules(${PROJECT_NAME} Widgets)

target_link_libraries(${PROJECT_NAME} dtkWidgets)

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${PROJECT_NAME}Export.h")

add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Export.h" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/tissue
  COMPONENT tissue-visualization)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT tissue-visualization
         EXPORT tissueDepends)

######################################################################
### CMakeLists.txt ends here
