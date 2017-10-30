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

project(tissueGrowthSimulator)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  tissueGrowthSimulatorMainWindow.h)

set(${PROJECT_NAME}_SOURCES
  tissueGrowthSimulatorMainWindow.cpp)

## #################################################################
## Wrapping
## #################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${dtk_MODULE_PATH})

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS)

  include(dtkWrap)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS})

  if (DTK_WRAPPING_PYTHON)

    if(DTK_PYTHON_WRAPPER MATCHES "SWIG")

    endif()

    if(DTK_PYTHON_WRAPPER MATCHES "SIP")

      set(${PROJECT_NAME}_SIP_WRAPPERS
        sip${PROJECT_NAME}tissueGrowthSimulatorMainWindow.cpp)

      dtk_wrap_sip(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP tissueGrowthSimulator "${CMAKE_SOURCE_DIR}/sip/tissueGrowthSimulator/tissueGrowthSimulatormod.sip" "${${PROJECT_NAME}_SIP_WRAPPERS}" ${${PROJECT_NAME}_WRAP_DEPENDS})

    endif()

  endif()
endif()

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME}_backend SHARED
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

add_executable(${PROJECT_NAME}
  main.cpp)

qt5_use_modules(${PROJECT_NAME}_backend Core)
qt5_use_modules(${PROJECT_NAME}_backend Widgets)
qt5_use_modules(${PROJECT_NAME}_backend OpenGL)

target_link_libraries(${PROJECT_NAME}_backend dtkCore)
target_link_libraries(${PROJECT_NAME}_backend dtkScript)
target_link_libraries(${PROJECT_NAME}_backend dtkWidgets)
target_link_libraries(${PROJECT_NAME}_backend dtkComposer)

target_link_libraries(${PROJECT_NAME}_backend tissueCore)
target_link_libraries(${PROJECT_NAME}_backend tissueComposer)
target_link_libraries(${PROJECT_NAME}_backend tissueFonts)
target_link_libraries(${PROJECT_NAME}_backend tissueWidgets)

target_link_libraries(${PROJECT_NAME}_backend ${VTK_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_backend)

## #################################################################
## Handling of generated script modules
## #################################################################

set(${PROJECT_NAME}_MODULES)

if(DTK_BUILD_WRAPPERS AND SIP_FOUND)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
      COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/modules
      COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/modules")

    if(APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
        COMMAND ln -sf ../lib/lib${PROJECT_NAME}_backend.dylib ${PROJECT_NAME}.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/modules/${PROJECT_NAME}.so)
    endif(APPLE)

    if(UNIX AND NOT APPLE)
      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${CMAKE_SOURCE_DIR}/sip/${PROJECT_NAME}/${PROJECT_NAME}mod.sip
        COMMAND ln -fs ../${CMAKE_INSTALL_LIBDIR}/lib${PROJECT_NAME}_backend.so ${PROJECT_NAME}.so
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

      set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/modules/${PROJECT_NAME}.so)
    endif(UNIX AND NOT APPLE)

endif(DTK_BUILD_WRAPPERS AND SIP_FOUND)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

######################################################################
### CMakeLists.txt ends here
