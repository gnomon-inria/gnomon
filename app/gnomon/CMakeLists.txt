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

project(gnomon)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonMainWindow.h)

set(${PROJECT_NAME}_SOURCES
  gnomonMainWindow.cpp)

set(${PROJECT_NAME}_SOURCES_QRC
  gnomon.qrc)

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_RCC ${${PROJECT_NAME}_SOURCES_QRC})

add_library(${PROJECT_NAME}_backend SHARED
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_SOURCES_RCC})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
  main.cpp)

target_link_libraries(${PROJECT_NAME}_backend PUBLIC Qt5::Core)
target_link_libraries(${PROJECT_NAME}_backend PUBLIC Qt5::Widgets)
target_link_libraries(${PROJECT_NAME}_backend PUBLIC Qt5::OpenGL)

target_link_libraries(${PROJECT_NAME}_backend PRIVATE dtkCore)
# target_link_libraries(${PROJECT_NAME}_backend PRIVATE dtkScript)
target_link_libraries(${PROJECT_NAME}_backend PRIVATE dtkWidgets)

# target_link_libraries(${PROJECT_NAME}_backend PRIVATE dtkImagingCore)

target_link_libraries(${PROJECT_NAME}_backend PUBLIC gnomonCore)
target_link_libraries(${PROJECT_NAME}_backend PUBLIC gnomonWidgets)
target_link_libraries(${PROJECT_NAME}_backend PUBLIC gnomonWorkspace)

target_link_libraries(${PROJECT_NAME}_backend PRIVATE ${VTK_LIBRARIES})

target_link_libraries(${PROJECT_NAME} PRIVATE dtkLog)
target_link_libraries(${PROJECT_NAME} PRIVATE dtkWidgets)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_backend)

## #################################################################
## Bundle setup
## #################################################################

if(APPLE)
  set(${PROJECT_NAME}_RESOURCE_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.app/Contents/Resources)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${${PROJECT_NAME}_RESOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.icns ${${PROJECT_NAME}_RESOURCE_DIR})
endif(APPLE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.plist.in ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.plist)

if(APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.plist)
endif(APPLE)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}_backend
  EXPORT gnomonDepends
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(TARGETS ${PROJECT_NAME}
   BUNDLE DESTINATION bin
  RUNTIME DESTINATION bin)

######################################################################
### CMakeLists.txt ends here
