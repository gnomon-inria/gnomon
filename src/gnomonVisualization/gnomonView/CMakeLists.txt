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
    # gnomonViewManager
    # gnomonViewManager.h
    gnomonMplView
    gnomonMplView.h
    gnomonQmlView
    gnomonQmlView.h
)

ADD_GNOMON_SOURCES(
    gnomonAbstractView.cpp
    # gnomonFormAdapterMenu.cpp
    gnomonVtkView.cpp
    gnomonVtkViewList.cpp
    gnomonVtkViewPool.cpp
    # gnomonViewManager.cpp
    gnomonMplView.cpp
    gnomonQmlView.cpp
)

######################################################################
### CMakeLists.txt ends here
