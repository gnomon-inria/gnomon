cmake_minimum_required(VERSION 3.20.0)

######################################################################

project(gnomon
  VERSION 0.81.1
  LANGUAGES C CXX)

## ###################################################################
## cmake modules folder
## ###################################################################

set(LAYER_TARGETS ${PROJECT_NAME}-targets)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

## ###################################################################
## Version setup
## ###################################################################

set(${PROJECT_NAME}_VERSION_MAJOR 0)
set(${PROJECT_NAME}_VERSION_MINOR 81)
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


if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

## #################################################################
## Install prefix
## #################################################################

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if (DEFINED ENV{CONDA_PREFIX})
    set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "${PROJECT_NAME} libdir" FORCE)
    set(CMAKE_INSTALL_PREFIX "$ENV{CONDA_PREFIX}" CACHE PATH "${PROJECT_NAME} install prefix" FORCE)
  else()
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "${PROJECT_NAME} install prefix" FORCE)
  endif()
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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)

if("${isSystemDir}" STREQUAL "-1")
   set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
endif("${isSystemDir}" STREQUAL "-1")

## #################################################################
## Options
## #################################################################

option(GNOMON_ENABLE_VISU "Enable Visualization and workspaces" true)
option(GNOMON_ENABLE_GUI "Enable Gui" true)

## ###################################################################
## Test setup
## ###################################################################

include(CTest)
enable_testing()

## ###################################################################
## Dependencies - cmake
## ###################################################################

include(GenerateExportHeader)

## ###################################################################
## Dependencies
## ###################################################################

set(Python3_FIND_VIRTUALENV FIRST)
if (WIN32)
  # needed when we run cmake in a conda environment
  set(Python3_FIND_REGISTRY LAST)
endif()

find_package(Python3 3.9 EXACT COMPONENTS Interpreter Development NumPy)
set(DTK_PYTHON_INSTALL_PATH "${CMAKE_INSTALL_LIBDIR}/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages")

find_package(Qt6 REQUIRED COMPONENTS Core Concurrent Network Qml Test Xml)
if(QT_KNOWN_POLICY_QTP0001)
  qt_policy(SET QTP0001 OLD)
endif()


find_package(dtkLog      REQUIRED)
find_package(dtkCore     REQUIRED)
find_package(dtkImaging  REQUIRED)
find_package(dtkScript   REQUIRED)
find_package(VTK         REQUIRED) # only for vtkImageData in gnomonImageDtk
find_package(xtensor     REQUIRED) # because of dtkImaging
find_package(libzip         REQUIRED) # for workspaceBrowsing

if(GNOMON_ENABLE_VISU)
  add_definitions(-DGNOMON_USE_VISU)
  find_package(Qt6 REQUIRED COMPONENTS Gui Quick QuickWidgets)
  if(GNOMON_ENABLE_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Qml QuickControls2 WebEngineQuick Widgets)
    find_package(xVis REQUIRED)
  endif()
endif(GNOMON_ENABLE_VISU)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/gnomon)
include_directories(${Python3_INCLUDE_DIRS})

## #################################################################
## Install prefix Modules
## #################################################################

set(CMAKE_MODULES_INSTALL_DIRECTORY ${CMAKE_INSTALL_PREFIX}/${DTK_PYTHON_INSTALL_PATH})

include(gnomonSubdirectoryAddFiles)

## ###################################################################
## Input
## ###################################################################

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(wrp)
if(GNOMON_ENABLE_GUI)
  add_subdirectory(qml)
endif()

add_subdirectory(python)
# add_subdirectory(doc)

add_subdirectory(tst)
add_subdirectory(tst-py)

## ###################################################################
## Export configuration
## ###################################################################

include(CMakePackageConfigHelpers)

set(${PROJECT_NAME}_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE
  STRING "install path for ${PROJECT_NAME}Config.cmake")

set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)
configure_package_config_file(cmake/${PROJECT_NAME}Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "${${PROJECT_NAME}_CMAKE_INSTALL_DIR}"
  PATH_VARS ${PROJECT_NAME}_INCLUDE_DIRS)

set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
configure_package_config_file(cmake/${PROJECT_NAME}Config.cmake.in
  ${PROJECT_BINARY_DIR}/to_install/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION "${${PROJECT_NAME}_CMAKE_INSTALL_DIR}"
  PATH_VARS ${PROJECT_NAME}_INCLUDE_DIRS)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  VERSION ${${PROJECT_NAME}_VERSION}
  COMPATIBILITY AnyNewerVersion)

## ###################################################################
## Exporting
## ###################################################################

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/to_install/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${${PROJECT_NAME}_CMAKE_INSTALL_DIR})

install(EXPORT ${LAYER_TARGETS}
  FILE ${PROJECT_NAME}Targets.cmake
  DESTINATION ${${PROJECT_NAME}_CMAKE_INSTALL_DIR})

export(PACKAGE ${PROJECT_NAME})

## ###################################################################
## Beautifying
## ###################################################################

mark_as_advanced(${PROJECT_NAME}_VERSION_MAJOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_MINOR)
mark_as_advanced(${PROJECT_NAME}_VERSION_BUILD)

mark_as_advanced(Qt6_DIR)
mark_as_advanced(Qt6Concurrent_DIR)
mark_as_advanced(Qt6Core_DIR)
mark_as_advanced(Qt6Declarative_DIR)
mark_as_advanced(Qt6Gui_DIR)
mark_as_advanced(Qt6Network_DIR)
mark_as_advanced(Qt6Qml_DIR)
mark_as_advanced(Qt6Quick_DIR)
mark_as_advanced(Qt6QuickTest_DIR)
mark_as_advanced(Qt6Script_DIR)
mark_as_advanced(Qt6Test_DIR)
mark_as_advanced(Qt6Xml_DIR)
mark_as_advanced(Qt6Sql_DIR)

mark_as_advanced(CMAKE_AR)
mark_as_advanced(CMAKE_BUILD_TYPE)
mark_as_advanced(CMAKE_INSTALL_PREFIX)
mark_as_advanced(CMAKE_OSX_ARCHITECTURES)
mark_as_advanced(CMAKE_OSX_DEPLOYMENT_TARGET)
mark_as_advanced(CMAKE_OSX_SYSROOT)

#omero stuff
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
