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


## #################################################################
## Sources
## #################################################################


ADD_GNOMON_SUBDIRECTORY_HEADERS(
    gnomonLStringAdapterCommand
    gnomonLStringAdapterCommand.h
    gnomonLStringReaderCommand
    gnomonLStringReaderCommand.h
    gnomonLStringTranslationCommand
    gnomonLStringTranslationCommand.h
    gnomonLStringWriterCommand
    gnomonLStringWriterCommand.h)


ADD_GNOMON_SUBDIRECTORY_SOURCES(
  gnomonLStringAdapterCommand.cpp
  gnomonLStringReaderCommand.cpp
  gnomonLStringTranslationCommand.cpp
  gnomonLStringWriterCommand.cpp)


######################################################################
### CMakeLists.txt ends here
