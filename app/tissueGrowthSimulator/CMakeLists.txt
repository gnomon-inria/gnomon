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

project(tissueGrowthSimulator)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  tissueGrowthSimulatorCodeEditor.h
  tissueGrowthSimulatorComposerWidget.h
  tissueGrowthSimulatorComposerWidget_p.h
  tissueGrowthSimulatorEditorsPanel.h
  tissueGrowthSimulatorMainToolBar.h
  tissueGrowthSimulatorMainWidget.h
  tissueGrowthSimulatorMainWindow.h
  tissueGrowthSimulatorPythonInterpreterWidget.h)

set(${PROJECT_NAME}_SOURCES
  tissueGrowthSimulatorCodeEditor.cpp
  tissueGrowthSimulatorComposerWidget.cpp
  tissueGrowthSimulatorEditorsPanel.cpp
  tissueGrowthSimulatorMainToolBar.cpp
  tissueGrowthSimulatorMainWidget.cpp
  tissueGrowthSimulatorMainWindow.cpp
  tissueGrowthSimulatorPythonInterpreterWidget.cpp
  main.cpp)

## #################################################################
## Build rules
## #################################################################

add_executable(${PROJECT_NAME}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

qt5_use_modules(${PROJECT_NAME} Core)
qt5_use_modules(${PROJECT_NAME} Widgets)
qt5_use_modules(${PROJECT_NAME} OpenGL)

target_link_libraries(${PROJECT_NAME} dtkComposer)
target_link_libraries(${PROJECT_NAME} dtkCore)
target_link_libraries(${PROJECT_NAME} dtkScript)
target_link_libraries(${PROJECT_NAME} dtkWidgets)
target_link_libraries(${PROJECT_NAME} tissueCore)

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME} DESTINATION lib)

######################################################################
### CMakeLists.txt ends here
