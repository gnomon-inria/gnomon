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

project(gnomonOmero)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonOmero
  gnomonOmero.h
  gnomonOmeroObject
  gnomonOmeroObject.h
  gnomonOmeroObject_p.h
  gnomonOmeroDataset
  gnomonOmeroDataset.h
  gnomonOmeroImage
  gnomonOmeroImage.h
  gnomonOmeroProject
  gnomonOmeroProject.h
  gnomonOmeroImage
  gnomonOmeroImage.h
)

set(${PROJECT_NAME}_SOURCES
  gnomonOmero.cpp
  gnomonOmeroObject.cpp
  gnomonOmeroDataset.cpp
  gnomonOmeroImage.cpp
  gnomonOmeroProject.cpp
  gnomonOmeroImage.cpp
)

## #################################################################
## Build rules
## #################################################################

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gnomonOmeroConfig.h.in
  ${CMAKE_CURRENT_SOURCE_DIR}/gnomonOmeroConfig.h)
set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  gnomonOmeroConfig.h)

## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

include_directories(${dtk_INCLUDE_DIRS})
include_directories(${dtkImaging_INCLUDE_DIRS})

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)
#target_link_libraries(${PROJECT_NAME} Qt5::Gui)
#target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} dtkCore)
target_link_libraries(${PROJECT_NAME} dtkImaging)
#target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

target_link_libraries(${PROJECT_NAME} gnomonCore)
target_link_libraries(${PROJECT_NAME} ${OMERO_CLIENT_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${OMERO_ICE_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${Ice_LIBRARIES})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(PROJECT_NAME_HEADERS
   ${PROJECT_NAME_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")


## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/gnomon
  COMPONENT gnomon-core)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT gnomon-core
         EXPORT gnomonDepends)

######################################################################
### CMakeLists.txt ends here
