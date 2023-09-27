project(gnomonWorkspace VERSION ${gnomon_VERSION})

## #################################################################
## Sources
## #################################################################

set(${PROJECT_NAME}_HEADERS
  gnomonPythonAlgorithmPluginCode
  gnomonPythonAlgorithmPluginCode.h
  gnomonAbstractWorkspace.h
  gnomonAlgorithmWorkspace
  gnomonAlgorithmWorkspace.h
  gnomonAlgorithmWorkspace_p.h
  gnomonWorkspace
  gnomonWorkspaceBinarization
  gnomonWorkspaceBinarization.h
  gnomonWorkspaceBrowser
  gnomonWorkspaceBrowser.h
  # gnomonWorkspaceDatabaseBrowser.h
  # gnomonWorkspaceCellComplexConstructor
  # gnomonWorkspaceCellComplexConstructor.h
  # gnomonWorkspaceCellComplexFromCellImage
  # gnomonWorkspaceCellComplexFromCellImage.h
  # gnomonWorkspaceCellImageConstructor
  # gnomonWorkspaceCellImageConstructor.h
  # gnomonWorkspaceCellImageFilter
  # gnomonWorkspaceCellImageFilter.h
  gnomonWorkspaceCellImageQuantification
  gnomonWorkspaceCellImageQuantification.h
  gnomonWorkspaceCellImageTracking
  gnomonWorkspaceCellImageTracking.h
  # gnomonWorkspaceFusion
  # gnomonWorkspaceFusion.h
  # gnomonWorkspaceImageConstructor
  # gnomonWorkspaceImageConstructor.h
  gnomonWorkspaceImageMeshing
  gnomonWorkspaceImageMeshing.h
  # gnomonWorkspaceLStringTranslation
  # gnomonWorkspaceLStringTranslation.h
  gnomonWorkspaceLSystemModel
  gnomonWorkspaceLSystemModel.h
  # gnomonWorkspaceLSystemSimulator
  # gnomonWorkspaceLSystemSimulator.h
  # gnomonWorkspaceMeshConstructor
  # gnomonWorkspaceMeshConstructor.h
  gnomonWorkspaceMeshFilter
  gnomonWorkspaceMeshFilter.h
  gnomonWorkspaceMorphonet
  gnomonWorkspaceMorphonet.h
  # gnomonWorkspacePlantScan3D
  # gnomonWorkspacePlantScan3D.h
  # gnomonWorkspacePointCloudConstructor
  # gnomonWorkspacePointCloudConstructor.h
  gnomonWorkspacePointDetection
  gnomonWorkspacePointDetection.h
  gnomonWorkspacePointCloudQuantification
  gnomonWorkspacePointCloudQuantification.h
  gnomonWorkspacePreprocess
  gnomonWorkspacePreprocess.h
  gnomonWorkspacePythonAlgorithm
  gnomonWorkspacePythonAlgorithm.h
  # gnomonWorkspacePythonSimulator
  # gnomonWorkspacePythonSimulator.h
  gnomonWorkspaceRegistration
  gnomonWorkspaceRegistration.h
  gnomonWorkspaceSimulation
  gnomonWorkspaceSimulation.h
  gnomonWorkspaceSegmentation
  gnomonWorkspaceSegmentation.h
  # gnomonWorkspaceTreeAnalysis
  # gnomonWorkspaceTreeAnalysis.h
  # gnomonWorkspaceTreeConstructor
  # gnomonWorkspaceTreeConstructor.h
  # gnomonWorkspaceTreeFromLString
  # gnomonWorkspaceTreeFromLString.h
  gnomonWorkspaceTemplate_p.h
  gnomonWorkspaceTemplate_p.tpp)

set(${PROJECT_NAME}_SOURCES
  gnomonPythonAlgorithmPluginCode.cpp
  gnomonAlgorithmWorkspace.cpp
  gnomonWorkspaceBinarization.cpp
  gnomonWorkspaceBrowser.cpp
  # gnomonWorkspaceDatabaseBrowser.cpp
  # gnomonWorkspaceCellComplexConstructor.cpp
  # gnomonWorkspaceCellComplexFromCellImage.cpp
  # gnomonWorkspaceCellImageConstructor.cpp
  # gnomonWorkspaceCellImageFilter.cpp
  gnomonWorkspaceCellImageQuantification.cpp
  gnomonWorkspaceCellImageTracking.cpp
  # gnomonWorkspaceImageConstructor.cpp
  gnomonWorkspaceImageMeshing.cpp
  # gnomonWorkspaceFusion.cpp
  # gnomonWorkspaceLStringTranslation.cpp
  gnomonWorkspaceLSystemModel.cpp
  # gnomonWorkspaceLSystemSimulator.cpp
  # gnomonWorkspaceMeshConstructor.cpp
  gnomonWorkspaceMeshFilter.cpp
  gnomonWorkspaceMorphonet.cpp
  # gnomonWorkspacePlantScan3D.cpp
  # gnomonWorkspacePointCloudConstructor.cpp
  gnomonWorkspacePointDetection.cpp
  gnomonWorkspacePointCloudQuantification.cpp
  gnomonWorkspacePreprocess.cpp
  gnomonWorkspacePythonAlgorithm.cpp
  # gnomonWorkspacePythonSimulator.cpp
  gnomonWorkspaceRegistration.cpp
  gnomonWorkspaceSegmentation.cpp
  gnomonWorkspaceSimulation.cpp
  # gnomonWorkspaceTreeAnalysis.cpp
  # gnomonWorkspaceTreeConstructor.cpp
  # gnomonWorkspaceTreeFromLString.cpp)
)
set(${PROJECT_NAME}_SOURCES_QRC gnomonWorkspace.qrc)

## #################################################################
## Build rules
## #################################################################

qt6_add_resources(${PROJECT_NAME}_SOURCES_RCC ${${PROJECT_NAME}_SOURCES_QRC})

add_library(${PROJECT_NAME} SHARED
  ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS}
  ${${PROJECT_NAME}_SOURCES_RCC})

target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Gui)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Qml)

target_link_libraries(${PROJECT_NAME} PRIVATE dtkCore)
target_link_libraries(${PROJECT_NAME} PRIVATE dtkScript)

target_link_libraries(${PROJECT_NAME} PRIVATE libzip::zip)

target_link_libraries(${PROJECT_NAME} PUBLIC gnomonCore)
target_link_libraries(${PROJECT_NAME} PUBLIC gnomonVisualization)
target_link_libraries(${PROJECT_NAME} PUBLIC gnomonPipeline)

target_link_libraries(${PROJECT_NAME} PRIVATE ${VTK_LIBRARIES})

## #############################################################################
##
## #############################################################################

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${layer_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
  $<BUILD_INTERFACE:${layer_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
  $<INSTALL_INTERFACE:include>)

## ###################################################################
## Target properties
## ###################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION   ${gnomon_VERSION}
                                                 SOVERSION ${gnomon_VERSION_MAJOR})

## #################################################################
## Export header file
## #################################################################

generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export")

set(${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS}
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export"
  "${CMAKE_BINARY_DIR}/${PROJECT_NAME}Export.h")

## ###################################################################
## Install rules - files
## ###################################################################

install(FILES ${${PROJECT_NAME}_HEADERS}
  DESTINATION include/${PROJECT_NAME}
  COMPONENT gnomon-workspace)

## ###################################################################
## Install rules - targets
## ###################################################################

install(TARGETS ${PROJECT_NAME} EXPORT ${LAYER_TARGETS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

######################################################################
### CMakeLists.txt ends here
