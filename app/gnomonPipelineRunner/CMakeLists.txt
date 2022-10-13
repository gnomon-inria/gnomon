project(gnomonPipelineRunner)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_SOURCES
  main.cpp)

## #################################################################
## Build rules
## #################################################################

link_directories(${CMAKE_BINARY_DIR}/lib)

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0) 
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE
  dtkCore
  dtkLog
  gnomonCore
  gnomonPipeline
  Python3::Python)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)


## #################################################################
## Installation
## #################################################################

install(TARGETS ${PROJECT_NAME}
   BUNDLE DESTINATION bin
  RUNTIME DESTINATION bin)

