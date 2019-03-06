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

project(gnomonCore)

## #################################################################
## Inputs
## #################################################################

add_subdirectory(gnomonAlgorithm)
add_subdirectory(gnomonForm)

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonAbstractCommand
  gnomonAbstractCommand.h
  gnomonAbstractEvolutionModel
  gnomonAbstractEvolutionModel.h
  gnomonLandmark.h
  gnomonAbstractModel
  gnomonAbstractModel.h
  gnomonAbstractSystemScenario
  gnomonAbstractSystemScenario.h
  gnomonAbstractTissueGrowthModel
  gnomonAbstractTissueGrowthModel.h
  gnomonCellComplexFromCellImageCommand
  gnomonCellComplexFromCellImageCommand.h
  gnomonCellComplexReaderCommand
  gnomonCellComplexReaderCommand.h
  gnomonCellComplexWriterCommand
  gnomonCellComplexWriterCommand.h
  gnomonCellImageFilterCommand
  gnomonCellImageFilterCommand.h
  gnomonCellImageQuantificationCommand
  gnomonCellImageQuantificationCommand.h
  gnomonCellImageReaderCommand
  gnomonCellImageReaderCommand.h
  gnomonCellImageWriterCommand
  gnomonCellImageWriterCommand.h
  gnomonCore
  gnomonCore.h
  gnomonCoreParameter
  gnomonCoreParameter.h
  gnomonCoreSettings
  gnomonCoreSettings.h
  gnomonDataManager
  gnomonDataManager.h
  gnomonFemSolverCommand
  gnomonFemSolverCommand.h
  gnomonFileSystemFormReader.h
  gnomonFormVisitor
  gnomonFormVisitor.h
  gnomonImagesFusionCommand
  gnomonImagesFusionCommand.h
  gnomonImagesRegistrationCommand
  gnomonImagesRegistrationCommand.h
  gnomonImagesSerieFilterCommand
  gnomonImagesSerieFilterCommand.h
  gnomonImagesSerieReaderCommand
  gnomonImagesSerieReaderCommand.h
  gnomonImagesSerieWriterCommand
  gnomonImagesSerieWriterCommand.h
  gnomonMeshFromImageCommand
  gnomonMeshFromImageCommand.h
  gnomonMeshReaderCommand
  gnomonMeshReaderCommand.h
  gnomonMeshWriterCommand
  gnomonMeshWriterCommand.h
  gnomonSegmentationCommand
  gnomonSegmentationCommand.h
  gnomonSystem
  gnomonSystem.h
  gnomonTypeDef
  gnomonTypeDef.h
  gnomonTime
  gnomonTime.h
  gnomonTreeReaderCommand
  gnomonTreeReaderCommand.h
  ${${PROJECT_NAME}_ALGORITHM_HEADERS}
  ${${PROJECT_NAME}_FORM_HEADERS})

set(${PROJECT_NAME}_SOURCES
  gnomonAbstractEvolutionModel.cpp
  gnomonAbstractModel.cpp
  gnomonAbstractSystemScenario.cpp
  gnomonAbstractTissueGrowthModel.cpp
  gnomonCellComplexFromCellImageCommand.cpp
  gnomonCellComplexReaderCommand.cpp
  gnomonCellComplexWriterCommand.cpp
  gnomonCellImageFilterCommand.cpp
  gnomonCellImageQuantificationCommand.cpp
  gnomonCellImageReaderCommand.cpp
  gnomonCellImageWriterCommand.cpp
  gnomonCore.cpp
  gnomonCoreSettings.cpp
  gnomonCoreParameter.cpp
  gnomonDataManager.cpp
  gnomonFemSolverCommand.cpp
  gnomonFileSystemFormReader.cpp
  gnomonFormVisitor.cpp
  gnomonImagesFusionCommand.cpp
  gnomonImagesRegistrationCommand.cpp
  gnomonImagesSerieFilterCommand.cpp
  gnomonImagesSerieReaderCommand.cpp
  gnomonImagesSerieWriterCommand.cpp
  gnomonMeshFromImageCommand.cpp
  gnomonMeshReaderCommand.cpp
  gnomonMeshWriterCommand.cpp
  gnomonSegmentationCommand.cpp
  gnomonSystem.cpp
  gnomonTime.cpp
  gnomonTreeReaderCommand.cpp
  ${${PROJECT_NAME}_ALGORITHM_SOURCES}
  ${${PROJECT_NAME}_FORM_SOURCES})

## #################################################################
## Wrapping
## #################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${dtk_MODULE_PATH})

set(${PROJECT_NAME}_SOURCES_WRAP)

if(DTK_BUILD_WRAPPERS)

  set(${PROJECT_NAME}_WRAP_DEPENDS ${${PROJECT_NAME}_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/gnomonCore.i.in)

  if (DTK_WRAPPING_PYTHON AND DTK_PYTHON_WRAPPER MATCHES "SWIG")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/gnomonCore.i.in ${CMAKE_BINARY_DIR}/gnomonCore.i @ONLY)
    dtk_wrap(${PROJECT_NAME} ${PROJECT_NAME}_SOURCES_WRAP gnomoncore python ${CMAKE_BINARY_DIR}/gnomonCore.i "${${PROJECT_NAME}_WRAP_DEPENDS}" "${dtk_INCLUDE_DIRS};${PROJECT_SOURCE_DIR}/src/;${dtkImaging_INCLUDE_DIRS};${dtk_WRAP_PREFIX}/dtkBase;${dtk_WRAP_PREFIX}/dtkCore;${dtk_WRAP_PREFIX}/QtCore;${dtkImaging_WRAP_PREFIX}/dtkImagingCore/;${CMAKE_SOURCE_DIR}/src")

    set_property(SOURCE gnomonCore_wrap_python.cpp PROPERTY SKIP_AUTOMOC ON)
  endif()
endif()


## #################################################################
## Build rules
## #################################################################

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES_WRAP}
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS})

## ###################################################################
## Link rules
## ###################################################################

target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

target_link_libraries(${PROJECT_NAME} dtkCore)
target_link_libraries(${PROJECT_NAME} dtkWidgets)

target_link_libraries(${PROJECT_NAME} dtkImagingCore)

if(DTK_BUILD_WRAPPERS)
  if(DTK_WRAPPING_PYTHON)
    target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} vtkWrappingPythonCore)
  endif(DTK_WRAPPING_PYTHON)
endif(DTK_BUILD_WRAPPERS)

# #################################################################
## Target properties
## #################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(${PROJECT_NAME} PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH 0)
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${gnomon_VERSION} SOVERSION ${gnomon_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## #################################################################
## Handling of generated script modules
## #################################################################

set(${PROJECT_NAME}_MODULES)

if(DTK_BUILD_WRAPPERS)

  if(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)

    if(DTK_PYTHON_WRAPPER MATCHES "SWIG")

      add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        DEPENDS ${PROJECT_SOURCE_DIR}/gnomonCore.i
        COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
        COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${${PROJECT_NAME}_BINARY_DIR}/gnomoncore.py ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}
        COMMENT "-- Moving python modules to ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}")

      set(${PROJECT_NAME}_MODULES ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/gnomoncore.py)

      if(APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          DEPENDS ${PROJECT_SOURCE_DIR}/gnomonCore.i
          COMMAND ln -sf ../../libgnomonCore.dylib _gnomoncore.so
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

        set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_gnomoncore.so)
      endif(APPLE)

      if(UNIX AND NOT APPLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
          DEPENDS ${PROJECT_SOURCE_DIR}/gnomonCore.i
          COMMAND ln -fs ../../libgnomonCore.so _gnomoncore.so
          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH})

        set(${PROJECT_NAME}_MODULES ${${PROJECT_NAME}_MODULES} ${CMAKE_BINARY_DIR}/${DTK_PYTHON_INSTALL_PATH}/_gnomoncore.so)
      endif(UNIX AND NOT APPLE)

    endif()

  endif(DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)

endif(DTK_BUILD_WRAPPERS)

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/gnomon
  COMPONENT gnomon-core)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT gnomon-core
         EXPORT gnomonDepends)

## ###################################################################
## Install rules - modules
## ###################################################################

if(SWIG_FOUND)
  if (DTK_WRAPPING_PYTHON AND PYTHONLIBS_FOUND)
    install(FILES ${${PROJECT_NAME}_MODULES} DESTINATION ${DTK_PYTHON_INSTALL_PATH})
    install(FILES ${CMAKE_BINARY_DIR}/gnomonCore.i DESTINATION wrp/gnomonCore/)
  endif()
endif()

######################################################################
### CMakeLists.txt ends here
