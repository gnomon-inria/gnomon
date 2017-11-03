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

cmake_minimum_required(VERSION 3.6.0)

######################################################################

project(gnomon)

## ###################################################################
## Version setup
## ###################################################################

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 0)
set(${PROJECT_NAME}_VERSION_PATCH 1)
set(${PROJECT_NAME}_VERSION
  ${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH})

## ###################################################################
## Output directory setup
## ###################################################################

include(GNUInstallDirs)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

## #################################################################
## Install prefix
## #################################################################

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "${PROJECT_NAME} install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

## #################################################################
## Generate compilation database
## #################################################################

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

## #################################################################
## Build setup
## #################################################################

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## ###################################################################
## Dependencies - cmake
## ###################################################################

include(GenerateExportHeader)

## #################################################################
## Dependencies - external
## #################################################################

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network Svg Xml)

find_package(dtk REQUIRED)
find_package(dtkImaging REQUIRED)

include_directories(${dtk_INCLUDE_DIRS})
include_directories(${dtkImaging_INCLUDE_DIRS})

find_package(VTK)

if(NOT VTK_FOUND)
  message(WARNING "VTK_DIR not found")
else()
  include(${VTK_USE_FILE})
endif()

## #################################################################
## Dependencies - internal
## #################################################################

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonCore)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonComposer)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonFonts)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonVisualization)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonWidgets)

## ###################################################################
## Input
## ###################################################################

add_subdirectory(src)
add_subdirectory(app)

## ###################################################################
## Exporting
## ###################################################################

export(PACKAGE ${PROJECT_NAME})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.install.cmake.in
  "${PROJECT_BINARY_DIR}/install/${PROJECT_NAME}Config.cmake" @ONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)

install(FILES
  "${CMAKE_BINARY_DIR}/install/${PROJECT_NAME}Config.cmake"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/gnomon)

install(EXPORT gnomonDepends DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/gnomon)

## ###################################################################
## Beautifying
## ###################################################################

mark_as_advanced(${PROJECT_NAME}_VERSION_MAJOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_MINOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_BUILD)

mark_as_advanced(Qt5Concurrent_DIR)
mark_as_advanced(Qt5Core_DIR)
mark_as_advanced(Qt5Declarative_DIR)
mark_as_advanced(Qt5Gui_DIR)
mark_as_advanced(Qt5Network_DIR)
mark_as_advanced(Qt5Qml_DIR)
mark_as_advanced(Qt5Quick_DIR)
mark_as_advanced(Qt5QuickTest_DIR)
mark_as_advanced(Qt5Script_DIR)
mark_as_advanced(Qt5Test_DIR)
mark_as_advanced(Qt5Widgets_DIR)
mark_as_advanced(Qt5Xml_DIR)
mark_as_advanced(Qt5Sql_DIR)

mark_as_advanced(CMAKE_AR)
mark_as_advanced(CMAKE_BUILD_TYPE)
mark_as_advanced(CMAKE_INSTALL_PREFIX)
mark_as_advanced(CMAKE_OSX_ARCHITECTURES)
mark_as_advanced(CMAKE_OSX_DEPLOYMENT_TARGET)
mark_as_advanced(CMAKE_OSX_SYSROOT)

## ###################################################################
## Packaging
## ###################################################################

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")

include(CPack)

######################################################################
### CMakeLists.txt ends here
