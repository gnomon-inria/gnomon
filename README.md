# Installation

For sake of clarity, let's consider that all the programs are installed under `$HOME/Development` folder.

## Pre-requisites stuff

### Install conda

``` shell
cd ~/Downloads
brew install wget
wget https://repo.continuum.io/miniconda/Miniconda2-latest-MacOSX-x86_64.sh
chmod u+x Miniconda2-latest-MacOSX-x86_64.sh
./Miniconda2-latest-MacOSX-x86_64.sh
# install in $HOME/.conda
```

Then, make sure to have conda in your PATH environment variable, before any folder containing `qmake`.

### Using Conda environment:

First clone gnomon-recipes:

``` shell
cd $HOME/Development
git clone git@gitlab.inria.fr:gnomon/gnomon-recipes.git
cd gnomon-recipes/environment
```

To create the conda environment using the previous recipe saved under `gnomon.yml`:
```shell
conda env create -f gnomon-1.x.x.yml
```
To activate the conda environment:
``` shell
source activate gnomon-1.x.x
```

Inside this environment install dtk, its applicative layers, gnomon and its plugins by cloning the source code.

Pre-requisites might be installed outside the environment.

### OpenGL Stuff (at least for ubuntu)

``` shell
sudo apt install freeglut3-dev
sudo apt install mesa-common-dev
```

### CMake

``` shell
apt install cmake cmake-curses-gui # ubuntu
dnf install cmake # fedora
brew install cmake # mac
```

#### Qt objects within Python environnement using SWIG.

Install swig development packages with RPM. On mac, use `brew install swig`

### VTK8 installation

Starts by downloading the sources for VTK8 (here 8.0.1):
``` shell
cd $HOME/Development
wget http://www.vtk.org/files/release/8.0/VTK-8.0.1.tar.gz
```

Unzip and create the `build` directory:
``` shell
tar -zxvf VTK-8.0.1.tar.gz
cd VTK-8.0.1/
mkdir build
cd build
```

Compile after defining `cmake` options using arguments parsing (`cmake` options can also be defined using `ccmake ..` to access CMake curse interface):
``` shell
source activate gnomon-1.x.x
cmake .. -DVTK_Group_Qt=ON -DVTK_QT_VERSION=5 -DVTK_RENDERING_BACKEND=OpenGL2 -DModule_vtkGUISupportQtOpenGL=ON
make -j4
```

## OpenAlea legacy [REQUIRED]
Before installing the following sources, do not forget to activate the conda environment:
```shell
source activate gnomon-1.x.x
```

### OpenAlea CellComplex

``` shell
cd $HOME/Development
# Clone the sources:
git clone https://github.com/gcerutti/cellcomplex.git openalea-cellcomplex
# Checkout the proper branch:
cd openalea-cellcomplex
git branch feature/standalone origin/feature/standalone
git checkout feature/standalone
git pull origin feature/standalone
# Install in develop mode:
python setup.py develop
```

### OpenAlea DracoStem

``` shell
cd $HOME/Development
# Clone the sources:
git clone https://github.com/gcerutti/draco_stem.git openalea-draco-stem
# Checkout the proper branch:
cd openalea-draco-stem
git branch feature/standalone origin/feature/standalone
git checkout feature/standalone
git pull origin feature/standalone
# Install in develop mode:
python setup.py develop
```

### OpenAlea TimageTK
You will need `scons` to compile parts of the sources.
On MacOS: `brew install scons`.
On Ubuntu: `sudo apt install scons`.

Create a `.profile` file in your home folder (why?):
`touch $HOME/.profile`.

``` shell
cd $HOME/Development
# Clone the sources:
git clone git@gitlab.inria.fr:mosaic/timagetk.git openalea-timagetk
# Checkout the proper branch:
cd openalea-timagetk
git pull origin master
# Install in develop mode:
python setup.py develop
```

### OpenAlea TissueAnalysis

``` shell
cd $HOME/Development
# Clone the sources:
git clone git@gitlab.inria.fr:mosaic/tissue_analysis.git openalea-tissue-analysis
# Checkout the proper branch:
cd openalea-tissue-analysis
git branch feature/standalone origin/feature/standalone
git checkout feature/standalone
git pull origin feature/standalone
# Install in develop mode:
python setup.py develop
```

## Get dtk and its applicative layers

### Compilation options:

#### Python wrapping:

To enable python wrapping use compilation flag: ` -DDTK_WRAPPING_PYTHON=ON`

To enable SWIG wrapping use compilation flag: ` -DDTK_PYTHON_WRAPPER=SWIG`

To enable SIP wrapping use compilation flag: ` -DDTK_PYTHON_WRAPPER=SIP`

To enable both SIP and SWIG wrapping use compilation flag: ` -DDTK_PYTHON_WRAPPER=SWIG_AND_SIP`

### dtk

``` shell
source activate gnomon-1.x.x
cd $HOME/Development
git clone git@gitlab.inria.fr:dtk/dtk.git
cd dtk
git checkout develop
mkdir build
cd build
cmake .. -DDTK_WRAPPING_PYTHON=ON -DDTK_BUILD_COMPOSER=ON -DDTK_BUILD_DISTRIBUTED=ON -DDTK_BUILD_SCRIPT=ON -DDTK_BUILD_WIDGETS=ON -DDTK_BUILD_WRAPPERS=ON -DDTK_BUILD_SUPPORT_COMPOSER=OFF -DDTK_PYTHON_WRAPPER=SWIG_AND_SIP -DDTK_BUILD_SUPPORT_CONTAINER=OFF -DDTK_BUILD_SUPPORT_CORE=OFF -DDTK_BUILD_SUPPORT_GUI=OFF -DDTK_BUILD_SUPPORT_MATH=OFF
make -j4
```

### dtk-imaging

``` shell
source activate gnomon-1.x.x
cd $HOME/Development
git clone git@gitlab.inria.fr:dtk/dtk-imaging.git
cd dtk-imaging
git checkout develop
mkdir build
cd build
cmake .. -Ddtk_DIR=$HOME/Development/dtk/build
make -j4
```

### dtk-plugins-imaging

``` shell
source activate gnomon-1.x.x
cd $HOME/Development
git clone git@gitlab.inria.fr:dtk/dtk-plugins-imaging.git
cd dtk-plugins-imaging
git checkout develop
mkdir build
cd build
cmake .. -Ddtk_DIR=$HOME/Development/dtk/build -DdtkImaging_DIR=$HOME/Development/dtk-imaging/build -DVTK_DIR=$HOME/Development/VTK-8.0.1/build
make -j4
```

## Get gnomon & gnomon-plugins

### gnomon

To select light or dark theme for gnomon set the compilation flag ` -DGNOME_STYLE="ONELIGHT"|"ONEDARK"`.

``` shell
source activate gnomon-1.x.x
cd $HOME/Development
git clone git@gitlab.inria.fr:gnomon/gnomon.git
cd gnomon
git checkout develop
mkdir build
cd build
cmake .. -Ddtk_DIR=$HOME/Development/dtk/build -DdtkImaging_DIR=$HOME/Development/dtk-imaging/build -DVTK_DIR=$HOME/Development/VTK-8.0.1/build -DGNOMON_STYLE:STRING=ONEDARK
make -j4
```

### gnomon-plugins

``` shell
source activate gnomon-1.x.x
cd $HOME/Development
git clone git@gitlab.inria.fr:gnomon/gnomon-plugins.git
cd gnomon-plugins
git checkout develop
mkdir build
cd build
cmake .. -Ddtk_DIR=$HOME/Development/dtk/build -Dgnomon_DIR=$HOME/Development/gnomon/build -DVTK_DIR=$HOME/Development/VTK-8.0.1/build
make -j4
```

### gnomon-scripts

``` shell
source activate gnomon-1.x.x
cd $HOME/Development
git clone git@gitlab.inria.fr:gnomon/gnomon-scripts.git
```

### gnomon-compositions

``` shell
source activate gnomon-1.x.x
cd $HOME/Development
git clone git@gitlab.inria.fr:gnomon/gnomon-compositions.git
```

### gnomon-data

``` shell
source activate gnomon-1.x.x
cd $HOME/Development
git clone git@gitlab.inria.fr:gnomon/gnomon-data.git
```

## Configuration

In the `$HOME/.config` folder, one has to create a folder `inria`, then one has to set several files:

``` shell
touch dtk-composer.ini
touch dtk-imaging.ini
touch gnomon-core.ini
```

### Composer config

Edit `dtk-composer.ini` file and add the following lines:

``` shell
[extension]
plugins=$HOME/Development/dtk-imaging/build/plugins:$HOME/Development/dtk-discrete-geometry/build/plugins:$HOME/Development/gnomon/build/plugins
```

This will enable the composer embedded into gnomon application to find node extensions provided by gnomon, dtk-imaging and dtk-discrete-geometry.

### dtk-imaging config

Edit `dtk-imaging.ini` file and add the following lines:

``` shell
[imaging]
plugins=$HOME/Development/dtk-plugins-imaging/build/lib
```

This will tell to dtk-imaging layer where are the plugins that implements its abstractions.

### gnomon config

Edit `gnomon-core.ini` file and add the following lines:

``` shell
[gnomon-core]
plugins=$HOME/Development/gnomon-plugins/build/lib
```

**Optional: Enable jupyter console by default in Gnomon** (instead of python)

You need to add some Configuration lines to `dtk-script.ini`.

If using an install from sources:
``` shell
[modules]
path=$HOME/Development/dtk/build/modules:$HOME/Development/dtk/build/lib:$HOME/Development/dtk-imaging/build/modules:$HOME/Development/dtk-plugins-imaging/build/modules:$HOME/Development/dtk-plugins-imaging/src/Python:$HOME/Development/gnomon/build/modules:$HOME/Development/gnomon/build-conda/lib:$HOME/Development/gnomon-plugins/src/Python

[init]
script=$HOME/Development/gnomon-scripts/create_jupyter_console.py
```

If using an install from Conda:
``` shell
[modules]
path=$HOME/Development/dtk/build-conda/modules:$HOME/Development/dtk/build/lib:$HOME/Development/dtk-imaging/build/modules:$HOME/Development/dtk-plugins-imaging/build/modules:$HOME/Development/dtk-plugins-imaging/src/Python:$HOME/Development/gnomon/build/modules:$HOME/Development/gnomon/build-conda/lib:$HOME/Development/gnomon-plugins/src/Python

[init]
script=$HOME/Development/gnomon-scripts/create_jupyter_console.py
```

## Launch gnomonGrowthSimulator

In order to check the validity of the installation, one can launch gnomonGrowthSimulator executable as follows:

``` shell
cd $HOME/Development/gnomon/build
./bin/gnomonGrowthSimulator.app/Contents/MacOS/gnomonGrowthSimulator
```

Then in the research field, one can look for gnomon and check that at least one node from gnomon is available. One can then drag and drop it into the composer. Eventually, one can select the node and check in th left panel whether an implementation is available.

## Omero layer 5.2.7 (server = tissuelab.inria.fr)

WARNING: this section should be removed/rewritten then gnomon.inria.fr will be
installed. See next section.

We need to install omero C++ and zeroc-ice. Omero MUST be at the same level than the server. At the moment,
the version is 5.2.7 omn tissuelab.inria.fr

Omero depens on ICE (https://zeroc.com/products/ice)

For some linux flavors, need to recompile from source https://github.com/zeroc-ice/ice

``` shell
$ source activate gnomon-1.x.x
$ git clone -b 3.7 https://github.com/zeroc-ice/ice.git
$ make
$ make install
```

which will install the libs in /opt/Ice-3.7.0

For macOSX:

``` shell
$ brew install zeroc-ice/tap/ice
```


Omero will be installed from the sources:
http://downloads.openmicroscopy.org/omero/5.2.7/artifacts/openmicroscopy-5.2.7.zip

Prérequisite:
- ice (cf before)
- JDK: http://download.oracle.com/otn-pub/java/jdk/8u151-b12/e758a0de34e24606bca991d704f6dcbf/jdk-8u151-macosx-x64.dmg
- JRE: http://download.oracle.com/otn-pub/java/jdk/8u151-b12/e758a0de34e24606bca991d704f6dcbf/jre-8u151-macosx-x64.dmg

WARNING: omero-5.2.7 DOES NOT BUILD with java9 !!!!!

``` shell
$ source activate gnomon-1.x.x
$ wget http://downloads.openmicroscopy.org/omero/5.2.7/artifacts/openmicroscopy-5.2.7.zip
$ unzip openmicroscopy-5.2.7.zip
$ cd openmicroscopy-5.2.7
$ ./build.py build-cpp
```

The compilation is done in: openmicroscopy-5.2.7/target/OMERO.cpp-5.2.7-ice36-Mac OS X-10.12.6-x86_64
The omero-client_DIR and omero-ice_DIR looks like:
~/Development/openmicroscopy-5.2.7/target/OMERO.cpp-5.2.7-ice36-Mac OS
X-10.12.6-x86_64/lib/cmake


## Omero layer 5.4.7 (server = gnomon.inria.fr)

The new server is running omero-5.4.7 on gnomon.inria.fr

Only tested on macOSX (10.13)

* get the sources from
  http://downloads.openmicroscopy.org/omero/5.4.7/artifacts/openmicroscopy-5.4.7.zip
* cd ~/Development/openmiscroscopy-5.4.7
* mkdir build INSTALL
* cd build
* cmake ..
  -DCMAKE_INSTALL_PREFIX:PATH=$HOME/Developement/openmicroscopy-5.4.7/INSTALL
* make
* make install


In order to compile gnomon with openmicroscopy layer, do:

* cmake -Domero-ice_DIR:PATH=$HOME/Development/openmicroscopy-5.4.7/INSTALL/lib/cmake  -Domero-client_DIR:PATH=$HOME/Development/openmicroscopy-5.4.7/INSTALL/lib/cmake -DVTK_DIR:PATH=$HOME/Development/VTK-8.0.0/build ..

## Ice install

### find the git tag

``` shell
cd "$(brew --repo homebrew/core)"
git log master -- Formula/ice.rb
# search for the specifiec version you need ( here <= 3.6.3 )
```

###  checkout the specific version

``` shell
cd "$(brew --repo homebrew/core)" && git checkout a2abaa62e3575aca4ffcb337ca00a9aa6ddccd4b
HOMEBREW_NO_AUTO_UPDATE=1 brew install ice
```

### do back to master

``` shell
git -C "$(brew --repo homebrew/core)" checkout master
```

### link to the ice version you want to use

``` shell
brew unlink ice@3.6
brew link ice
```
### Special directives for compilation

#### linux

``` shell
export ICE_HOME=/opt/Ice-6.4.2
```
