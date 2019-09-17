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

project(gnomonVisualization)

## #################################################################
## Inputs
## #################################################################

add_subdirectory(gnomonActor)
add_subdirectory(gnomonInspector)
add_subdirectory(gnomonView)
add_subdirectory(gnomonVisualizations)
add_subdirectory(gnomonVisualizationPlugins)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
    gnomonVisualization
    gnomonVisualization.h
    ${${PROJECT_NAME}_ACTOR_HEADERS}
    ${${PROJECT_NAME}_INSPECTOR_HEADERS}
    ${${PROJECT_NAME}_VIEW_HEADERS}
    ${${PROJECT_NAME}_VISUALIZATIONS_HEADERS})

set(${PROJECT_NAME}_SOURCES
    gnomonVisualization.cpp
    ${${PROJECT_NAME}_ACTOR_SOURCES}
    ${${PROJECT_NAME}_INSPECTOR_SOURCES}
    ${${PROJECT_NAME}_VIEW_SOURCES}
    ${${PROJECT_NAME}_VISUALIZATIONS_SOURCES})

set(${PROJECT_NAME}_SOURCES_QRC
  gnomonVisualization.qrc)

## #############################################################################
##
## #############################################################################

set(layer_BINARY_DIR "${PROJECT_BINARY_DIR}/..")

## #################################################################
## Wrapping
## #################################################################

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${dtk_MODULE_PATH})

# set(${PROJECT_NAME}_SOURCES_WRAP)

# if(DTK_BUILD_WRAPPERS)

#   set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS})

#   if (DTK_WRAPPING_PYTON)
#     if(DTK_PYTHON_WRAPPER MATCHES "SIP")

#       set(${PROJECT_NAME}_SIP_WRAPPERS
#         sip${PROJECT_NAME}gnomonViewMatplotlib.cpp)

#       dtk_wrap_sip(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP gnomonVisualization ${CMAKE_SOURCE_DIR}/sip/gnomonVisualization/gnomonVisualizationmod.sip "${${PROJECT_NAME}_SIP_WRAPPERS}" "${${PROJECT_NAME}_WRAP_DEPENDS}")

#       set_property(SOURCE sipgnomonVisualizationcmodule.cpp                           PROPERTY SKIP_AUTOMOC ON)
#       set_property(SOURCE sipgnomonVisualizationgnomonViewMatplotlib.cpp              PROPERTY SKIP_AUTOMOC ON)
#     endif()

#     if (DTK_PYTHON_WRAPPER MATCHES "SWIG")
#       configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gnomonVisualization.i.in ${CMAKE_BINARY_DIR}/gnomonVisualization.i @ONLY)
#       dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP gnomonvisualization python ${CMAKE_BINARY_DIR}/gnomonVisualization.i "" "${dtk_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}/src/;${dtkImaging_INCLUDE_DIRS};${dtk_WRAP_PREFIX}/dtkBase;${dtk_WRAP_PREFIX}/dtkCore;${dtk_WRAP_PREFIX}/QtCore;${dtk_WRAP_PREFIX}/QtWidgets;${dtk_WRAP_PREFIX}/dtkWidgets;${dtkImaging_WRAP_PREFIX}/dtkImagingCore/;${CMAKE_SOURCE_DIR}/src")

#       set_property(SOURCE gnomonVisualization_wrap_python.cpp PROPERTY SKIP_AUTOMOC ON)
#     endif()
#   endif()
# endif()

## #################################################################
## Build rules
## #################################################################

qt5_add_resources(${PROJECT_NAME}_SOURCES_RCC ${${PROJECT_NAME}_SOURCES_QRC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES_RCC})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Gui)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE dtkFonts)
target_link_libraries(${PROJECT_NAME} PRIVATE dtkThemes)

target_link_libraries(${PROJECT_NAME} PUBLIC dtkVisualization)
target_link_libraries(${PROJECT_NAME} PUBLIC dtkWidgets)

# TODO: Image
#target_link_libraries(${PROJECT_NAME} dtkImagingCore)
#target_link_libraries(${PROJECT_NAME} dtkImagingWidgets)

target_link_libraries(${PROJECT_NAME} PRIVATE gnomonCore)
target_link_libraries(${PROJECT_NAME} PRIVATE gnomonWidgets)

target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${layer_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<BUILD_INTERFACE:${layer_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
  $<INSTALL_INTERFACE:include>)

# if(DTK_BUILD_WRAPPERS)
#   if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
#     target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
#   endif()
# endif()

# #################################################################
## Target properties
## #################################################################

# if(NOT SIP_FOUND)
#   set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
#   set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
# endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${gnomon_VERSION} SOVERSION ${gnomon_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## #################################################################
## Handling of generated script modules
## #################################################################

# set(${PROJECT_NAME}_MODULES)

# if(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

#   if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)

#     if(DTK_PYTHON_WRAPPER MATCHES "SIP")
#       add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#         DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
#         COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
#         COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

#       if(APPLE)
#         add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#           DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
#           COMMAND ln -sf ../../lib${PROJECT_NAME}.dylib ${PROJECT_NAME}.so
#           WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

#         set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/${PROJECT_NAME}.so)
#       endif(APPLE)

#       if(UNIX AND NOT APPLE)
#         add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#           DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
#           COMMAND ln -fs ../../lib${PROJECT_NAME}.so ${PROJECT_NAME}.so
#           WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

#         set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/${PROJECT_NAME}.so)
#       endif(UNIX AND NOT APPLE)
#     endif()

#     if(DTK_PYTHON_WRAPPER MATCHES "SWIG")
#       add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#         DEPENDS ${PROJECT_SOURCE_DIR}/gnomonVisualization.i
#         COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
#         COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_BINARY_DIR}/gnomonvisualization.py ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
#         COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

#       set(${PROJECT_NAME}_MODULES ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomonvisualization.py)

#       if(APPLE)
#         add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#           DEPENDS ${PROJECT_SOURCE_DIR}/gnomonVisualization.i
#           COMMAND ln -sf ../../libgnomonVisualization.dylib _gnomonvisualization.so
#           WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

#         set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_gnomonvisualization.so)
#       endif(APPLE)

#       if(UNIX AND NOT APPLE)
#         add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#           DEPENDS ${PROJECT_SOURCE_DIR}/gnomonVisualization.i
#           COMMAND ln -fs ../../libgnomonVisualization.so _gnomonvisualization.so
#           WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

#         set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_gnomonvisualization.so)
#       endif(UNIX AND NOT APPLE)

#     endif()

#   endif(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND )

# endif(DTK_BUILD_WRAPPERS AND SWIG_FOUND)

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/${PROJECT_NAME})

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME} EXPORT layer-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# ## ###################################################################
# ## Install rules - modules
# ## ###################################################################

# if(SWIG_FOUND)
#   if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
#     install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION ${DTK_PYTHON_INSTALL_PATH})
#     install(FILES ${CMAKE_BINARY_DIR}/gnomonVisualization.i DESTINATION wrp/gnomonVisualization/)
#   endif()
# endif()

######################################################################
### CMakeLists.txt ends here
