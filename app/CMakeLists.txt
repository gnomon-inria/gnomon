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

add_subdirectory(gnomonGrowthSimulator)

if(OMERO_ICE_FOUND AND OMERO_CLIENT_FOUND)
  add_subdirectory(gnomonOmeroClient)
endif(OMERO_ICE_FOUND AND OMERO_CLIENT_FOUND)

######################################################################
### CMakeLists.txt ends here
