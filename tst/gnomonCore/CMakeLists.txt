project(gnomonCoreTests)

## ###################################################################
## Build tree setup
## ###################################################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

## ###################################################################
## Input
## ###################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonDataManagerTest.h)

set(${PROJECT_NAME}_SOURCES
  gnomonDataManagerTest.cpp)

## ###################################################################
## Input - introspected
## ###################################################################

create_test_sourcelist(
  ${PROJECT_NAME}_SOURCES_TST
  ${PROJECT_NAME}.cpp
  ${${PROJECT_NAME}_SOURCES})

## ###################################################################
## Build rules
## ###################################################################

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES_TST}
  ${${PROJECT_NAME}_SOURCES})

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} gnomonCore)

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Test)

## ###################################################################
## Test rules
## ###################################################################

add_test(gnomonDataManagerTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonDataManagerTest)
