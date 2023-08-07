project(xLogger)

include(CMakeDependencies.txt)

## #############################################################################
## Sources
## #############################################################################

set(${PROJECT_NAME}_HEADERS
  xLogger
  xLogger.hpp
  xLoggerConsoleController.hpp
  xLoggerConsoleItem.hpp)

set(${PROJECT_NAME}_SOURCES
  xLogger.cpp
  xLoggerConsoleController.cpp
  xLoggerConsoleItem.cpp)

## #############################################################################
## Build rules
## #############################################################################

add_library(${PROJECT_NAME} STATIC
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt::Gui)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt::Qml)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)

set_target_properties(${PROJECT_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/..")

add_library(x::Logger ALIAS ${PROJECT_NAME})

## #############################################################################
## Install rules
## #############################################################################

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION lib
  RUNTIME DESTINATION lib)
