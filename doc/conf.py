# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Gnomon'
copyright = '2024, Gnomon Development Team'
author = 'Gnomon Development Team'
release = '1.0.1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'myst_parser',
    # "sphinx_external_toc",
    "sphinx_design",
]

templates_path = ['_templates']
exclude_patterns = []

source_suffix = '.md'

# external_toc_path = "_toc.yml"
# external_toc_exclude_missing = False

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'pydata_sphinx_theme'
html_static_path = ['_static']
html_logo = '_static/gnomon_logo.png'
html_favicon = '_static/gnomon_logo.png'

html_theme_options = {
    "logo": {
        "text": "Gnomon"
    },
    "icon_links": [
        {
            "name": "GitLab",
            "url": "https://gitlab.inria.com/mosaic/gnomon",
            "icon": "fab fa-gitlab",
            "type": "fontawesome",
        },
    ],
}

# -- MyST options ------------------------------------------------------------
myst_enable_extensions = ["colon_fence", "attrs_inline"]
