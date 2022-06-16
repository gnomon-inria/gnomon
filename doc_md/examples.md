# Examples


## Image Analysis Scenarios

:::{toctree}
:hidden:
:maxdepth: 2

example/image_segmentation
example/image_enhancement
example/image_registration
example/image_python_plugin
:::

These examples provide a variety of scenarios for simple 3D image-related tasks. They detail step by step the interactions you will need to perform, and are a good way to get an overview of the possibilities of the Gnomon platform for image analysis.

:::{warning}
This section is under construction! Some information might be missing.
:::

::::{card-carousel} 3

:::{card}
:class-header: sd-fs-5 sd-font-weight-bold
:link: example/image_segmentation
:link-type: doc
:img-bottom: _static/image_segmentation_example.png

Image Segmentation
^^^
Load a 3D intensity image and extract the cells into labelled regions.
:::

:::{card} 
:class-header: sd-fs-5 sd-font-weight-bold
:link: example/image_enhancement
:link-type: doc
:img-bottom: _static/image_enhancement_example.png

Image Enhancement
^^^
Load a 3D image and enhance the signal to improve its segmentation.
:::

:::{card}
:class-header: sd-fs-5 sd-font-weight-bold
:link: example/image_registration
:link-type: doc
:img-bottom: _static/image_registration_example.png

Image Registration
^^^
Load a series of images and align them using a geometric transformation.
:::

:::{card} 
:class-header: sd-fs-5 sd-font-weight-bold
:link: example/image_python_plugin
:link-type: doc
:img-bottom: _static/image_python_plugin_example.png

Image Python Plugin
^^^
Load an image and directly code the next processing step in Python.
:::

::::

:::{note}
All the data files used in these examples can be found in the [gnomon-data](https://gitlab.inria.fr:gnomon/gnomon-data.git) repository.
:::