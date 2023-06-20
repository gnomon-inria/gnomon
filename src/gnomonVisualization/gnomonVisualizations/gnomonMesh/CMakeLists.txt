ADD_GNOMON_SUBDIRECTORY_HEADERS(
  gnomonAbstractMeshVtkVisualization
  gnomonAbstractMeshVtkVisualization.h
  gnomonMeshVtkVisualization
  gnomonMeshVtkVisualization.h
  gnomonVtkDecorator.h
  gnomonVtkDecoratorIsoContours.h
  gnomonVtkDecoratorSurfaceColor.h
  #gnomonVtkDecoratorVectorGlyphs.h
  )


ADD_GNOMON_SUBDIRECTORY_SOURCES(
  gnomonAbstractMeshVtkVisualization.cpp
  gnomonMeshVtkVisualization.cpp
  gnomonVtkDecorator.cpp
  gnomonVtkDecoratorIsoContours.cpp
  gnomonVtkDecoratorSurfaceColor.cpp
  #gnomonVtkDecoratorVectorGlyphs.cpp
 )
