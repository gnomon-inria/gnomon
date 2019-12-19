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

find_package(Sphinx QUIET)
find_package(Doxygen QUIET)


if (Sphinx_FOUND AND Doxygen_FOUND)

if(NOT DEFINED SPHINX_THEME)
  set(SPHINX_THEME "sphinx_rtd_theme")
endif()

if(NOT DEFINED SPHINX_THEME_DIR)
  set(SPHINX_THEME_DIR)
endif()

set(PROJECT_SHORT_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}")
set(PROJECT_VERSION ${${PROJECT_NAME}_VERSION})

# configured documentation tools and intermediate build results
set(BINARY_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")

# Sphinx cache with pickled ReST documents
set(SPHINX_CACHE_DIR "${CMAKE_CURRENT_BINARY_DIR}/_doctrees")

# HTML output directory
set(SPHINX_HTML_DIR "${CMAKE_CURRENT_BINARY_DIR}/html")

configure_file(conf.py ${BINARY_BUILD_DIR}/conf.py IMMEDIATE @ONLY)

configure_file(Doxyfile ${BINARY_BUILD_DIR}/Doxyfile IMMEDIATE @ONLY)

add_custom_target(dox
  WORKING_DIRECTORY ${BINARY_BUILD_DIR}
  COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
  COMMENT "Building Doxygen doc")

add_custom_target(doc
  WORKING_DIRECTORY ${BINARY_BUILD_DIR}
  COMMAND ${SPHINX_EXECUTABLE} -q -b html -c "${BINARY_BUILD_DIR}" -d "${SPHINX_CACHE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" "${SPHINX_HTML_DIR}"
  COMMENT "Building HTML documentation with Sphinx")

add_dependencies(doc dox)

endif(Sphinx_FOUND AND Doxygen_FOUND)

######################################################################
### CMakeLists.txt ends here
