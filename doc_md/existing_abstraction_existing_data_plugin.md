# Defining a Python plugin for an existing algorithm on existing form data structures

## Create a Python module for the plugin

### Create a Python class inheriting the algorithm abstraction



**Image filter plugin**

In the following example, we will create a plugin class that performs a
Gaussian smoothing of a multichannel image. It is related to the
`gnomonAbstractImageFilter` workspace that encompasses all algorithms
that input an image and output another image, transformed by a filter of
some kind.

If you want to choose another workspace/abstraction see [TODO](linktofile)

First, let's create the module in the algorithm section of the package 
(see [making a package](package.md) for more information on how to make
a plugin package)

    linearFilterTimagetk.py

``` python
from gnomon.core import gnomonAbstractImageFilter

class linearFilterTimagetk(gnomonAbstractImageFilter):

    def __init__(self):
        super().__init__()
```

In order for the plugin to be registered to the plugin factory some 
specific decorators must be added to the class.


**`gnomonPlugin` decorator**

Gnomon defines a Python class decorator that implements all the
necessary methods to register the plugin to the platform, with just one
line of code.

```{eval-rst}
.. py:function:: gnomonPlugin(version, coreversion, cls=None, namespace=gnomoncore, base_class=None)
   
   :param str version: version of the plugin
   :param str coreversion: version of gnomoncore
```


```{warning} 
Keep the cls arg equal to `None`
```

``` python
import gnomon.core
from gnomon.core import gnomonAbstractImageFilter

from gnomon.utils import corePlugin

@corePlugin(version="0.1.0", coreversion="0.60.0")
class linearFilterTimagetk(gnomonAbstractImageFilter):
```

```{warning}
Since the algorithm is imported from the `gnomoncore` module, the plugin
should be registered to the `gnomoncore` namespace.
```


## Provide the specifications of the plugin

### Define the inputs and outputs of the plugin

```{note}
Python decorators allow to map the attributes of the class that
represent the temporal series of forms (in the example case, images) to
the setter and getter methods of the abstraction.
```

**Inputs**

The `gnomonAbstractImageFilter` abstraction requires an image as input.
Therefore we use the `imageInput` decorator to make the link
between the `images` attribute of the class (that will contain the input
images) and the accessor methods from the signature of the abstraction :
`setInput` and `input`.


**Outputs**

The `gnomonAbstractImageFilter` abstraction returns an image as output.
Therefore we use the `imageOutput` decorator to make the link
between the `filtered_images` attribute of the class (that will contain
the output images) and the accessor method from the signature of the
abstraction : `output`.


```{warning}
The class attributes are Python dictionaries for which the keys are
temporal indices and values are data structures representing the forms.
In the example case, they are assumed to be the default data structures
representing intensity images : a dictionnary of instances
`SpatialImage` class from the `timagetk` Python library for which keys
are channel names.
```

``` python
from gnomon.utils.decorators import imageInput, imageOutput

@gnomonPlugin(version="0.1.0", coreversion="0.60.0")
@imageInput(attr='images')
@imageOutput(attr='filtered_images')
class linearFilterTimagetk(gnomonAbstractImageFilter):

    def __init__(self):
        super(linearFilterTimagetk, self).__init__()

        self.images = {}
        self.filtered_images = {}
```

### Define the parameters of the algorithm

To provide parameters for the algorithm which will be accessible from the interface, 
**gnomon** uses the parameter classes provided by the **dtkcore** module. 

**dtkcore** provides for a variety of classes for different types notably for 
integers, real numbers, character strings, boolean and so on:

#### simple
 - dtk::d_string

#### Numeric
 - dtk::d_uchar
 - dtk::d_char
 - dtk::d_uint
 - dtk::d_int 
 - dtk::d_real
 - dtk::d_bool

#### List parameters
 - dtk::d_inliststring
 - dtk::d_inlistreal
 - dtk::d_inlistint

#### List of List
 - dtk::d_inliststringlist

#### Path
 - dtk::d_path

#### Range
 - dtk::d_range_uchar
 - dtk::d_range_char
 - dtk::d_range_uint
 - dtk::d_range_int 
 - dtk::d_range_real



In the example case, we define a single parameter that corresponds to
the sigma of the Gaussian kernel used for the filtering of the image. As
it is a real-valued parameter, we use the `d_real` class from
the `dtkcore` module to define the parameter with the following prototype:

```{py:class} d_real(label: str, default_value: float, min: float, max: float, decimals: int, doc: str)

```

Parameters are defined as a dictionary attribute `_parameters` of the
plugin class for which keys are the names of the parameter. This dictionary must be
filled at init.

``` python
self._parameters["sigma"] = dtkcore.d_real("sigma", 1., 0, 10., 2, "Standard deviation of the Gaussian kernel")
```


**`gnomonParametric` decorator**

The `gnomonParametric` Python class decorator allows to map the
parameters to a graphical rendering in the Gnomon interface by defining
all the necessary functions. It also provides shortened access to the
parameter values as `self['parameter_name']`. It is included in the `corePlugin` decorator.


``` python
import gnomon.core
from gnomon.core import gnomonAbstractImageFilter

from gnomon.utils import corePlugin
from gnomon.utils.decorators import imageInput, imageOutput

import dtkcore

@corePlugin(version="0.1.0", coreversion="0.60.0")
@imageInput("images")
@imageOutput("filtered_images")
class linearFilterTimagetk(gnomonAbstractImageFilter):

    def __init__(self):
        super(linearFilterTimagetk, self).__init__()

        self.images = {}
        self.filtered_images = {}

        self._parameters = {}
        self._parameters["sigma"] = dtkcore.d_real("sigma", 1., 0, 10., 2, 
            "Standard deviation of the Gaussian kernel")
```

## Implement the plugin

### Write the `run` method

**`run`**

This is where the actual code of the plugin is written, and it generally
has always the same structure

- set the outputs to empty dictionaries
- iterate over the time points of the inputs
- get the input data structures at the current time
- compute the outputs from the inputs (actual algorithm)
- fill the outputs data structures for the current time

```{note}
The `run` method does not return anything, it simply updates the values
of the output data structures based on the content of the input data
structures
```


**Algorithm code**

In the example case, the algorithm is implemented in a function of the
`timagetk` Python package that we therefore need to import beforehand.
The function is called to compute the Gaussian smoothing of each image
channel at each time point, and fill the output dictionary
`filtered_images` with the results.


``` python
from timagetk.plugins.linear_filtering import linear_filtering
```

``` python
def run(self):
    self.filtered_images = {}

    for time in self.images.keys():
        self.filtered_images[time] = {}

        for channel in self.images[time].keys():
            img = self.images[time][channel]
            filtered_img = linear_filtering(img, method='gaussian_smoothing', sigma=self['gaussian_sigma'])
            self.filtered_images[time][channel] = filtered_img
```

**Parameter values**

Within the code of the `run` function, the parameters will be used to
perform the computations and can be accessed easily thanks to the
`gnomonParametric` decorator. Here for instance, the value of the
`gaussian_sigma` parameter (that might have been set manually by the
user using a graphical interface) is passed to the function as
`self['gaussian_sigma']`.


## Provide a documentation for the plugin

### Fill in the class docstring


**NumPy style docstring**

You can provide a description text that will be included in the
application and displayed when your plugin is selected by the user. It
should follow the NumPy style for Python docstrings and contain at least

- A short description on the first line (less than 80 characters)
- A brief description paragraph detailing the algorithm and its
  parameters



``` python
class linearFilterTimagetk(gnomonAbstractImageFilter):
"""Compute the Gaussian smoothing of an image.

The algorithm performs a filtering of all the channels of a 3D image by an
isotropic Gaussian kernel of standard deviation equal to the value of the
gaussian_sigma parameter.
"""
```

## Enable the dynamical discovery of the plugin

### Edit the setup.py of the Python package


**Entry points**

To be dynamically discovered by the Gnomon platform, the plugin class
should be referenced in the entry points of your Python interpreter. To
do so, Gnomon offers a function that introspects a package looking for
Gnomon plugins, and include them in a way that they will be found by the
platform. This is done by adding the following lines to the `setup.py`
of the Python package containing the plugin module.


    setup.py

``` python
from gnomon.utils.gnomonPlugin import gnomon_declare_plugins

setup_kwds['entry_points'] = gnomon_declare_plugins('src/package_name')
```

```{warning}
You need to make sure that the path passed to the
`gnomon_declare_plugins` function contains the sources of the package,
or at least all modules defining Gnomon plugins.
```

## Complete plugin module

``` python
import gnomon.core
from gnomon.core import gnomonAbstractImageFilter

from gnomon.utils import corePlugin
from gnomon.utils.decorators import imageInput, imageOutput

import dtkcore

from timagetk.plugins.linear_filtering import linear_filtering

@corePlugin(version="0.1.0", coreversion="0.60.0")
@imageInput(attr="images")
@imageOutput(attr="filtered_images")
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
        self._parameters["sigma"] = dtkcore.d_real("sigma", 1., 0, 10., 2,
            "Standard deviation of the Gaussian kernel")

    def run(self):
        self.filtered_images = {}

        for time in self.images.keys():
            self.filtered_images[time] = {}

            for channel in self.images[time].keys():
                img = self.images[time][channel]
                filtered_img = linear_filtering(img, method='gaussian_smoothing', sigma=self['gaussian_sigma'])
                self.filtered_images[time][channel] = filtered_img
```
