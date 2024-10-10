# Creating a new Plugin Package

To make sure that your **Python Plugins**[{fas}`book-open;sd-text-primary fa-2xs`](plugin-definition) will be discovered by Gnomon, the best way is to store them into an installable [Python package](https://packaging.python.org/en/latest/tutorials/packaging-projects/) that will declare them as [entry points](https://packaging.python.org/en/latest/specifications/entry-points/). In this page we provide a step-by-step guide to easily create a Python package for your Plugins.

## Package folder architecture

Create a folder named `gnomon-package-pkgname` that should at least contain the following items:
- `setup.py`
- `pyproject.toml`
- `src/gnomon_package_pkgname` :  with subfolders for different plugin categories
    - with an `__init__.py` in each subfolder
- `README.md`

:::{dropdown} Package Architecture
:class: note
:name: plugin-package-architecture

Here is the typical architecture of a plugin package:
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
│   └───gnomon_package_pkgname
│       └───algorithm
│       │   │   __init__.py
│       │   │   algorithm_plugin1.py
│       │   │   algorithm_plugin2.py
│       │   │   ...
│       │
│       └───form
│       │   │   __init__.py
│       │   │   form_plugin1.py
│       │
│       └───io
│       │   │   __init__.py
│       │   │   io_plugin1.py
│       │
│       │   __init__.py
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
::: 

:::{tip}
You can have a look at the existing packages on  https://gitlab.inria.fr/gnomon/gnomon-packages to get inspiration from.
:::

## Python Packaging

### Create a `setup.py` file

You can initialize your `setup.py` you can follow this minimal template:

```python
#!/usr/bin/env python
#-*- coding: utf-8 -*-

from setuptools import setup, find_packages

short_descr = "package description"
readme = open("README.md")

# find packages
pkgs = find_packages('src')

setup_kwds = {
    name: 'gnomon_package_pkgname',
    version: "X.X.X",
    description: short_descr,
    long_description: readme,
    author: "Author",
    author_email: "author@email.com",
    url: '',
    license: 'LGPLv3-or-later',
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

The setup **keywords dictionary** lets you indicate the name of the plugin package, as well as its version, licence, etc. We advise that requirement keywords remain empty and to manage the dependencies of the package through the **conda** environment.

(package-entry-points)=
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

:::{note}
Entry points are part of the arguments of the `setup` function and needs to be passed to it as showed in the `setup.py` template.
:::

In the context of gnomon we will only be advertising modules, the group name is the abstract base class minus the `gnomonAbstract` part and the entry point name should be the same name as the module name and the class name of the plugin.

For example, if we were to register a plugin called `downsampleFilter` which implements the abstract base class `gnomonAbstractImageFilter` and is located in `src/plugin_name/algorithm` we would get:
```python
entry_points = {
    'imageFilter': [
        'downsampleFilter = plugin_name.algorithm.downsampleFilter',
    ]
}
```


### Configure the `pyproject.toml`

Create the `pyproject.toml` with the following content:

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

### Package installation

In a terminal window, navigate to the root directory of your plugin package and then:
- Activate your local environment, for instance 
```shell script
conda activate gnomon
```
- Install the package in your environment by running
```shell script
pip install .
```

You can check that everything is okay by importing your package in your python interpreter:
```bash
  python -c "import gnomon_package_pkgname; print(gnomon_package_pkgname)"
```

## Adding unit tests

Each plugin should be tested by a unit test, to make sure Gnomon is able to intantiate it and that it performs the desired task correctly.
- Add a `test/` folder at the root of the package
- Write one module per plugin, defining a test class inheriting [`unittest.TestCase`](https://docs.python.org/3/library/unittest.html#unittest.TestCase)
- The `test_XXX` methods should check that the plugin runs without errors and generates the expected output
- If necessary, you may add a `resources` folder with (**small !**) sample data to run your tests on

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

## Publishing as a conda package

To share more conveniently your Plugin package, we recommend that you build it into a conda package. You will need to set up two files in order to build a conda package: `build.sh` and `meta.yaml`.

In addition to that, we will also use an `env.yaml` file to define an environment that includes all the dependencies required to use your Plugins. Create a `conda/` folder with the following structure:

```
gnomon-package-pkgname
│   ...
│
└───src
│   │   ...

└───conda
    │   build.sh
    │   env.yaml
    │   meta.yaml
```

### Defining an environment: `env.yaml`

Conda can save the state of an environment in a `.yaml` file and then create an environment from it. This is useful as it helps to have a consistent environment when developing, especially if one wants to build from source.
Setting up all the dependencies becomes as easy as:
```shell script
git clone https://gitlab.com/your-org/gnomon-package-pkgname.git
cd gnomon-package-pkgname
conda env create -f conda/env.yaml
```

You could create the `env.yaml` file by exporting an existing conda environment with `conda env export`, but to get a more parcimonious environment, we recommend to write it manually.

Create
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
  - gnomon>=1.0.1
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
    - gnomon>=1.0.1
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
Here we use the template `{{ environ.get('GIT_DESCRIBE_TAG', 'default') }}` to get the tag from git if you're using git to hold the version.
Otherwise put the version manually.

#### source
We only have one source and it is local. The path should be the relative path to the root of the package where the `setup.py` is located.

#### build
For a pure python package this section should stay the same.

The line `preserve_egg_dir: True` **must** be there.

#### requirements
In this section we define the different requirements needed to build, to link and to run. There are three sections which basically go like this:
- `build`: what packages are needed in order to build the package
- `host`: what packages should be linked in the destination platform
- `run`: what packages are needed at runtime

More information on the definition of `meta.yaml` [here](https://docs.conda.io/projects/conda-build/en/latest/resources/define-metadata.html#).

### Build script: `build.sh`

This one is rather easy. It is simply the bash script that needs to be called in order to build what needs to be packaged, in our case a python package.

The content should therefor simply be:
```bash
#!/bin/bash
pip install .
```

### Building the conda package

Once all the files are ready, you can build the conda package. If you are doing this for the first time, you will need to [install the build utility from conda](https://docs.conda.io/projects/conda-build/en/latest/install-conda-build.html). You will then be able to run the following command, using the channel names required by your package after a `-c`: 

```shell script
conda build . -c conda-forge -c gnomon -c mosaic -c morpheme -c dtk-forge6
```

Such a package can then be [uploaded on your Anaconda channel using the recommended procedure](https://docs.anaconda.com/anacondaorg/user-guide/packages/conda-packages/#uploading-conda-packages).
