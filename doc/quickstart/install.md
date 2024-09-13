# Installation

## [Mac and Linux] Installing Gnomon using conda

:::{warning}
[Mac] Gnomon requires Mac OS 11 (Big Sur) or later
:::

### Prerequisite: Install Conda
- Make sure you have `conda` installed on your system. To check, simply open a new terminal window and type:
```shell script
conda
```

- If `conda` is installed, you should get a long return value describing how to use the command, otherwise you will get a `command not found` message.

- If `conda` is not installed, we recommend that you [install Miniconda](https://docs.conda.io/en/latest/miniconda.html) by picking the *latest* installer suitable for your system.

:::{admonition} About Conda and Miniconda
Conda is an open source package management system and environment management system that runs on Windows, macOS and Linux. Conda quickly installs, runs and updates packages and their dependencies. Conda easily creates, saves, loads and switches between environments on your local computer. Find out more on [the official documentation](https://docs.conda.io/en/latest/)

[Miniconda](https://docs.conda.io/en/latest/miniconda.html) is a free minimal installer for conda. It is a small, bootstrap version of Anaconda that includes only `conda`, Python, the packages they depend on, and a small number of other useful packages, including pip, zlib and a few others.
:::

### Install Gnomon

:::{warning}
Installing `gnomon` requires around *4 GB* of free disk space, environment included.
:::

#### **Step 1:** (Optional) Install Mamba in your `(base)` environment
```shell script
conda install -n base -c conda-forge mamba
```

#### **Step 2:**  Create a conda environment with the right python version. Then activate this environment
```shell script
conda create -n gnomon python=3.9
conda activate gnomon
```

:::{note}
For Mac M1, you need to tell conda to use x86 architecture like this:
```shell script
CONDA_SUBDIR=osx-64 conda create -n gnomon python=3.9
conda activate gnomon
conda config --env --set subdir osx-64
```
:::

:::{dropdown} Existing environment
In case you already have a conda environment named `(gnomon)`, you can specify another name after the `-n` option:
```shell script
conda create -n gnomon-stable python=3.9
```
:::

#### **Step 3:** Install **gnomon** and its **dependencies**
```shell script
mamba install -c dtk-forge6 -c gnomon -c mosaic -c morpheme -c conda-forge gnomon
```

:::{note}
At this stage the `gnomon` application is installed but "empty" since no plugins are installed by default. To actually make it usable, you will have to install [**plugin packages**](install_packages)
:::

#### **Step 4:** Congrats, you can now launch the application
```shell script
gnomon
```

### Updating Gnomon

#### **Step 1:** Activate your `(gnomon)` environment
```shell script
conda activate gnomon
```

#### **Step 2:**  Update **gnomon** and its **dependencies**
```shell script
gnomon-utils package update
```

:::{dropdown} Alternative command
```shell script
mamba update -c dtk-forge6 -c gnomon -c mosaic -c morpheme -c conda-forge gnomon
```
:::

## [Windows] Install Gnomon via WSL

:::{warning}
[Windows] Some of the dependencies required by Gnomon are not packaged for Windows, but it is possible to use Linux packages via the Windows Subsystem for Linux (WSL).
:::

[Installation guidelines for Windows](installation_windows)

:::{toctree}
:maxdepth: 1
:hidden:
installation_windows
:::