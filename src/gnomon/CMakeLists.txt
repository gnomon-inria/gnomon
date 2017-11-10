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

execute_process(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GNOMON_GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GNOMON_GIT_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(
  COMMAND git rev-list --count ${GNOMON_GIT_BRANCH}..origin/${GNOMON_GIT_BRANCH}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GNOMON_GIT_UPDATES
  OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gnomonConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/gnomonConfig.h @ONLY)

######################################################################
### CMakeLists.txt ends here
