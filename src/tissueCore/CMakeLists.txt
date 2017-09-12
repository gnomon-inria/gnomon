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

project(tissueCore)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS)

set(${PROJECT_NAME}_SOURCES)

## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

qt5_use_modules(${PROJECT_NAME} Core)

## #################################################################
## Export header file
## #################################################################

generate_export_header(PROJECT_NAME EXPORT_FILE_NAME "PROJECT_NAMEExport.h")

add_custom_command(TARGET PROJECT_NAME PRE_BUILD
  COMMAND CMAKE_COMMAND ARGS -E copy_if_different "PROJECT_NAME_BINARY_DIR/PROJECT_NAMEExport.h" "CMAKE_BINARY_DIR"
  COMMAND CMAKE_COMMAND ARGS -E copy_if_different "PROJECT_NAME_BINARY_DIR/PROJECT_NAMEExport.h" "CMAKE_BINARY_DIR/PROJECT_NAMEExport")

set(PROJECT_NAME_HEADERS
  PROJECT_NAME_HEADERS
  "CMAKE_BINARY_DIR/PROJECT_NAMEExport"
  "CMAKE_BINARY_DIR/PROJECT_NAMEExport.h")

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

######################################################################
### CMakeLists.txt ends here
