# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
sys.path.insert(0, os.path.abspath('../build/python'))


# -- Project information -----------------------------------------------------

project = 'gnomon'
copyright = '2022, Inria'
author = 'Mosaic project-team and AMDT team'

# The short X.Y version
version = '0.71'
# The full version, including alpha/beta/rc tags
release = '0.71.0'


# -- General configuration ---------------------------------------------------

# The suffix of source filenames.
source_suffix = '.md'


# The master toctree document.
master_doc = 'index'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = []
extensions += ['myst_parser']
extensions += ['sphinx.ext.autosectionlabel']
extensions += ['sphinx_design']  # Enable panels, cards & tabs usage
extensions += ['sphinx.ext.graphviz']
extensions += ['sphinx.ext.autodoc']
extensions += ['sphinx.ext.napoleon']

myst_heading_anchors = 2
myst_enable_extensions = ["colon_fence", "amsmath", "dollarmath"]

# Prefix document path to section labels, to use:
# `path/to/file:heading` instead of just `heading`
autosectionlabel_prefix_document = True

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'friendly'

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# Napoleon options
napoleon_attr_annotations = False
autodoc_typehints = "both"
autodoc_default_options = {
    'member-order': 'bysource',
    'special-members': '__init__',
}
autodoc_mock_imports = ["gnomon.core", "gnomon.visualization", "gnomon.pipeline"]

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'press'

if html_theme == 'press':
    html_sidebars = {'**': ['util/searchbox.html', 'globaltoc.html']}
    globaltoc_maxdepth = 2
    globaltoc_includehidden = False
    globaltoc_collapse = True
    html_logo = '_static/gnomon_logo.png'
    html_favicon = '_static/gnomon_icon.ico'
    html_css_files = ['css/press_custom.css']
    html_show_sphinx = True
    html_show_copyright = False
    html_theme_options = {
        "external_links": [
            ("Source Code", "https://gitlab.inria.fr/gnomon/gnomon"),
            ("Anaconda.org", "https://anaconda.org/gnomon/gnomon")
        ]
    }

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

