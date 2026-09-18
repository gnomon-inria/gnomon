# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Gnomon'
copyright = '2024, Gnomon Development Team'
author = 'Gnomon Development Team'
release = '1.0.2'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'myst_parser',
    # "sphinx_external_toc",
    "sphinx_design",
    "sphinx_tippy",
    "sphinx.ext.autodoc",
    "sphinx.ext.napoleon",
]
autodoc2_packages = [
    "../build/python/gnomon/utils/gnomonPlugin.py",
    "../build/python/gnomon/utils/decorators",
    "../build/python/gnomon/utils/gnomonCommandLineTools.py",
    "../build/python/gnomon/utils/gnomonTemplateUtils.py",
    "../build/python/gnomon/utils/package_utils.py",
    "../build/python/gnomon/utils/pipelines.py",
]

templates_path = ['_templates']
exclude_patterns = []

source_suffix = '.md'

# Napoleon options
napoleon_attr_annotations = False
autodoc_typehints = "both"
autodoc_class_signature = "separated"
add_module_names = False
autodoc_default_options = {
    "member-order": "bysource",
    "special-members": "__init__",

}
#autodoc_mock_imports = ["gnomon.core", "gnomon.visualization", "gnomon.pipeline"]

# external_toc_path = "_toc.yml"
# external_toc_exclude_missing = False

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'pydata_sphinx_theme'
html_static_path = ['_static']
html_css_files = ['css/theme_custom.css', 'css/tippy_custom.css', 'css/gallery_custom.css', 'css/admonition_custom.css']
html_logo = '_static/gnomon_logo.png'
html_favicon = '_static/gnomon_logo.png'

html_theme_options = {
    "logo": {
        "text": "Gnomon"
    },
    "icon_links": [
        {
            "name": "GitLab",
            "url": "https://gitlab.inria.fr/gnomon/gnomon",
            "icon": "fab fa-gitlab",
            "type": "fontawesome",
        },
    ],
    "navbar_align": "content",
    "announcement": "This new version of the documentation is currently under construction, but you can still access <a href='https://gnomon.gitlabpages.inria.fr/gnomon/legacy/'>the previous version!</a>",
    "footer_start": ["copyright"],
    "footer_end": ["sphinx-version", "theme-version"]
}

# -- MyST options ------------------------------------------------------------
myst_enable_extensions = [
    "colon_fence",
    "dollarmath",
    "amsmath",
    "deflist",
    "attrs_inline",
    "attrs_block"
]
