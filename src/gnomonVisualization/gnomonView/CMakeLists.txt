ADD_GNOMON_HEADERS(
    gnomonAbstractView
    gnomonAbstractView.h
    gnomonAbstractView_p.h
    # gnomonFormAdapterMenu
    # gnomonFormAdapterMenu.h
    gnomonVtkView
    gnomonVtkView.h
    gnomonVtkViewList
    gnomonVtkViewList.h
    gnomonVtkViewPool
    gnomonVtkViewPool.h
    gnomonMplView
    gnomonMplView.h
    gnomonQmlView
    gnomonQmlView.h
    gnomonNurbsView
    gnomonNurbsView.h
)

ADD_GNOMON_SOURCES(
    gnomonAbstractView.cpp
    # gnomonFormAdapterMenu.cpp
    gnomonVtkView.cpp
    gnomonVtkViewList.cpp
    gnomonVtkViewPool.cpp
    gnomonMplView.cpp
    gnomonQmlView.cpp
    gnomonNurbsView.cpp
)

######################################################################
### CMakeLists.txt ends here
