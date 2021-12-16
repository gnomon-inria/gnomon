## #################################################################
## Sources
## #################################################################


ADD_GNOMON_SUBDIRECTORY_HEADERS(
    gnomonBinaryImageFromImageCommand
    gnomonBinaryImageFromImageCommand.h
    gnomonBinaryImageReaderCommand
    gnomonBinaryImageReaderCommand.h
    gnomonBinaryImageWriterCommand
    gnomonBinaryImageWriterCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
    gnomonBinaryImageFromImageCommand.cpp
    gnomonBinaryImageReaderCommand.cpp
    gnomonBinaryImageWriterCommand.cpp)


######################################################################
### CMakeLists.txt ends here
