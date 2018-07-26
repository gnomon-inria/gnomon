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

project(gnomonGrowthSimulator)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonGrowthSimulatorMainWindow.h)

set(${PROJECT_NAME}_SOURCES
  gnomonGrowthSimulatorMainWindow.cpp)

if(APPLE)
  set(${PROJECT_NAME}_SOURCES ${${PROJECT_NAME}_SOURCES} gnomonGrowthSimulatorMainWindow.mm)
endif(APPLE)

## #################################################################
## Wrapping
## #################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${dtk_MODULE_PATH})

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS})

  if (DTK_WRAPPING_PYTHON)

    if(DTK_PYTHON_WRAPPER MATCHES "SWIG")

    endif()

    if(DTK_PYTHON_WRAPPER MATCHES "SIP")

      set(${PROJECT_NAME}_SIP_WRAPPERS sip${PROJECT_NAME}gnomonGrowthSimulatorMainWindow.cpp)

      dtk_wrap_sip(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP gnomonGrowthSimulator "${CMAKE_SOURCE_DIR}/sip/gnomonGrowthSimulator/gnomonGrowthSimulatormod.sip" "${${PROJECT_NAME}_SIP_WRAPPERS}" ${${PROJECT_NAME}_WRAP_DEPENDS})

      set_property(SOURCE sipgnomonGrowthSimulatorcmodule.cpp PROPERTY SKIP_AUTOMOC ON)
      set_property(SOURCE sipgnomonGrowthSimulatorgnomonGrowthSimulatorMainWindow.cpp PROPERTY SKIP_AUTOMOC ON)

    endif()

  endif()
endif()

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME}_backend SHARED
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_SOURCES_WRAP})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
  main.cpp)

qt5_use_modules(${PROJECT_NAME}_backend Core)
qt5_use_modules(${PROJECT_NAME}_backend Widgets)
qt5_use_modules(${PROJECT_NAME}_backend OpenGL)

target_link_libraries(${PROJECT_NAME}_backend dtkCore)
target_link_libraries(${PROJECT_NAME}_backend dtkScript)
target_link_libraries(${PROJECT_NAME}_backend dtkWidgets)
target_link_libraries(${PROJECT_NAME}_backend dtkComposer)

target_link_libraries(${PROJECT_NAME}_backend gnomonCore)
target_link_libraries(${PROJECT_NAME}_backend gnomonStyle)
target_link_libraries(${PROJECT_NAME}_backend gnomonComposer)
target_link_libraries(${PROJECT_NAME}_backend gnomonFonts)
target_link_libraries(${PROJECT_NAME}_backend gnomonWidgets)

target_link_libraries(${PROJECT_NAME}_backend ${VTK_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_backend)

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

## #################################################################
## Handling of generated script modules
## #################################################################

set(${PROJECT_NAME}_MODULES)

if(DTK_BUILD_WRAPPERS AND SIP_FOUND)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
      COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
      COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

    if(APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
        COMMAND ln -sf ../../lib${PROJECT_NAME}_backend.dylib ${PROJECT_NAME}.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/${PROJECT_NAME}.so)
    endif(APPLE)

    if(UNIX AND NOT APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
        COMMAND ln -fs ../../lib${PROJECT_NAME}_backend.so ${PROJECT_NAME}.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/${PROJECT_NAME}.so)
    endif(UNIX AND NOT APPLE)

endif(DTK_BUILD_WRAPPERS AND SIP_FOUND)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

## ###################################################################
## Install rules - modules
## ###################################################################

if(SWIG_FOUND)
  if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
    install(TARGETS ${PROJECT_NAME}_backend DESTINATION lib)
    install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION ${DTK_PYTHON_INSTALL_PATH})
  endif()
endif()

######################################################################
### CMakeLists.txt ends here
