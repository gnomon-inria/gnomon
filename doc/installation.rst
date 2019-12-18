=================
Installing Gnomon
=================


Conda package
=============

Install
--------------------------------------------------------------------------------

Create a new conda environment (recommended)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block::

    $ conda create -n gnomon -c dtk -c mosaic -c gnomon gnomon plugin_tissueimage

.. note:: the name of the new environment (here: 'gnomon') is defined by the option -n


In an existing conda environment containing already Python 3.7.5+
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block::

    $ conda install -c dtk -c mosaic -c gnomon gnomon plugin_tissueimage

Launching Gnomon
--------------------------------------------------------------------------------
First activate the ``gnomon`` environment, then launch the executable:

.. code-block::

    $ conda activate gnomon
    $ gnomon

.. note:: On linux, it is possible to defines an alias to simplify this step:

    If you answered *yes* to "Do you wish the installer to initialize Miniconda3 by running conda init? [yes|no]" during miniconda3 installation or called ``conda init bash``:

    .. code-block::

        $ echo "conda activate gnomon && gnomon" >> ~./bashrc

    Else:

    .. code-block::

        $ echo "source <path_to_miniconda3>/bin/activate && conda activate gnomon && gnomon" >> ~./bashrc

    where ``<path_to_miniconda3>`` is your path to miniconda3 install: ``~/miniconda3`` by default.

Updating Gnomon
--------------------------------------------------------------------------------
First activate the *gnomon* environment, then update the environment from conda channels dtk, mosaic & gnomon:

.. code-block::

    $ conda activate gnomon
    $ conda update -c dtk -c mosaic -c gnomon gnomon plugin_tissueimage

About Conda
--------------------------------------------------------------------------------
**Conda** is an open source package management system and environment management system that runs on Windows, macOS and Linux. Conda quickly installs, runs and updates packages and their dependencies.
Conda easily creates, saves, loads and switches between environments on your local computer.
See `<https://docs.conda.io/en/latest/>`_

**Miniconda** is a free minimal installer for conda.
It is a small, bootstrap version of Anaconda that includes only ``conda``, ``Python``, the packages they depend on, and a small number of other useful packages, including ``pip``, ``zlib`` and a few others.
See `<https://docs.conda.io/en/latest/miniconda.html>`_


From sources
=============

Install `gnomon`
--------------------------------------------------------------------------------
Clone `gnomon` sources:

.. code-block::

    $ git clone https://gitlab.inria.fr/gnomon/gnomon.git

Within the cloned `gnomon/` folder, create a conda environment:

.. code-block::

    $ conda env create -f conda/env.yaml

Activate the newly created environment:

.. code-block::

    $ conda activate gnomon-dev

(OPTIONAL) check the `libuuid` has been taken from `dtk` channel:

.. code-block::

    $ conda list libuuid

If not:

.. code-block::

    $ conda install -c dtk libuuid

Then, compile and install the sources:

.. code-block::

    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX -DCMAKE_PREFIX_PATH=$CONDA_PREFIX ..
    $ make -j install

If all went well, you can now start the app:
* On linux:

.. code-block::

    $ ./bin/gnomon

* On macOs:

.. code-block::

    $ ./bin/gnomon.app/Contents/MacOS/gnomon


Install ``gnomon-packages-tissueimage``
--------------------------------------------------------------------------------
To perform tissue image analysis, you need to clone & install the ``gnomon-packages-tissueimage`` sources:

.. code-block::

    $ git clone https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimage.git
    $ cd gnomon-package-tissueimage
    $ python setup.py develop


Install ``gnomon-package-data`` (OPTIONAL)
--------------------------------------------------------------------------------
To test ``gnomon`` or use example scripts you need to clone & install the ``gnomon-data`` sources:

.. code-block::

    $ git clone https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-data.git
    $ cd gnomon-package-data
    $ python setup.py develop

Desinstall a gnomon environment
--------------------------------------------------------------------------------

.. code-block::

    $ conda deactivate (if you are not already in (base) environment)
    $ conda env remove -n gnomon
    $ conda create -n gnomon -c dtk -c gnomon -c mosaic gnomon plugin-tissueimage
    $ conda activate gnomon
