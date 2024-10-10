# Creating an Algorithm Plugin in Python

Gnomon has an extensible architecture, meaning that you can provide your own implementations of Algorithms, and interoperate them seamlessly with the rest of the platform components. In this page we provide a step-by-step guide to easily create a Python Plugin to implement an Algorithm.

:::{important}
This guide describes how to define a Plugin implementing **an existing Algorithm abstraction** and using only **existing Form Data Plugins** for its implementation.

If this is not your case you should probably have a look at [how to define a new Form Data Plugin](new_form_data_plugin) or [how to add a new Algorithm abstraction](new_algorithm_abstraction) first.
:::

## Image Filter Plugin

In the following example, we will create a Plugin class that performs a Gaussian smoothing of a multichannel Image Form. It is related to the `gnomonAbstractImageFilter` Algorithm abstraction that encompasses all algorithms that input an image Form and output another image Form, transformed by a filter of some kind. It will therefore appear in the [Image Preprocessing Workspace](../../user_guide/workspaces/image_preprocessing).

To represent the Image Data, our Plugin will rely on the `gnomonImageDataMultiChannelImage` Data Plugin from the [`gnomon_package_tissueimage`](../packages/gnomon_package_tissueimage) package, that manipulates an Image through the `MultiChannelImage` class of the [`timagetk` Python 3D image processing library](https://mosaic.gitlabpages.inria.fr/timagetk).

## Create a Python module for the Plugin

First, let's create the module in the algorithm section of the package:
```
linearFilterTimagetk.py
```

You can have a look at [the typical architecture of a Plugin Package](plugin-package-architecture) to know where to put the module file.

### Create the Plugin class

In the `linearFilterTimagetk.py`, we define a Python class inheriting the algorithm abstraction:

```python
from gnomon.core import gnomonAbstractImageFilter

class linearFilterTimagetk(gnomonAbstractImageFilter):

    def __init__(self):
        super().__init__()
```

#### `algorithmPlugin` decorator

Gnomon provides a generic Python class decorator that implements all the necessary methods to register the plugin to the platform, with just one line of code. The available decorators are:
- `formDataPlugin`
- `algorithmPlugin`
- `modelPlugin`
- `visualizationPlugin`

In this example we will use `algorithmPlugin` because the Plugin we want to define inherits an Algorithm abstraction.

:::{dropdown} `algorithmPlugin` decorator API
```{eval-rst}
.. autofunction:: gnomon.utils.gnomonPlugin.algorithmPlugin
```
:::

The `algorithmPlugin` decorator has to be imported from the [`gnomon.utils` package](../api_doc/utils/index.md) and allows to specify the version of the Plugin (that will be registered when the Plugin is used in a Pipeline) but also the version of the core platform with which it is compatible. This is also where you can give it a more user-friendly name, that will be displayed in the application:

```python
import gnomon.core
from gnomon.core import gnomonAbstractImageFilter

from gnomon.utils import algorithmPlugin

@algorithmPlugin(version="0.1.0", coreversion="1.0.2", name="Linear Filter")
class linearFilterTimagetk(gnomonAbstractImageFilter):
```

### Define the input and output Forms of the plugin

The `gnomon.utils` package provides decorators that allow to map the **attributes** of the class that represent the Forms (in the example case, Images) to the setter and getter **methods** of the Algorithm abstraction. 

:::{admonition} Form Attributes
:class: tip

Plugin classes must have attributes storing **Forms**, which are temporal series of a given data structure. In Python, they are to be implemented as [dictionaries](https://docs.python.org/3/tutorial/datastructures.html#dictionaries) where keys correspond to time points, and values to instances of the data structure used by the **Data Plugin** to represent the **Form Data** at a given time point.

In the example, the attributes will be dictionaries mapping time points to instances of the `MultiChannelImage` class from the `timagetk` package.
:::

#### `imageInput` and `imageOutput` decorators

The `gnomonAbstractImageFilter` abstraction requires an image as input. Therefore, we use the `imageInput` decorator to make the link between the `images` attribute of the class (that will contain the input images) and the accessor methods from the signature of the abstraction (`setInput` and `input`). We also explicitly state that the Plugin expect a Form implemented with the `gnomonImageDataMultiChannelImage` Data Plugin as input.

Since the Algorithm abstraction returns an image as output, we do the same with the  `imageOutput` decorator to link the `filtered_images` to the output accessor method of the abstraction (`output`), using the same Data Plugin.

:::{dropdown} `imageInput` and `imageOutput` decorators API
```{eval-rst}
.. autofunction:: gnomon.utils.decorators.imageInput
.. autofunction:: gnomon.utils.decorators.imageOutput
```
:::

```python
from gnomon.utils.decorators import imageInput, imageOutput

@algorithmPlugin(version="0.1.0", coreversion="1.0.2", name="Linear Filter")
@imageInput(attr='images', data_plugin="gnomonImageDataMultiChannelImage")
@imageOutput(attr='filtered_images', data_plugin="gnomonImageDataMultiChannelImage")
class linearFilterTimagetk(gnomonAbstractImageFilter):

    def __init__(self):
        super().__init__()

        self.images = {}
        self.filtered_images = {}
```

### Define the Parameters of the Algorithm

Plugins may declare any number of **Parameters** to give the user ways to tune the execution of the Algorithm. To create ¨arameters that will be accessible from the user interface, **Gnomon** uses the [parameter classes provided by the **dtkcore** Python module](../../api_doc/dtk_parameters).

In our example case, we define a single parameter that corresponds to the sigma of the Gaussian kernel used for the filtering of the image. As it is a real-valued parameter, we use the `d_real` class from the `dtkcore` module to define the parameter:

:::{dropown} `dtkcore.d_real` constructor API
```{py:class} d_real(label: str, default_value: float, min: float, max: float, decimals: int, doc: str)

```
:::
```python
dtkcore.d_real(
    "sigma", 1., 0, 10., 2, 
    "Standard deviation of the Gaussian kernel"
)
```

Parameters have to be store as a dictionary attribute `_parameters` of the Plugin class for which keys are the names of the parameter. This dictionary must be filled at in the `__init__` constructor of the class.

```python
import gnomon.core
from gnomon.core import gnomonAbstractImageFilter

from gnomon.utils import algorithmPlugin
from gnomon.utils.decorators import imageInput, imageOutput

import dtkcore

@algorithmPlugin(version="0.1.0", coreversion="1.0.2")
@imageInput("images")
@imageOutput("filtered_images")
class linearFilterTimagetk(gnomonAbstractImageFilter):

    def __init__(self):
        super().__init__()

        self.images = {}
        self.filtered_images = {}

        self._parameters = {}
        self._parameters["sigma"] = dtkcore.d_real(
            "sigma", 1., 0, 10., 2,
            "Standard deviation of the Gaussian kernel"
        )
```

The `algorithmPlugin` will make sure that all the Parameters defined
this way are correctly exposed by the Plugin, and will therefore be mapped to a graphical rendering in the Gnomon interface by defining. It does so through the `gnomonParametric` decorator that defines the right functions, and also provides shortened access to the parameter values as `self['parameter_name']`.

## Implement the Plugin

Defining the input and output Forms and the Parameters is really what allows to make the connection between the Gnomon user interface and the Plugin. Now the only missing part is to make it actually compute an output from its input.

### Write the `run` method

The `run` method is where the actual code of the plugin is written, and it generally has always the same structure:
- set the outputs to empty dictionaries (time series)
- iterate over the time points of the inputs
- get the input data structures at the current time
- compute the outputs from the inputs (actual algorithm)
- fill the outputs data structures for the current time

:::{note}
The `run` method does not return anything, it simply updates the values of the output data structures based on the content of the input data structures.
:::

#### Algorithm Code

In the example case, the algorithm is implemented in a function of the `timagetk` Python package that we therefore need to import beforehand.
The function is called to compute the Gaussian smoothing of each image channel at each time point of the `images` input dictionary, and fill the output dictionary `filtered_images` with the results.

```python
from timagetk.plugins.linear_filtering import linear_filtering
from timagetk import MultiChannelimage
```

```python
def run(self):
    self.filtered_images = {}

    for time in self.images.keys():
        filtered_image = {}

        for channel in self.images[time].keys():
            img = self.images[time][channel]
            filtered_img = linear_filtering(
                img,
                method='gaussian_smoothing',
                sigma=self['gaussian_sigma']
            )
            filtered_image[channel] = filtered_img

        self.filtered_images[time] = MultiChannelImage(self.filtered_images[time])
```

#### Parameter values

Within the code of the `run` function, the parameters will be used to perform the computations and can be accessed easily thanks to the `gnomonParametric` decorator. Here for instance, the value of the `gaussian_sigma` parameter (that might have been set manually by the user using a graphical interface) is passed to the function as `self['gaussian_sigma']`.

### Adding progress values

Now that we have a functioning plugin we might want to give some information on the progress of the computation back to Gnomon.  To achieve that, Gnomon provides three pre-implemented methods accessible to every plugin: 
* `set_max_progress(self, v: int)` to define the total length of the progress bar
* `increment_progress(self, increase: int = 1)` to update the progress bar
* `set_progress_message(self, message: str)` to provide a short message detailing what computation is happening

First, we need to call `self.increment_progress()` and `set_progress_message(message)` every so often inside `run()`. Then, for this to work properly, we must set max progress to the number of expected calls to `increment_progress`.

For instance:

```python
def run(self):
    self.set_max_progress(sum(len(img) for img in self.images.values))
    self.filtered_images = {}

    for time in self.images.keys():
        filtered_image = {}

        for channel in self.images[time].keys():
            img = self.images[time][channel]
            self.set_progress_message(f"T {time} - channel {channel} : applying filter")
            filtered_img = linear_filtering(
                img,
                method='gaussian_smoothing',
                sigma=self['gaussian_sigma']
            )
            self.increment_progress()
            filtered_image[channel] = filtered_img

        self.filtered_images[time] = MultiChannelImage(filtered_image)
```

The maximum progress here is the sum of the number of channels of each image in the time series.

:::{note}
The `increment_progress` serves another additional purpose. It gives a break point to pause or stop the process when requested so by Gnomon.
:::

## Provide a documentation for the plugin

The Gnomon application can display some information about your Plugin when the user selects it, if you provide the necessary documentation.

### Fill in the class docstring

To find that information, Gnomon will use the documentation string (docstring) of your Plugin class. It should follow the [NumPy style for Python docstrings](https://numpydoc.readthedocs.io/en/latest/format.html) and contain at least:
- A short description on the first line (less than 80 characters)
- A brief description paragraph detailing the algorithm and its
  parameters

```python
class linearFilterTimagetk(gnomonAbstractImageFilter):
"""Compute the Gaussian smoothing of an image.

The algorithm performs a filtering of all the channels of a 3D image by an
isotropic Gaussian kernel of standard deviation equal to the value of the
gaussian_sigma parameter.
"""
```

## Enable the dynamical discovery of the Plugin

To be dynamically discovered by the Gnomon platform, the Plugin class should be referenced in the **entry points** of your Python interpreter. To do so, Gnomon offers a function that introspects a package looking for Gnomon plugins, and include them in a way that they will be found by the platform. To enable the discovery of your Plugin, we recommend that you include it in a Plugin Package and [add it as on entry point to install with the package](package-entry-points).

:::{dropdown} Complete code of the Plugin module

```python
import gnomon.core
from gnomon.core import gnomonAbstractImageFilter

from gnomon.utils import algorithmPlugin
from gnomon.utils.decorators import imageInput, imageOutput

import dtkcore

from timagetk.plugins.linear_filtering import linear_filtering
from timagetk import MultiChannelimage

@algorithmPlugin(version="0.1.0", coreversion="1.0.2")
@imageInput(attr="images", data_plugin="gnomonImageDataMultiChannelImage")
@imageOutput(attr="filtered_images", data_plugin="gnomonImageDataMultiChannelImage")
class linearFilterTimagetk(gnomonAbstractImageFilter):
    """Compute the Gaussian smoothing of an image.

    The algorithm performs a filtering of all the channels of a 3D image by an
    isotropic Gaussian kernel of standard deviation equal to the value of the
    gaussian_sigma parameter.
    """

    def __init__(self):
        super().__init__()

        self.images = {}
        self.filtered_images = {}

        self._parameters = {}
        self._parameters["sigma"] = dtkcore.d_real(
            "sigma", 1., 0, 10., 2,
            "Standard deviation of the Gaussian kernel"
        )

    def run(self):
        self.set_max_progress(sum(len(img) for img in self.images.values))
        self.filtered_images = {}

        for time in self.images.keys():
            filtered_image = {}

            for channel in self.images[time].keys():
                img = self.images[time][channel]
                filtered_img = linear_filtering(
                    img, 
                    method='gaussian_smoothing', 
                    sigma=self['gaussian_sigma']
                )
                self.increment_progress()
                filtered_image[channel] = filtered_img

            self.filtered_images[time] = MultiChannelImage(filtered_image)
```
::: 