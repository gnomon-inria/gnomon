project(gnomonCoreTests)

## ###################################################################
## Build tree setup
## ###################################################################

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

## ###################################################################
## Input
## ###################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonDataManagerTest.h
  gnomonDiscreteDynamicFormTest.h
  gnomonFileSystemFormReaderTest.h
  gnomonSystemScenarioTest.h
  gnomonImagesFusionCommandTest.h
  gnomonImagesSerieReaderCommandTest.h)

set(${PROJECT_NAME}_SOURCES
  gnomonDataManagerTest.cpp
  gnomonDiscreteDynamicFormTest.cpp
  gnomonFileSystemFormReaderTest.cpp
  gnomonSystemScenarioTest.cpp
  gnomonImagesFusionCommandTest.cpp
  gnomonImagesSerieReaderCommandTest.cpp)

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
add_test(gnomonSystemScenarioTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonSystemScenarioTest)
add_test(gnomonFileSystemFormReaderTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonFileSystemFormReaderTest)
add_test(gnomonDiscreteDynamicFormTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonDiscreteDynamicFormTest)
add_test(gnomonImagesFusionCommandTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonImagesFusionCommandTest)
add_test(gnomonImagesSerieReaderCommandTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonImagesSerieReaderCommandTest)
