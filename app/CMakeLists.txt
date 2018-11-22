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

add_subdirectory(gnomon)
if(GNOMON_BUILD_COMPOSER)
add_subdirectory(gnomonGrowthSimulator)
endif(GNOMON_BUILD_COMPOSER)

if(OMERO_ICE_FOUND AND OMERO_CLIENT_FOUND)
  add_subdirectory(gnomonOmeroClient)
endif(OMERO_ICE_FOUND AND OMERO_CLIENT_FOUND)

######################################################################
### CMakeLists.txt ends here
