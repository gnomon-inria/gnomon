## #################################################################
## Sources
## #################################################################


ADD_GNOMON_SUBDIRECTORY_HEADERS(
    gnomonImageConstructorCommand
    gnomonImageConstructorCommand.h
    gnomonImageFilterCommand
    gnomonImageFilterCommand.h
    gnomonImageFusionCommand
    gnomonImageFusionCommand.h
    gnomonImageReaderCommand
    gnomonImageReaderCommand.h
    gnomonImageRegistrationCommand
    gnomonImageRegistrationCommand.h
    gnomonImageWriterCommand
    gnomonImageWriterCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonImageConstructorCommand.cpp
    gnomonImageFilterCommand.cpp
    gnomonImageFusionCommand.cpp
    gnomonImageReaderCommand.cpp
    gnomonImageRegistrationCommand.cpp
    gnomonImageWriterCommand.cpp)


######################################################################
### CMakeLists.txt ends here
