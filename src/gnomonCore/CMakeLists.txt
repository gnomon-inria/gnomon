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
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonAbstractAlgorithm
  gnomonAbstractAlgorithm.h
  gnomonAbstractCellComplexData
  gnomonAbstractCellComplexData.h
  gnomonAbstractCellComplexFromCellImage
  gnomonAbstractCellComplexFromCellImage.h
  gnomonAbstractCellComplexReader
  gnomonAbstractCellComplexReader.h
  gnomonAbstractCellGraphData
  gnomonAbstractCellGraphData.h
  gnomonAbstractCellGraphFromImage
  gnomonAbstractCellGraphFromImage.h
  gnomonAbstractCellImageData
  gnomonAbstractCellImageData.h
  gnomonAbstractCellImageFromImage
  gnomonAbstractCellImageFromImage.h
  gnomonAbstractCellImageReader
  gnomonAbstractCellImageReader.h
  gnomonAbstractCellImageWriter
  gnomonAbstractCellImageWriter.h
  gnomonAbstractCommand
  gnomonAbstractCommand.h
  gnomonAbstractDynamicForm
  gnomonAbstractDynamicForm.h
  gnomonAbstractEvolutionModel
  gnomonAbstractEvolutionModel.h
  gnomonAbstractFemSolver
  gnomonAbstractFemSolver.h
  gnomonAbstractForm
  gnomonAbstractForm.h
  gnomonAbstractImagesFusion
  gnomonAbstractImagesFusion.h
  gnomonAbstractImagesRegistration
  gnomonAbstractImagesRegistration.h
  gnomonAbstractImagesSerieFilter
  gnomonAbstractImagesSerieFilter.h
  gnomonAbstractImagesSerieReader
  gnomonAbstractImagesSerieReader.h
  gnomonAbstractImagesSerieWriter
  gnomonAbstractImagesSerieWriter.h
  gnomonAbstractIntensityImageData
  gnomonAbstractIntensityImageData.h
  gnomonLandmark.h
  gnomonAbstractMeshData
  gnomonAbstractMeshData.h
  gnomonAbstractMeshFromImage
  gnomonAbstractMeshFromImage.h
  gnomonAbstractMeshReader
  gnomonAbstractMeshReader.h
  gnomonAbstractMeshWriter
  gnomonAbstractMeshWriter.h
  gnomonAbstractModel
  gnomonAbstractModel.h
  gnomonAbstractSurfaceImageFromImage
  gnomonAbstractSurfaceImageFromImage.h
  gnomonAbstractSystemScenario
  gnomonAbstractSystemScenario.h
  gnomonAbstractTissueGrowthModel
  gnomonAbstractTissueGrowthModel.h
  gnomonCellComplex
  gnomonCellComplex.h
  gnomonCellComplexReaderCommand
  gnomonCellComplexReaderCommand.h
  gnomonCellGraph
  gnomonCellGraph.h
  gnomonCellImage
  gnomonCellImage.h
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
  gnomonDiscreteDynamicForm
  gnomonDiscreteDynamicForm.h
  gnomonFemSolverCommand
  gnomonFemSolverCommand.h
  gnomonFileSystemFormReader.h
  gnomonFormVisitor
  gnomonFormVisitor.h
  gnomonImagesSerie
  gnomonImagesSerie.h
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
  gnomonIntensityImage
  gnomonIntensityImage.h
  gnomonMesh
  gnomonMesh.h
  gnomonMeshReaderCommand
  gnomonMeshReaderCommand.h
  gnomonMeshWriterCommand
  gnomonMeshWriterCommand.h
  gnomonSegmentationCommand
  gnomonSegmentationCommand.h
  gnomonSphereForm
  gnomonSphereForm.h
  gnomonSystem
  gnomonSystem.h
  gnomonTypeDef
  gnomonTypeDef.h
  gnomonTime
  gnomonTime.h
  gnomonWallForm
  gnomonWallForm.h)

set(${PROJECT_NAME}_SOURCES
  gnomonAbstractAlgorithm.cpp
  gnomonAbstractCellComplexData.cpp
  gnomonAbstractCellComplexFromCellImage.cpp
  gnomonAbstractCellComplexReader.cpp
  gnomonAbstractCellGraphData.cpp
  gnomonAbstractCellGraphFromImage.cpp
  gnomonAbstractCellImageData.cpp
  gnomonAbstractCellImageFromImage.cpp
  gnomonAbstractCellImageReader.cpp
  gnomonAbstractCellImageWriter.cpp
  gnomonAbstractEvolutionModel.cpp
  gnomonAbstractFemSolver.cpp
  gnomonAbstractForm.cpp
  gnomonAbstractImagesFusion.cpp
  gnomonAbstractImagesRegistration.cpp
  gnomonAbstractImagesSerieFilter.cpp
  gnomonAbstractImagesSerieReader.cpp
  gnomonAbstractImagesSerieWriter.cpp
  gnomonAbstractIntensityImageData.cpp
  gnomonAbstractMeshData.cpp
  gnomonAbstractMeshFromImage.cpp
  gnomonAbstractMeshReader.cpp
  gnomonAbstractMeshWriter.cpp
  gnomonAbstractModel.cpp
  gnomonAbstractSurfaceImageFromImage.cpp
  gnomonAbstractSystemScenario.cpp
  gnomonAbstractTissueGrowthModel.cpp
  gnomonCellComplexReaderCommand.cpp
  gnomonCellImageReaderCommand.cpp
  gnomonCellImageWriterCommand.cpp
  gnomonCore.cpp
  gnomonCoreSettings.cpp
  gnomonCoreParameter.cpp
  gnomonDataManager.cpp
  gnomonDiscreteDynamicForm.cpp
  gnomonFemSolverCommand.cpp
  gnomonFileSystemFormReader.cpp
  gnomonFormVisitor.cpp
  gnomonImagesSerie.cpp
  gnomonImagesFusionCommand.cpp
  gnomonImagesRegistrationCommand.cpp
  gnomonImagesSerieFilterCommand.cpp
  gnomonImagesSerieReaderCommand.cpp
  gnomonImagesSerieWriterCommand.cpp
  gnomonMeshReaderCommand.cpp
  gnomonMeshWriterCommand.cpp
  gnomonSegmentationCommand.cpp
  gnomonSphereForm.cpp
  gnomonSystem.cpp
  gnomonTime.cpp
  gnomonWallForm.cpp)

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
