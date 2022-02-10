===================================================================================
Defining a Python plugin for an existing algorithm on existing form data structures
===================================================================================

Create a Python module for the plugin
=====================================

Create a Python class inheriting the algorithm abstraction
----------------------------------------------------------

.. topic:: Image filter plugin

    In the follwoing example, we will create a plugin class that performs a Gaussian smoothing of a multichannel image. It is related to the ``gnomonAbstractImageFilter`` abstraction that encompasses all algorithms that input an image and output another image, transformed by a filter of some kind.

.. code-block::

    linearFilterTimagetk.py

.. code-block::
    python

    from gnomon.core import gnomonAbstractImageFilter

    class linearFilterTimagetk(gnomonAbstractImageFilter):

        def __init__(self):
            super(linearFilterTimagetk, self).__init__()


* Register the class to the Gnomon plugin machinery

.. topic:: ``gnomonPlugin`` decorator

    Gnomon defines a Python class decorator that implements all the necessary methods to register the plugin to the platform, with just one line of code.

.. warning::

    Since the algorithm is imported from the ``gnomoncore`` module, the plugin should be registered to the ``gnomoncore`` namespace.

.. code-block::
    python

    import gnomon.core
    from gnomon.core import gnomonAbstractImageFilter

    from gnomon.utils import gnomonPlugin

    @gnomonPlugin(namespace=gnomoncore)
    class linearFilterTimagetk(gnomonAbstractImageFilter):


Provide the specifications of the plugin
========================================

Define the inputs and outputs of the plugin
-------------------------------------------

.. note::

    Python decorators allow to map the attributes of the class that represent the temporal series of forms (in the example case, images) to the setter and getter methods of the abstraction.

.. topic:: Inputs

    The ``gnomonAbstractImageFilter`` abstraction requires an image as input. Therefore we use the ``gnomonImageInput`` decorator to make the link between the ``images`` attribute of the class (that will contain the input images) and the accessor methods from the signature of the abstraction : ``setInput`` and ``input``.

.. topic:: Outputs

    The ``gnomonAbstractImageFilter`` abstraction returns an image as output. Therefore we use the ``gnomonImageOutput`` decorator to make the link between the ``filtered_images`` attribute of the class (that will contain the output images) and the accessor method from the signature of the abstraction : ``output``.

.. warning::

    The class attributes are Python dictionaries for which the keys are temporal indices and values are data structures representing the forms. In the example case, they are assumed to be the default data structures representing intensity images : a dictionnary of instances ``SpatialImage`` class from the ``timagetk`` Python library for which keys are channel names.

.. code-block::
    python

    from gnomon.utils.gnomonDecorator import gnomonImageInput, gnomonImageOutput

    @gnomonPlugin(namespace=gnomoncore)
    @gnomonImageInput(attr='images',method='input',setter_method='setInput')
    @gnomonImageOutput(attr='filtered_images',method='output')
    class linearFilterTimagetk(gnomonAbstractImageFilter):

        def __init__(self):
            super(linearFilterTimagetk, self).__init__()

            self.images = {}
            self.filtered_images = {}

Define the parameters of the algorithm
--------------------------------------

.. note::

    Gnomon provides a number of parameter types that ar accessible in the ``gnomoncore`` module, notably for integers, real numbers, character strings, boolean and so on.

.. topic:: Parameters

    In the example case, we define a single parameter that corresponds to the sigma of the Gaussian kernel used for the filtering of the image. As it is a real-valued parameter, we use the ``parameterDouble`` class from the ``gnomoncore`` module to define the parameter (type, value, constraints and description).

.. code-block::
    python

    gnomoncore.ParameterDouble(1., 0, 10., 2, "Standard deviation of the Gaussian kernel")

.. note::

    Parameters are defined as a dictionary attribute ``_parameters`` of the plugin class for which keys are the names of the parameter

.. topic:: ``gnomonParametric`` decorator

    The ``gnomonParametric`` Python class decorator allows to map the parameters to a graphical rendering in the Gnomon interface by defining all the necessary functions. It also provides a shortened access to the parameter values as ``self['parameter_name']``.

.. code-block::
    python

    import gnomon.core
    from gnomon.core import gnomonAbstractImageFilter

    from gnomon.utils import gnomonPlugin
    from gnomon.utils import gnomonParametric
    from gnomon.utils.gnomonDecorator import gnomonImageInput, gnomonImageOutput

    @gnomonPlugin(namespace=gnomoncore)
    @gnomonParametric
    @gnomonImageInput(attr="images",method='input',setter_method='setInput')
    @gnomonImageOutput(attr="filtered_images",method='output')
    class linearFilterTimagetk(gnomonAbstractImageFilter):

        def __init__(self):
            super(linearFilterTimagetk, self).__init__()

            self.images = {}
            self.filtered_images = {}

            self._parameters = {}
            self._parameters['gaussian_sigma'] = gnomoncore.ParameterDouble(1., 0, 10., 2,
                "Standard deviation of the Gaussian kernel")


Implement the plugin
====================

Write the ``run`` method
------------------------

.. topic:: ``run``

    This is where the actual code of the plugin is written, and it generally has always the same structure

    - set the outputs to empty dictionaries
    - iterate over the time points of the inputs
    - get the input data structures at the current time
    - compute the outputs from the inputs (actual algorithm)
    - fill the outputs data structures for the current time

.. note::

    The ``run`` method does not return anything, it simply updates the values of the output data structures based on the content of the input data structures

.. topic:: Algorithm code

    In the example case, the algorithm is implemented in a function of the ``timagetk`` Python package that we therefore need to import beforehand. The function is called to compute the Gaussian smoothing of each image channel at each time point, and fill the output dictionary ``filtered_images`` with the results.

.. code-block::
    python

    from timagetk.plugins.linear_filtering import linear_filtering

.. code-block::
    python

        def run(self):
            self.filtered_images = {}

            for time in self.images.keys():
                self.filtered_images[time] = {}

                for channel in self.images[time].keys():
                    img = self.images[time][channel]
                    filtered_img = linear_filtering(img, method='gaussian_smoothing', sigma=self['gaussian_sigma'])
                    self.filtered_images[time][channel] = filtered_img

.. topic:: Parameter values

    Within the code of the ``run`` function, the parameters will be used to perform the computations and can be accessed easily thanks to the ``gnomonParametric`` decorator. Here for instance, the value of the ``gaussian_sigma`` parameter (that might have been set manually by the user using a graphical interface) is passed to the function as ``self['gaussian_sigma']``.

Provide a documentation for the plugin
======================================

Fill in the class docstring
---------------------------

.. topic:: NumPy style docstring

    You can provide a description text that will be included in the application and displayed when your plugin is selected by the user. It should follow the NumPy style for Python docstrings and contain at least

    - A short description on the first line (less than 80 characters)
    - A brief description paragraph detailing the algorithm and its parameters

.. code-block::
    python

    class linearFilterTimagetk(gnomonAbstractImageFilter):
    """Compute the Gaussian smoothing of an image.

    The algorithm performs a filtering of all the channels of a 3D image by an
    isotropic Gaussian kernel of standard deviation equal to the value of the
    gaussian_sigma parameter.
    """

Enable the dynamical discovery of the plugin
============================================

Edit the setup.py of the Python package
---------------------------------------

.. topic:: Entry points

    To be dynamically discovered by the Gnomon platform, the plugin class should be referenced in the entry points of your Python interpreter. To do so, Gnomon offers a function that introspects a package looking for Gnomon plugins, and include them in a way that they will be found by the platform. This is done by adding the following lines to the ``setup.py`` of the Python package containing the plugin module.

.. code-block::

    setup.py

.. code-block::
    python


    from gnomon.utils.gnomonPlugin import gnomon_declare_plugins

    setup_kwds['entry_points'] = gnomon_declare_plugins('src/package_name')

.. warning::

    You need to make sure that the path passed to the ``gnomon_declare_plugins`` function contains the sources of the package, or at least all modules defining Gnomon plugins.


Complete plugin module
======================

.. code-block::
    python

    import gnomon.core
    from gnomon.core import gnomonAbstractImageFilter
    
    from gnomon.utils import gnomonPlugin
    from gnomon.utils import gnomonParametric
    from gnomon.utils.gnomonDecorator import gnomonImageInput, gnomonImageOutput

    from timagetk.plugins.linear_filtering import linear_filtering

    @gnomonPlugin(namespace=gnomoncore)
    @gnomonParametric
    @gnomonImageInput(attr="images",method='input',setter_method='setInput')
    @gnomonImageOutput(attr="filtered_images",method='output')
    class linearFilterTimagetk(gnomonAbstractImageFilter):
        """Compute the Gaussian smoothing of an image.

        The algorithm performs a filtering of all the channels of a 3D image by an
        isotropic Gaussian kernel of standard deviation equal to the value of the
        gaussian_sigma parameter.
        """

        def __init__(self):
            super(linearFilterTimagetk, self).__init__()

            self.images = {}
            self.filtered_images = {}

            self._parameters = {}
            self._parameters['gaussian_sigma'] = gnomoncore.ParameterDouble(1., 0, 10., 2,
                "Standard deviation of the Gaussian kernel")

        def run(self):
            self.filtered_images = {}

            for time in self.images.keys():
                self.filtered_images[time] = {}

                for channel in self.images[time].keys():
                    img = self.images[time][channel]
                    filtered_img = linear_filtering(img, method='gaussian_smoothing', sigma=self['gaussian_sigma'])
                    self.filtered_images[time][channel] = filtered_img
