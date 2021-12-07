# How to make a new plugin package

## Package folder architecture

**Minimal files for packaging are**
- `setup.py`
- `src/plugin_name` :  with a subfolder for algorithm, form, IO, ...
- `__init__.py` in each subfolder

```
gnomon-package-pkgname
│   README.md
│   setup.py
│   LICENSE   
│
└───src
    └───plugin_name
        └───algorithm
        │   │   __init__.py
        │   │   algorithm_plugin1.py
        │   │   algorithm_plugin2.py
        │   │   ...
        │
        └───form
        │   │   __init__.py
        │   │   form_plugin1.py
        │
        └───io
        │   │   __init__.py
        │   │   io_plugin1.py
        │
        │   __init__.py
```

**Go to package directory** on  https://gitlab.inria.fr/gnomon/gnomon-packages and choose one package to get inspiration from.

### setup.py

To write the `setup.py` you can follow this minimal template:

```python 
#!/usr/bin/env python
#-*- coding: utf-8 -*-

from setuptools import setup, find_packages

from gnomon_utils.gnomonPlugin import gnomon_declare_plugins

short_descr = "package description"
readme = open("README.md")

# find packages
pkgs = find_packages('src')

setup_kwds = {
    name: 'plugin_name',
    version: "X.X.X",
    description: short_descr,
    long_description: readme,  
    author: "Author",
    author_email: "author@email.com",
    url: '',
    license: 'LGPL',
    zip_safe: False,

    packages: pkgs,

    package_dir: {'': 'src'},
    setup_requires: [],
    install_requires: [],
    tests_require: [],
    entry_points: gnomon_declare_plugins('src/'),
    keywords: '',

    test_suite: 'nose.collector'
}

setup(**setup_kwds)
```

This python file includes the following functions from [`setuptools`](https://setuptools.pypa.io/en/latest/setuptools.html)
- `setup` : create the Python Egg for the package.
- `find_packages` : find modules from source directory, given as arg.

**Create a setup keywords dictionary**: in this dictionary you precise plugin's name, version, licence. The requirement keywords are all left empty as we consider that dependencies are managed through the conda environment.

**Entry points**: a specific Gnomon function `gnomon_declare_plugins` allows to parse the source modules and automatically declare [entry-points](https://packaging.python.org/en/latest/specifications/entry-points/) for the plugin classes.

## Package installation
**At the root of the package** 
- activate your local environment e.g.: `conda activate gnomon-x`
- run `python setup.py develop`

You can check that everything is okay by importing your package in your python interpreter:
`python -c "import plugin_name; print(plugin_name)"`

## Publishing as a conda package



