=================
Installing Gnomon
=================

Conda package
=============

Installation by creating a new conda environment (recommended)
--------------------------------------------------------------

.. code-block::

    > conda create -n gnomon -c dtk -c mosaic -c gnomon -c conda-forge gnomon plugin_tissueimagemesh

note: the name of the new environment (here: 'gnomon') is defined by the option -n

Installation in an existing conda environment containing already Python 3.6+
-----------------------------------------------------------------------------

.. code-block::

    > conda install -c dtk -c mosaic -c gnomon -c conda-forge gnomon plugin_tissueimagemesh


About Conda
-----------

**Conda** is an open source package management system and environment management system that runs on Windows, macOS and Linux. Conda quickly installs, runs and updates packages and their dependencies. Conda easily creates, saves, loads and switches between environments on your local computer. See `<https://docs.conda.io/en/latest/>`_

**Miniconda** is a free minimal installer for conda. It is a small, bootstrap version of Anaconda that includes only conda, Python, the packages they depend on, and a small number of other useful packages, including pip, zlib and a few others. See `<https://docs.conda.io/en/latest/miniconda.html>`_

From sources
=============

Clone `gnomon` sources:

.. code-block::

    $ git clone https://gitlab.inria.fr/gnomon/gnomon.git

Create a conda environment:

.. code-block::

    $ conda env create -f conda/env.yaml

Activate the newly created environment:

.. code-block::

    $ conda activate gnomon-dev

(OPTIONAL) check the libuuid has been taken from `dtk` channel:

.. code-block::

    $ conda list libuuid

If not:

.. code-block::

    $ conda install -c dtk libuuid

Then we compile and install the sources:

.. code-block::

    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX -DCMAKE_PREFIX_PATH=$CONDA_PREFIX ..
    $ make -j install

If all went well, you can nom start the app:

.. code-block::

    $ ./bin/gnomon

=================
Launching Gnomon
=================

First activate the gnomon environment, they launch the executable:

.. code-block::

    > conda activate gnomon
    > gnomon

=================
Updating Gnomon
=================

First activate the gnomon environment, then update the environnement from conda channels dtk, mosaic, gnomon:

.. code-block::

    > conda activate gnomon
    > conda update -c dtk -c mosaic -c gnomon gnomon gnomon-plugins
