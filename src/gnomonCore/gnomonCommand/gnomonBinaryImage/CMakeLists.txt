## #################################################################
## Sources
## #################################################################


ADD_GNOMON_SUBDIRECTORY_HEADERS(
    gnomonBinaryImageReaderCommand
    gnomonBinaryImageReaderCommand.h
    gnomonBinaryImageWriterCommand
    gnomonBinaryImageWriterCommand.h
    gnomonBinaryImageFromImageCommand
    gnomonBinaryImageFromImageCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonBinaryImageReaderCommand.cpp
    gnomonBinaryImageWriterCommand.cpp
    gnomonBinaryImageFromImageCommand.cpp)


######################################################################
### CMakeLists.txt ends here
