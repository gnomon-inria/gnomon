# Installation

## Prerequisites stuff

For sake of clarity, let's consider that all the programs are installed under `$HOME/Development` folder.

### OpenGL Stuff (at least for ubuntu)

``` shell
sudo apt install freeglut3-dev
sudo apt install mesa-common-dev
```

### MacOS config


### CMake 3.9 version

Uninstall previous version if older. Check that `libncurses5-dev` or equivalent is installed, then do the following:

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

``` shell
cd $HOME/Development
wget http://download.qt.io/official_releases/online_installers/qt-unified-linux-x64-online.run
chmod +x qt-unified-linux-x64-3.0.1-online.run
```

In the dialog tool, select `$HOME/Development/Qt` as the place to install Qt. In this folder, one has to create a conf file for qtchooser as follows:

``` shell
cd $HOME/Development/Qt
echo "$HOME/Development/Qt/5.9.2/gcc_64/bin
$HOME/Development/Qt/5.9.2/gcc_64" > qt592.conf
```

In the bashrc file, one can add the following lines:

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

It remains to set this config as the default one for qtchooser. One has to create as sudoer a symbolink link to qt592.conf file into `/usr/lib/x86_64-linux-gnu/qtchooser`.


### VTK8 installation

``` shell
cd $HOME/Development
wget http://www.vtk.org/files/release/8.0/VTK-8.0.1.tar.gz
tar -zxvf VTK-8.0.1.tar.gz
cd VTK-8.0.1/
mkdir build
cd build
cmake .. -DVTK_Group_Qt=ON -DVTK_QT_VERSION=5 -DVTK_RENDERING_BACKEND=OpenGL2 -DModule_vtkGUISupportQtOpenGL=ON
make -j4
```


### Vt installation

``` shell
cd $HOME/Development
git clone git+ssh://mylogin@scm.gforge.inria.fr/gitroot/morpheme-privat/morpheme-privat.git
cd morpheme-privat/vt
mkdir build
cd build
cmake ..
make -j4
```

## dtk and applicative layers installation

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

### dtk imaging

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

### dtk plugins imaging

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

### dtk discrete geometry

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

## Gnomon plugins

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


### gnomon plugins

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

### dtk imaging config

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
