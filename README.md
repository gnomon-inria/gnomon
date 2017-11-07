# Installation

For sake of clarity, let's consider that all the programs are installed under `$HOME/Development` folder.


## Pre-requisites stuff

### Using Conda environment:
If you want to use a Conda environment, here named `gnomon-dtk` (or a VirtualEnv), create it using the following recipe (for VirtualEnv install dependencies):
``` yml
name: gnomon-dtk
channels:
  - defaults
dependencies:
  - python=2.7
  - ipython-qtconsole
  - jinja2
  - numpy
  - scipy
  - matplotlib
  - pandas
  - zeroc-ice
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
git tag
git checkout timagetkRelease1.0.0
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

### dtk

``` shell
cd $HOME/Development
git clone https://github.com/d-tk/dtk.git
cd dtk
git checkout develop
mkdir build
cd build
cmake .. -DDTK_WRAPPING_PYTHON=ON -DDTK_BUILD_COMPOSER=ON -DDTK_BUILD_DISTRIBUTED=ON -DDTK_BUILD_SCRIPT=ON -DDTK_BUILD_WIDGETS=ON -DDTK_BUILD_WRAPPERS=ON -DDTK_BUILD_SUPPORT_COMPOSER=ON -DDTK_BUILD_SUPPORT_CONTAINER=ON -DDTK_BUILD_SUPPORT_CORE=ON -DDTK_BUILD_SUPPORT_GUI=ON -DDTK_BUILD_SUPPORT_MATH=ON
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

## Launch gnomonGrowthSimulator

In order to check the validity of the installation, one can launch gnomonGrowthSimulator executable as follows:

``` shell
cd $HOME/Development/gnomon/build
./bin/gnomonGrowthSimulator
```

Then in the research field, one can look for gnomon and check that at least one node from gnomon is available. One can then drag and drop it into the composer. Eventually, one can select the node and check in th left panel whether an implementation is available.


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
Example here with a conda environment named `gnomon-dtk`:
``` shell
source activate gnomon-dtk
cd $HOME/Development/timagetk
python setup.py --prefix=$CONDA_ENV_PATH
```


### TimageTK, the image toolkit

Clone TimageTK source code:

``` shell
cd $HOME/Development/
git clone https://github.com/VirtualPlants/timagetk.git
```

Install it under the Conda environment (here named `gnomon-dtk`):
``` shell
source activate gnomon-dtk
cd timagetk
python setup.py --prefix=$CONDA_ENV_PATH
```


### tissue_analysis, the cell quantification toolkit

Clone tissue_analysis source code:

``` shell
cd $HOME/Development/
git clone https://github.com/VirtualPlants/tissue_analysis.git
```

Install it under the Conda environment (here named `gnomon-dtk`):
``` shell
source activate gnomon-dtk
cd tissue_analysis
python setup.py --prefix=$CONDA_ENV_PATH
```



