# Installation

For sake of clarity, let's consider that all the programs are installed under `$HOME/Development` folder.


## Pre-requisites stuff

### Using Conda environment:
If you want to use a Conda environment, here named `gnomon` (or a VirtualEnv), create it using the following recipe (for VirtualEnv install dependencies):
``` yml
name: gnomon
channels:
  - defaults
dependencies:
  - python=2.7
  - ipython-qtconsole
  - numpy
  - scipy
  - matplotlib
  - sip
  - pandas
  - zeroc-ice
```
To create the conda environment using the previous recipe saved under `gnomon.yml`:
```shell
conda env create -f gnomon.yml
```
To activate the conda environment:
``` shell
source activate gnomon
```

Inside this environment install dtk, its applicative layers, gnomon and its plugins by cloning the source code.

Pre-requisites might be installed outside the environment.

### OpenGL Stuff (at least for ubuntu)

``` shell
sudo apt install freeglut3-dev
sudo apt install mesa-common-dev
```

### MacOS config
???

### CMake 3.9 version

Uninstall previous version if older.
Check that `libncurses5-dev` or equivalent is installed, then do the following:

``` shell
cd $HOME/Development
wget https://cmake.org/files/v3.9/cmake-3.9.4.tar.gz
tar -zxvf cmake-3.9.4.tar.gz
cd cmake-3.9.4/
./configure
make -j4
sudo make install
sudo updatedb
```

### Qt5 installation on ubuntu

Download and install the latest release of Qt5 (here 5.9.2):
``` shell
cd $HOME/Development
wget http://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run
chmod +x qt-unified-linux-x64-online.run
./qt-unified-linux-x64-online.run
```

In the dialog tool, select `$HOME/Development/Qt` as the place to install Qt.
In this folder, one has to create a conf file for qtchooser as follows:

``` shell
cd $HOME/Development/Qt
echo "$HOME/Development/Qt/5.9.2/gcc_64/bin
$HOME/Development/Qt/5.9.2/gcc_64" > qt592.conf
```

To enable Qt5 and define environment variables, add the following lines to the `~/.bashrc` file:

``` shell
## #################################################################
## Qt selection
## #################################################################

export QT_SELECT=qt592
export Qt5_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5'
export Qt5Core_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5Core'
export Qt5Concurrent_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5Concurrent'
export Qt5Gui_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5Gui'
export Qt5Widgets_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5Widgets'
export Qt5Test_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5Test'
export Qt5OpenGL_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5OpenGL'
export Qt5PrintSupport_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5PrintSupport'
export Qt5Xml_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5Xml'
export Qt5SerialBus_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5SerialBus'
export Qt5Network_DIR='$HOME/Development/5.9.2/gcc_64/lib/cmake/Qt5Network'
```

It remains to set this config as the default one for qtchooser.
One has to create as sudoer a symbolic link to qt592.conf file into `/usr/lib/x86_64-linux-gnu/qtchooser`.

### Qt5 wrapping

#### Qt objects within Python environnement using sip.

Uninstall already existing version of sip if its version is less than 4.19 ("sip -V" to check the version), otherwise skip to PyQt5 installation.
Since the latest sip version of some packages managers is not up to date with the latest PyQt5, you can install them by hand from the sources.

Dowload and install sip :
``` shell
cd $HOME/Development
wget https://sourceforge.net/projects/pyqt/files/sip/sip-4.19.5/sip-4.19.5.tar.gz
tar -zxvf sip-4.19.5.tar.gz
cd sip-4.19.5
python2.7 configure.py
sudo make install
```

Make sure that the folder containing the qmake executable of the Qt librairies you have installed is the only folder containing a qmake executable, namely "$HOME/Development/Qt/5.9.2/gcc_64/bin". You can prepend it to your PATH, modify your .bashrc by adding : PATH=$HOME/Development/Qt/5.9.2/gcc_64/bin:$PATH to it.

Download and install PyQt5 :
``` shell
cd $HOME/Development
wget https://sourceforge.net/projects/pyqt/files/PyQt5/PyQt-5.9.1/PyQt5_gpl-5.9.1.tar.gz
tar -zxvf PyQt5_gpl-5.9.1.tar.gz
cd PyQt5_gpl-5.9.1
python2.7 configure.py
sudo make install
```

#### Qt objects within Python environnement using SWIG.

Install swig development packages with RPM.

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
cmake .. -DVTK_Group_Qt=ON -DVTK_QT_VERSION=5 -DVTK_RENDERING_BACKEND=OpenGL2 -DModule_vtkGUISupportQtOpenGL=ON
make -j4
```

### Morpheme 'vt' installation
<!-- Why not use the one packaged with timagetk ?! -->

Start by cloning the Morpheme source code, replacing `mylogin` with you INRIA forge login:
``` shell
cd $HOME/Development
git clone git+ssh://mylogin@scm.gforge.inria.fr/gitroot/morpheme-privat/morpheme-privat.git
```

Checkout the first tagged release (for timagetk) named "timagetkRelease1.0.0":
``` shell
git branch openalea_wrapper_v1.7 origin/openalea_wrapper_v1.7
git checkout openalea_wrapper_v1.7
```

Compile `vt` library as follow (dependency with 'lemon' & 'vtk'):
``` shell
cd morpheme-privat/vt
mkdir build
cd build
cmake ..
make -j4
```
<!-- Compilation flags to be defined -->
<!-- LOG: ccmake .. -->
<!-- GNU CXX COMPILER

 WARNING, lemon was NOT found

 WARNING, tracker will NOT be built

 WARNING, vtk was either not found or too old

 WARNING, library ' libfilters' building will NOT be complete

 WARNING, executables ' extraction_arbre' and 'classification' will NOT be
 built -->


## Get dtk and its applicative layers

### MacOS case

For MacOs users, in every cmake configurations that follow, one has to add the following cmake option:

``` shell
-DPYTHON_INCLUDE_DIR=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7
```

### Compilation options:

#### Python wrapping:
To enable python wrapping use compilation flag: ` -DDTK_WRAPPING_PYTHON=ON`

To enable SWIG wrapping use compilation flag: ` -DDTK_PYTHON_WRAPPER=SWIG`

To enable SIP wrapping use compilation flag: ` -DDTK_PYTHON_WRAPPER=SIP`

To enable both SIP and SWIG wrapping use compilation flag: ` -DDTK_PYTHON_WRAPPER=SWIG_AND_SIP`

### dtk


``` shell
cd $HOME/Development
git clone https://github.com/d-tk/dtk.git
cd dtk
git checkout develop
mkdir build
cd build
cmake .. -DDTK_WRAPPING_PYTHON=ON -DDTK_BUILD_COMPOSER=ON -DDTK_BUILD_DISTRIBUTED=ON -DDTK_BUILD_SCRIPT=ON -DDTK_BUILD_WIDGETS=ON -DDTK_BUILD_WRAPPERS=ON -DDTK_BUILD_SUPPORT_COMPOSER=ON -DDTK_PYTHON_WRAPPER=SWIG_AND_SIP -DDTK_BUILD_SUPPORT_CONTAINER=ON -DDTK_BUILD_SUPPORT_CORE=ON -DDTK_BUILD_SUPPORT_GUI=ON -DDTK_BUILD_SUPPORT_MATH=ON
make -j4
```

### dtk-imaging

``` shell
cd $HOME/Development
git clone https://github.com/d-tk/dtk-imaging.git
cd dtk-imaging
git checkout develop
mkdir build
cd build
cmake .. -Ddtk_DIR=$HOME/Development/dtk/build
make -j4
```

### dtk-plugins-imaging

``` shell
cd $HOME/Development
git clone https://github.com/d-tk/dtk-plugins-imaging.git
cd dtk-plugins-imaging
git checkout develop
mkdir build
cd build
cmake .. -Ddtk_DIR=$HOME/Development/dtk/build -DdtkImaging_DIR=$HOME/Development/dtk-imaging/build -DVTK_DIR=$HOME/Development/VTK-8.0.1/build -Dvt_DIR=$HOME/Development/morpheme-privat/vt/build
make -j4
```

### dtk-discrete-geometry

``` shell
cd $HOME/Development
git clone https://github.com/d-tk/dtk-discrete-geometry.git
cd dtk-discrete-geometry
git checkout develop
mkdir build
cd build
cmake .. -Ddtk_DIR=$HOME/Development/dtk/build -DdtkImaging_DIR=$HOME/Development/dtk-imaging/build -DDTKDISCRETEGEOMETRY_USES_DTKIMAGING=ON
make -j4
```

## Get gnomon & gnomon-plugins

### gnomon

To select light or dark theme for gnomon set the compilation flag ` -DGNOME_STYLE=ONELIGHT|ONEDARK`.

``` shell
cd $HOME/Development
git clone git@gitlab.inria.fr:gnomon/gnomon.git
cd gnomon
git checkout develop
mkdir build
cd build
cmake .. -Ddtk_DIR=$HOME/Development/dtk/build -DdtkImaging_DIR=$HOME/Development/dtk-imaging/build -DdtkDiscreteGeometry_DIR=$HOME/Development/dtk-discrete-geometry/build -DVTK_DIR=$HOME/Development/VTK-8.0.1/build
make -j4
```


### gnomon-plugins

``` shell
cd $HOME/Development
git clone git@gitlab.inria.fr:gnomon/gnomon-plugins.git
cd gnomon-plugins
git checkout develop
mkdir build
cd build
cmake .. -Ddtk_DIR=$HOME/Development/dtk/build -Dgnomon_DIR=$HOME/Development/gnomon/build -DVTK_DIR=$HOME/Development/VTK-8.0.1/build
make -j4
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
Add to `dtk-scripts.ini`:
``` shell
[init]
script=$HOME/Development/gnomon-scripts/create_jupyter_console.py
```

## Launch gnomonGrowthSimulator

In order to check the validity of the installation, one can launch gnomonGrowthSimulator executable as follows:

``` shell
cd $HOME/Development/gnomon/build
./bin/gnomonGrowthSimulator
```

Then in the research field, one can look for gnomon and check that at least one node from gnomon is available. One can then drag and drop it into the composer. Eventually, one can select the node and check in th left panel whether an implementation is available.

## omero layer

We need to install omero C++ and zeroc-ice. Omero MUST be at the same level than the server. At the moment,
the version is 5.2.7

Omero depens on ICE (https://zeroc.com/products/ice)

For some linux flavors, need to recompile from source https://github.com/zeroc-ice/ice

'''
$ git clone -b 3.7 https://github.com/zeroc-ice/ice.git
$ make
$ make install
'''
which will install the libs in /opt/Ice-3.7.0

For macOSX:

'''
$ brew install zeroc-ice/tap/ice
'''

Omero will be installed from the sources:
http://downloads.openmicroscopy.org/omero/5.2.7/artifacts/openmicroscopy-5.2.7.zip

Prérequisite:
- ice (cf before)
- JDK: http://download.oracle.com/otn-pub/java/jdk/8u151-b12/e758a0de34e24606bca991d704f6dcbf/jdk-8u151-macosx-x64.dmg
- JRE: http://download.oracle.com/otn-pub/java/jdk/8u151-b12/e758a0de34e24606bca991d704f6dcbf/jre-8u151-macosx-x64.dmg

WARNING: omero-5.2.7 DOES NOT BUILD with java9 !!!!!

'''
$ wget http://downloads.openmicroscopy.org/omero/5.2.7/artifacts/openmicroscopy-5.2.7.zip
$ unzip openmicroscopy-5.2.7.zip
$ cd openmicroscopy-5.2.7
$ ./build.py build-cpp
$
'''

The compilation is done in: openmicroscopy-5.2.7/target/OMERO.cpp-5.2.7-ice36-Mac OS X-10.12.6-x86_64


## install an old version of ice

### find the git tag
'''
cd "$(brew --repo homebrew/core)"
git log master -- Formula/ice.rb
 -> search for the specifiec version you need ( here <= 3.6.3 )
'''

###  checkout the specific version
'''
cd "$(brew --repo homebrew/core)" && git checkout a2abaa62e3575aca4ffcb337ca00a9aa6ddccd4b
HOMEBREW_NO_AUTO_UPDATE=1 brew install ice
'''

### do back to master
'''
git -C "$(brew --repo homebrew/core)" checkout master
'''

### link to the ice version you want to use
'''
brew unlink ice@3.6
brew link ice
'''


# special directive compilation

## macOSX
...

## linux
export ICE_HOME=/opt/Ice-6.4.2

## Optionals

TimageTK and tissue_analysis are pure python packages, to install them uses the `setup.py` with the following option depending on the type of install you would like:

  * System-wide install:
``` shell
python setup.py install
```

  * User specific install:
``` shell
python setup.py install --user
```

  * System-wide "developer install":
``` shell
python setup.py develop
```

  * User specific "developer install":
``` shell
python setup.py develop --user
```

  * Conda / VirtualEnv install:
If you are working under Conda or VirtualEnv activate the environment first, then use the `-prefix=` option to specify installation path.
Example here with a conda environment named `gnomon`:
``` shell
source activate gnomon
cd $HOME/Development/timagetk
python setup.py --prefix=$CONDA_ENV_PATH
```


### TimageTK, the image toolkit

Clone TimageTK source code:

``` shell
cd $HOME/Development/
git clone https://github.com/VirtualPlants/timagetk.git
```

Install it under the Conda environment (here named `gnomon`):
``` shell
source activate gnomon
cd timagetk
python setup.py --prefix=$CONDA_ENV_PATH
```


### tissue_analysis, the cell quantification toolkit

Clone tissue_analysis source code:

``` shell
cd $HOME/Development/
git clone https://github.com/VirtualPlants/tissue_analysis.git
```

Install it under the Conda environment (here named `gnomon`):
``` shell
source activate gnomon
cd tissue_analysis
python setup.py --prefix=$CONDA_ENV_PATH
```
