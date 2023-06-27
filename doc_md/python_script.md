# Scripting a Gnomon pipeline in Python

The Python API of `gnomon` makes it possible to create, configure and run gnomon plugins natively in a Python script.

## Loading plugin groups and creating plugins 

To be discovered by the plugin machinery, the plugins implementing a given gnomon abstraction must be loaded. The loading takes place by "group", which corresponds to a given plugin factory providing all the plugins implementing the related abstraction.

For instance, to instantiate an image reader plugin, the following lines are required:

```python
import gnomon.core
from gnomon.utils.gnomonPlugin import load_plugin_group

load_plugin_group('imageReader')
reader = gnomon.core.imageReader_pluginFactory().create("imageReaderTimagetk")
```

## Configuring plugins

The object returned by the `create` method of the plugin factory is an instance of the plugin class, on which it is possible to call all the methods defined by the abstraction API. Typically, we will have to set the different inputs of an algorithm plugin, and the values of its parameters, or the path of an i/o plugin.

For instance, to perform an image segmentation, we need to instantiate a dedicated plugin, and to connect it to the output of an image reader, before setting its parameters.

:::{note}
The `refreshParameters` method must be called after the inputs are set for the plugin parameters to have the correct name and definition domain!
:::

```python
load_plugin_group('imagecellImageFromImageReader')

segmentation = gnomon.core.cellImageFromImage_pluginFactory().create("autoSeededWatershedSegmentation")
segmentation.setInput(reader.image())
segmentation.refreshParameters()
segmentation.setParameter("h_min", 2)
```

## Running a pipeline

Each algorithm plugin can then be executed by calling the `run` method, which will compute a value for all the outputs defined by their abstraction API.

By chaining processing steps this way, it is possible to create a complete processing pipeline manually. For instance a simple segmentation pipeline that reads an image, segments it and saves the result in a new file:

```python
import gnomon.core

from gnomon.utils.gnomonPlugin import load_plugin_group

load_plugin_group('imageReader')
load_plugin_group('cellImageFromImage')
load_plugin_group('cellImageWriter')

reader = gnomon.core.imageReader_pluginFactory().create("imageReaderTimagetk")
reader.setPath("gnomon-data/p58-t0_imgFus_down_interp_2x.inr.gz")
reader.run()

segmentation = gnomon.core.cellImageFromImage_pluginFactory().create("autoSeededWatershedSegmentation")
segmentation.setInput(reader.image())
segmentation.refreshParameters()
segmentation.setParameter("h_min", 2)
segmentation.run()

writer = gnomon.core.cellImageWriter_pluginFactory().create("cellImageWriterTissueImage")
writer.setCellImage(segmentation.output())
writer.setPath("gnomon-data/p58-t0_imgFus_down_interp_2x_seg.tif")
writer.run()
```

## Visualization plugins

Unlike algorithm plugins, visualization plugins require not only data on which to perform their computations, but also a view in which to display the visual representation they have generated. The gnomon main application uses specific types of view that can not be directly instatiated in Python.

Instead, we provide simpler versions of 3D (`vtk`) and 2D (`matplotlib`) views that can be passed to the corresponding visualization plugins, so that they behave just as they would do in the main application.

For instance to visualize an image:

```python
import gnomon.visualization

from gnomon.utils.views import gnomonLightVtkView, setView

load_plugin_group('imageVtkVisualization')

view = gnomonLightVtkView()

image_visu = gnomon.visualization.imageVtkVisualization_pluginFactory().create("imageChannelHistogram")
setView(image_visu, view)
image_visu.setImage(reader.image())
image_visu.setParameter("opacity", 0.5)
image_visu.update()

view.show()
```

:::{note}
It is necessary to use the `setView` function that will, in addition of passing the view object to the visualization plugin, make sure that the view object is seen by the plugi as a native view from the gnomon application.
:::

## Full Example

```python
import gnomon.core
import gnomon.visualization

from gnomon.utils.gnomonPlugin import load_plugin_group
from gnomon.utils.views import gnomonLightVtkView, setView

load_plugin_group('imageReader')
load_plugin_group('cellImageFromImage')
load_plugin_group('cellImageWriter')
load_plugin_group('imageVtkVisualization')
load_plugin_group('cellImageVtkVisualization')

reader = gnomon.core.imageReader_pluginFactory().create("imageReaderTimagetk")
reader.setPath("gnomon-data/p58-t0_imgFus_down_interp_2x.inr.gz")
reader.run()

segmentation = gnomon.core.cellImageFromImage_pluginFactory().create("autoSeededWatershedSegmentation")
segmentation.setInput(reader.image())
segmentation.refreshParameters()
segmentation.setParameter("h_min", 2)
segmentation.run()

writer = gnomon.core.cellImageWriter_pluginFactory().create("cellImageWriterTissueImage")
writer.setCellImage(segmentation.output())
writer.setPath("gnomon-data/p58-t0_imgFus_down_interp_2x_seg.tif")
writer.run()

view = gnomonLightVtkView()

image_visu = gnomon.visualization.imageVtkVisualization_pluginFactory().create("imageChannelHistogram")
setView(image_visu, view)
image_visu.setImage(reader.image())
image_visu.setParameter("opacity", 0.5)
image_visu.update()

cellimage_visu = gnomon.visualization.cellImageVtkVisualization_pluginFactory().create("cellImageVtkVisualizationMarchingCubes")
setView(cellimage_visu, view)
cellimage_visu.setCellImage(segmentation.output())
cellimage_visu.setParameter("resolution", 0.5)
cellimage_visu.setParameter("size_factor", 0.9)
cellimage_visu.update()

view.setCameraXY(flip=True)
view.saveScreenshot("gnomon-data/p58-t0_imgFus_down_interp_2x_segmentation.png")

view.show()

```
