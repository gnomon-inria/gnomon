# Installation

For sake of clarity, let's consider that all the programs are installed under `$HOME/Development` folder.

## Pre-requisites stuff

### OpenGL Stuff (at least for ubuntu)

``` shell
sudo apt install freeglut3-dev
sudo apt install mesa-common-dev
sudo apt install uuid-dev
```

### CMake

``` shell
apt install cmake cmake-curses-gui # ubuntu
dnf install cmake # fedora
brew install cmake # mac
```

### Install Conda

``` shell
cd ~/Downloads
brew install wget
wget https://repo.continuum.io/miniconda/Miniconda2-latest-MacOSX-x86_64.sh
chmod u+x Miniconda2-latest-MacOSX-x86_64.sh
./Miniconda2-latest-MacOSX-x86_64.sh
# install in $HOME/.conda
```

Then, make sure to have conda in your PATH environment variable, before any folder containing `qmake`.

### Create Conda environment:

First clone gnomon-recipes:

``` shell
cd $HOME/Development
git clone git@gitlab.inria.fr:gnomon/gnomon-recipes.git
cd gnomon-recipes/environment
```

To create the conda environment :
```shell
conda env create -f gnomon-1.x.x.yml
```
To activate the conda environment:
``` shell
source activate gnomon-1.x.x
```

This environment installation / upgrade will pull for you : dtk, dtk-imaging, dtk-imaging-plugins (and its dependencies : itk, vtk...).

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
cmake .. -DGNOMON_STYLE:STRING=ONEDARK
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
cmake .. -Dgnomon_DIR=$HOME/Development/gnomon/build
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
touch dtk-script.ini
touch gnomon-core.ini
```

### Gnomon config

Edit `gnomon-core.ini` file and add the following lines replacing `<$HOME>` by the **real path**:
**Optional: Enable jupyter console by default in Gnomon** (instead of python)

``` shell
[gnomon-core]
plugins=<$HOME>/Development/gnomon-plugins/build/lib

[modules]
path=<$HOME>/Development/gnomon/build/lib/python2.7/site-packages/:<$HOME>/Development/gnomon-plugins/src/Python/

[init]
script=<$HOME>/Development/gnomon-scripts/1.x.x/create_jupyter_console.py
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
