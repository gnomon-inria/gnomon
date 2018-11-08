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

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

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

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## ###################################################################
## Test setup
## ###################################################################

include(CTest)

enable_testing()

## ###################################################################
## Dependencies - cmake
## ###################################################################

include(GenerateExportHeader)

## #################################################################
## Dependencies - external
## #################################################################

if(NOT APPLE)
  find_package(OpenSSL)
endif()

find_package(Ice COMPONENTS Ice IceUtil Glacier2 IceXML QUIET)
find_package(omero-ice QUIET)
find_package(omero-client QUIET)

if(Ice_FOUND)
  include_directories(${Ice_INCLUDE_DIRS})
endif()

if (OMERO_CLIENT_FOUND)
  include_directories(${OMERO_CLIENT_INCLUDE_DIR})
endif()

if (OMERO_ICE_FOUND)
  include_directories(${OMERO_ICE_INCLUDE_DIR})
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network Svg Xml Concurrent Test)

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

if(DTK_BUILD_WRAPPERS)
  include(dtkWrap)
endif()

## #################################################################
## Dependencies - internal
## #################################################################

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomon)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonCore)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonStyle)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonComposer)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonFonts)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonOmero)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonVisualization)
include_directories(${CMAKE_SOURCE_DIR}/src/gnomonWidgets)

## ###################################################################
## Input
## ###################################################################

add_subdirectory(src)
add_subdirectory(tst)
add_subdirectory(app)
add_subdirectory(test_python)

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

mark_as_advanced(Qt5_DIR)
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
mark_as_advanced(Qt5OpenGL_DIR)
mark_as_advanced(Qt5Svg_DIR)

mark_as_advanced(CMAKE_AR)
mark_as_advanced(CMAKE_BUILD_TYPE)
mark_as_advanced(CMAKE_INSTALL_PREFIX)
mark_as_advanced(CMAKE_OSX_ARCHITECTURES)
mark_as_advanced(CMAKE_OSX_DEPLOYMENT_TARGET)
mark_as_advanced(CMAKE_OSX_SYSROOT)

mark_as_advanced(Ice_DB_ARCHIVE_EXECUTABLE)
mark_as_advanced(Ice_DB_CHECKPOINT_EXECUTABLE)
mark_as_advanced(Ice_DB_DEADLOCK_EXECUTABLE)
mark_as_advanced(Ice_DB_DUMP_EXECUTABLE)
mark_as_advanced(Ice_DB_HOTBACKUP_EXECUTABLE)
mark_as_advanced(Ice_DB_LOAD_EXECUTABLE)
mark_as_advanced(Ice_DB_LOG_VERIFY_EXECUTABLE)
mark_as_advanced(Ice_DB_PRINTLOG_EXECUTABLE)
mark_as_advanced(Ice_DB_RECOVER_EXECUTABLE)
mark_as_advanced(Ice_DB_STAT_EXECUTABLE)
mark_as_advanced(Ice_DB_TUNER_EXECUTABLE)
mark_as_advanced(Ice_DB_UPGRADE_EXECUTABLE)
mark_as_advanced(Ice_DB_VERIFY_EXECUTABLE)
mark_as_advanced(Ice_DUMPDB_EXECUTABLE)
mark_as_advanced(Ice_GLACIER2ROUTER_EXECUTABLE)
mark_as_advanced(Ice_GLACIER2_LIBRARY_DEBUG)
mark_as_advanced(Ice_GLACIER2_LIBRARY_RELEASE)
mark_as_advanced(Ice_ICEBOXADMIN_EXECUTABLE)
mark_as_advanced(Ice_ICEBOXD_EXECUTABLE)
mark_as_advanced(Ice_ICEBOXNET_EXECUTABLE)
mark_as_advanced(Ice_ICEBOX_EXECUTABLE)
mark_as_advanced(Ice_ICEGRIDADMIN_EXECUTABLE)
mark_as_advanced(Ice_ICEGRIDNODED_EXECUTABLE)
mark_as_advanced(Ice_ICEGRIDNODE_EXECUTABLE)
mark_as_advanced(Ice_ICEGRIDREGISTRYD_EXECUTABLE)
mark_as_advanced(Ice_ICEGRIDREGISTRY_EXECUTABLE)
mark_as_advanced(Ice_ICEPATCH2CALC_EXECUTABLE)
mark_as_advanced(Ice_ICEPATCH2CLIENT_EXECUTABLE)
mark_as_advanced(Ice_ICEPATCH2SERVER_EXECUTABLE)
mark_as_advanced(Ice_ICESERVICEINSTALL_EXECUTABLE)
mark_as_advanced(Ice_ICESTORMADMIN_EXECUTABLE)
mark_as_advanced(Ice_ICESTORMMIGRATE_EXECUTABLE)
mark_as_advanced(Ice_ICEUTIL_LIBRARY_DEBUG)
mark_as_advanced(Ice_ICEUTIL_LIBRARY_RELEASE)
mark_as_advanced(Ice_ICEXML_LIBRARY_DEBUG)
mark_as_advanced(Ice_ICEXML_LIBRARY_RELEASE)
mark_as_advanced(Ice_ICE_LIBRARY_DEBUG)
mark_as_advanced(Ice_ICE_LIBRARY_RELEASE)
mark_as_advanced(Ice_INCLUDE_DIR)
mark_as_advanced(Ice_SLICE2CPP_EXECUTABLE)
mark_as_advanced(Ice_SLICE2CS_EXECUTABLE)
mark_as_advanced(Ice_SLICE2FREEZEJ_EXECUTABLE)
mark_as_advanced(Ice_SLICE2FREEZE_EXECUTABLE)
mark_as_advanced(Ice_SLICE2HTML_EXECUTABLE)
mark_as_advanced(Ice_SLICE2JAVA_EXECUTABLE)
mark_as_advanced(Ice_SLICE2JS_EXECUTABLE)
mark_as_advanced(Ice_SLICE2PHP_EXECUTABLE)
mark_as_advanced(Ice_SLICE2PY_EXECUTABLE)
mark_as_advanced(Ice_SLICE2RB_EXECUTABLE)
mark_as_advanced(Ice_SLICE_DIR)
mark_as_advanced(Ice_TRANSFORMDB_EXECUTABLE)

mark_as_advanced(omero-client_DIR)
mark_as_advanced(omero-ice_DIR)

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
