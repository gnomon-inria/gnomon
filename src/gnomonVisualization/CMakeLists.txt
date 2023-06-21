project(gnomonVisualization VERSION ${gnomon_VERSION})

## #################################################################
## Inputs
## #################################################################

add_subdirectory(gnomonActor)
add_subdirectory(gnomonCommand)
# add_subdirectory(gnomonInspector)
add_subdirectory(gnomonInteractorStyle)
add_subdirectory(gnomonManager)
add_subdirectory(gnomonView)
add_subdirectory(gnomonVisualizations)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
    gnomonVisualization
    gnomonVisualization.h

    gnomonCoreParameterColor # NOTE: Transitional ?
    gnomonCoreParameterColor.h # NOTE: Transitional ?
    gnomonCoreParameterColorObject # NOTE: Transitional ?
    gnomonCoreParameterColorObject.h # NOTE: Transitional ?
    gnomonCoreParameterColorTable
    gnomonCoreParameterColorTable.h
    gnomonCoreParameterColorTableObject
    gnomonCoreParameterColorTableObject.h
    gnomonLookupTable.h # NOTE: Transitional ?
    gnomonMaterial.h
    gnomonCoreParameterLookupTable # NOTE: Transitional ?
    gnomonCoreParameterLookupTable.h # NOTE: Transitional ?
    gnomonCoreParameterLookupTableObject # NOTE: Transitional ?
    gnomonCoreParameterLookupTableObject.h # NOTE: Transitional ?
    gnomonCoreParameterGraphical
    gnomonCoreParameterGraphical.h
    gnomonCoreParameterGraphicalObject
    gnomonCoreParameterGraphicalObject.h
    gnomonCoreParameterNurbs.h
    gnomonCoreParameterNurbsObject.h


    ${${PROJECT_NAME}_ACTOR_HEADERS}
    ${${PROJECT_NAME}_COMMAND_HEADERS}
#   ${${PROJECT_NAME}_INSPECTOR_HEADERS}
    ${${PROJECT_NAME}_INTERACTORSTYLE_HEADERS}
    ${${PROJECT_NAME}_MANAGER_HEADERS}
    ${${PROJECT_NAME}_VIEW_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_HEADERS})

set(${PROJECT_NAME}_SOURCES
    gnomonVisualization.cpp

    gnomonCoreParameterColor.cpp # NOTE: Transitional ?
    gnomonCoreParameterColorObject.cpp # NOTE: Transitional ?
    gnomonCoreParameterColorTable.cpp
    gnomonCoreParameterColorTableObject.cpp
    gnomonLookupTable.cpp # NOTE: Transitional ?
    gnomonCoreParameterLookupTable.cpp # NOTE: Transitional ?
    gnomonCoreParameterLookupTableObject.cpp # NOTE: Transitional ?
    gnomonCoreParameterGraphical.cpp
    gnomonCoreParameterGraphicalObject.cpp
    gnomonCoreParameterNurbs.cpp
    gnomonCoreParameterNurbsObject.cpp


    ${${PROJECT_NAME}_ACTOR_SOURCES}
    ${${PROJECT_NAME}_COMMAND_SOURCES}
#   ${${PROJECT_NAME}_INSPECTOR_SOURCES}
    ${${PROJECT_NAME}_INTERACTORSTYLE_SOURCES}
    ${${PROJECT_NAME}_MANAGER_SOURCES}
    ${${PROJECT_NAME}_VIEW_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_SOURCES})

set(${PROJECT_NAME}_SOURCES_QRC
  gnomonVisualization.qrc)

## #############################################################################
##
## #############################################################################

set(layer_BINARY_DIR "${PROJECT_BINARY_DIR}/..")

## #################################################################
## Build rules
## #################################################################

qt6_add_resources(${PROJECT_NAME}_SOURCES_RCC ${${PROJECT_NAME}_SOURCES_QRC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES_RCC})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Gui)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Xml)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Qml)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Quick)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::QuickWidgets)

target_link_libraries(${PROJECT_NAME} PRIVATE dtkScript)
target_link_libraries(${PROJECT_NAME} PUBLIC dtkImagingCore)

target_link_libraries(${PROJECT_NAME} PRIVATE gnomonCore)
target_link_libraries(${PROJECT_NAME} PRIVATE gnomonPipeline)

target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${layer_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<BUILD_INTERFACE:${layer_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
  $<INSTALL_INTERFACE:include>)

# #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${gnomon_VERSION} SOVERSION ${gnomon_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

install( FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"  DESTINATION include/${PROJECT_NAME})
install( FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h"  DESTINATION include/${PROJECT_NAME})

## ###################################################################
## Install rules - files
## ###################################################################

foreach ( file ${${PROJECT_NAME}_HEADERS} )
    get_filename_component( dir ${file} DIRECTORY )
    install( FILES ${file} DESTINATION include/${PROJECT_NAME}/${dir} )
endforeach()

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME} EXPORT ${LAYER_TARGETS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

######################################################################
### CMakeLists.txt ends here
