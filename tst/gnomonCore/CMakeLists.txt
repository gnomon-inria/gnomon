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
  gnomonFileSystemFormReaderTest.h
  gnomonImageFusionCommandTest.h
  gnomonImageRegistrationCommandTest.h
  gnomonImageFilterCommandTest.h
  gnomonImageReaderCommandTest.h
  gnomonCellImageFromImageCommandTest.h
  gnomonPythonPluginParserTest.h
  gnomonSystemScenarioTest.h
  gnomonTimeSeriesTest.h)

set(${PROJECT_NAME}_SOURCES
  gnomonDataManagerTest.cpp
  gnomonFileSystemFormReaderTest.cpp
  gnomonImageFusionCommandTest.cpp
  gnomonImageRegistrationCommandTest.cpp
  gnomonImageFilterCommandTest.cpp
  gnomonImageReaderCommandTest.cpp
  gnomonCellImageFromImageCommandTest.cpp
  gnomonPythonPluginParserTest.cpp
  gnomonSystemScenarioTest.cpp
  gnomonTimeSeriesTest.cpp)

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

target_link_libraries(${PROJECT_NAME} PUBLIC gnomonCore)

target_link_libraries(${PROJECT_NAME} PRIVATE dtkCore)
target_link_libraries(${PROJECT_NAME} PRIVATE dtkLog)
target_link_libraries(${PROJECT_NAME} PRIVATE dtkScript)
target_link_libraries(${PROJECT_NAME} PRIVATE dtkImagingCore)
target_link_libraries(${PROJECT_NAME} PRIVATE xtensor)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Test)

## ###################################################################
## Test rules
## ###################################################################

add_test(gnomonDataManagerTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonDataManagerTest)
add_test(gnomonFileSystemFormReaderTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonFileSystemFormReaderTest)
add_test(gnomonImageFusionCommandTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonImageFusionCommandTest)
add_test(gnomonImageRegistrationCommandTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonImageRegistrationCommandTest)
add_test(gnomonImageFilterCommandTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonImageFilterCommandTest)
add_test(gnomonImageReaderCommandTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonImageReaderCommandTest)
add_test(gnomonCellImageFromImageCommandTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonCellImageFromImageCommandTest)
add_test(gnomonPythonPluginParserTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonPythonPluginParserTest)
add_test(gnomonSystemScenarioTest ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonSystemScenarioTest)
add_test(gnomonTimeSeriesTest     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} gnomonTimeSeriesTest)
