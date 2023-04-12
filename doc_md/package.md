# How to make a new plugin package

## Package folder architecture

**Minimal files for packaging are**
- `setup.py`
- `pyproject.toml`
- `src/plugin_name` :  with a subfolder for algorithm, form, IO, ...
- `__init__.py` in each subfolder

```
gnomon-package-pkgname
│   README.md
│   setup.py
│   pyproject.toml
│   LICENSE   
│
└───conda
│   │   build.sh
│   │   env.yaml
│   │   meta.yaml
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

### setup.py & pyproject.toml: python packaging

#### `setup.py`
To write the `setup.py` you can follow this minimal template:

```python 
#!/usr/bin/env python
#-*- coding: utf-8 -*-

from setuptools import setup, find_packages

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
    package_data: {
        "": [
            "*.png",
            "*/*.png",
            "*/*/*.png",
            "*.json",
            "*/*.json",
            "*/*/*.json"
        ]
    },

    entry_points: {
        'data_plugin_group': [
            'form_plugin1 = plugin_name.form.form_plugin1',
        ],
        'io_plugin_group': [
            'io_plugin1 = plugin_name.io.io_plugin1',
        ],
        'algo_plugin_group': [
            'algorithm_plugin1 = plugin_name.algorithm.algorithm_plugin1',
            'algorithm_plugin2 = plugin_name.algorithm.algorithm_plugin2',
        ],
    ]
    setup_requires: [],
    install_requires: [],
    tests_require: [],
    keywords: '',

    test_suite: 'nose.collector'
}

setup(**setup_kwds)
```

This python file includes the following functions from [`setuptools`](https://setuptools.pypa.io/en/latest/setuptools.html)
- `setup` : create the Python Egg for the package.
- `find_packages` : find modules from source directory, given as arg.

**Create a setup keywords dictionary**: in this dictionary you precise plugin's name, version, licence. The requirement keywords are all left empty as we consider that dependencies are managed through the conda environment.

(entry_points)=
### Advertising your plugins in the global namespace: entry points

Before your plugins can be used by gnomon they need to be advertised so gnomon can dynamically discover them.
To achieve that we use a feature called [entry points](https://setuptools.pypa.io/en/latest/userguide/entry_point.html)
With entry points one can register a console command (console-script) which would run a specific function in the library or 
advertise modules or objects. What interest us is the second use case.

The format for declaring entry points is as follows:

```python
entry_points = {
    'group_name': [
        # advertises the module
        'entry_point_name = import.path.to.resource',
        # advertises a class from the module
        'entry_point_name = import.path.to.resource:class',
    ]
}
```

**Note:** entry points are part of the arguments of the `setup` function and needs to be passed to it as showed
in the `setup.py` template.

In the context of gnomon we will only be advertising modules, the group name is the abstract base class
minus the `gnomonAbstract` part and the entry point name should be the same name as the module name and the class name of the plugin.

For example, if we were to register a plugin called `downsampleFilter` which implements the abstract base class
`gnomonAbstractImageFilter` and is located in `src/plugin_name/algorithm` we would get: 
```python
entry_points = {
    'imageFilter': [
        'downsampleFilter = plugin_name.algorithm.downsampleFilter',
    ]
}
```

There are two special entry points' group: `console_scripts` and `gui_scripts` which we **won't** be using here.


#### `pyproject.toml`

```toml
[build-system]
requires = ["setuptools"]
build-backend = "setuptools.build_meta"

[tool.coverage.run]
source = ["src"]
omit = ["*__init__.py", "test/*", "setup.py"]

[tool.coverage.report]
omit = ["*__init__.py", "test/*", "setup.py"]
```

## Package installation
**At the root of the package** 
- activate your local environment e.g.: `conda activate gnomon-x`
- run `pip install .`

You can check that everything is okay by importing your package in your python interpreter:
```bash
  python -c "import plugin_name; print(plugin_name)"
```

## Adding unit tests

**Each plugin should be tested**
- add a `test/` folder at the root of the package
- write one module per plugin, defining a test class inheriting [`unittest.TestCase`](https://docs.python.org/3/library/unittest.html#unittest.TestCase)
- the `test_XXX` methods should check that the plugin runs without errors and generates the expected output
- if necessary, you may add a `resources` folder with (**small !**) sample data to run your tests on

```
gnomon-package-pkgname
│   ...   
│
└───src   
│   │   ...
│
└───test  
    └───resources
    │   │   ...
    │
    │   test_algorithm_plugin1.py
    │   test_algorithm_plugin2.py
    │   test_form_plugin1.py
    │   test_io_plugin1.py
```

## Building and Publishing a conda package

We need to set up 2 files in order to build a package: `build.sh` and `meta.yaml`.
In addition to that we will also use an `env.yaml` file to define our working environment.

### Defining an environment: `env.yaml`

Conda can save the state of an environment in a `.yaml` file and then create an environment from it.
This is useful as it helps to have a consistent environment when developing, especially if one wants to build from source.
Getting the environment setup becomes as easy as:
```bash
git clone https://gitlab.com/your-org/gnomon-package-pkgname.git
cd gnomon-package-pkgname
conda env create -f conda/env.yaml
```

There are two ways to make an `env.yaml` file: 
1. by exporting an existing env with `conda env export`
2. by hand

In this section we will look at the second method.

An `env.yaml` file looks as follows:
```yaml
name: gnomon-package-pkgname
channels:
  - gnomon
  - mosaic
  - morpheme
  - dtk-forge6
  - conda-forge
dependencies:
  - python=3.9
  - libgnomon>=0.80.0
  - gnomon_package_data
  - ipython
  - nose2
  - coverage
  - importlib_metadata
  - numpy
  - pip:
      - pandas
```

There are three parts in it:
1. `name` which will be the default name of the package (a new name can be used at install time with `conda env create -f conda/env.yaml -n new_name`).
2. `channels` which are the channels from which to pull the dependencies.
3. `dependencies` which lists the dependencies that needs to be installed. Dependencies can also be pulled from PyPI by specifying them in the `- pip:` part.

### Metadata file (recipe): `meta.yaml`

This is where the package metadata is defined as well as data regarding how the package should be built.
Here is an example from the package `gnomon_package_tissueimage`:

```yaml
package:
  name: gnomon_package_tissueimage
  version: {{ environ.get('GIT_DESCRIBE_TAG', 'default') }}

source:
  path: ../

build:
  number: {{ GIT_DESCRIBE_NUMBER }}
  preserve_egg_dir: True

requirements:
  build:
    - pip
    - python=3.9
  host:
    - python=3.9
  run:
    - python=3.9
    - libgnomon>=0.80.0
    - gnomon_package_data
    - ipython
    - nose2
    - coverage
    - matplotlib-base
    - numpy
    - pandas
    - scikit-learn
    - timagetk=3
    - ctrl
    - cellcomplex
    - importlib_metadata

about:
  home: https://gnomon.gitlabpages.inria.fr/gnomon/
  license: LGPL-3.0
  license_file: LICENSE
  summary: Gnomon python plugins to represent and manipulate 3D images
```
As you can see `jinja2` templating can be used here.

#### package
In this section the package name and version are defined. 
Here we use the template `{{ environ.get('GIT_DESCRIBE_TAG', 'default') }}`
to get the tag from git if you're using git to hold the version.
Otherwise put the version manually.

#### source
We only have one source and it is local. The path should be the relative 
path to the root of the package where the `setup.py` is located.

#### build
For a pure python package this section should stay the same.

The line `preserve_egg_dir: True` **must** be there.

#### requirements
In this section we define the different requirements needed to build, 
to link and to run. There are three sections which basically go like this:
- `build`: what packages are needed in order to build the package
- `host`: what packages should be linked in the destination platform
- `run`: what packages are needed at runtime

More information on the definition of `meta.yaml` [here](https://docs.conda.io/projects/conda-build/en/latest/resources/define-metadata.html#).

### Build script: `build.sh`

This one is rather easy. It is simply the bash script that needs to be called in order 
to build what needs to be packaged, in our case a python package.

The content should therefor simply be:
```bash
#!/bin/bash
pip install .
```

### Building

```bash
conda build . -c conda-forge -c gnomon -c mosaic -c morpheme -c dtk-forge6
```
