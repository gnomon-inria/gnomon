# Segmentation Workspace

The Segmentation Workspace allows to segment an {bdg-link-success-line}`Image <../forms/image.html>` passed as input into a {bdg-link-success-line}`Cell Image <../forms/cell_image.html>`, where cells are represented as image regions carrying identical labels.

:::::{hint}
::::{grid}
:gutter: 1
:margin: 0

:::{grid-item}
:columns: auto  
:child-align: center
You can find a detailed tutorial for the Segmentation Workspace in the
:::

:::{grid-item}
:columns: auto  
:child-align: center
```{button-link} ../example/image_segmentation.html
:color: primary
:outline:
Image Segmentation Scenario
```
:::
::::
:::::

The plugins performing cell segmentation usually consist in two phases: a cell detection and an image segmentation. In some cases, plugins will accept a PointCloud input providing the result of the cell detection. In the resulting CellImage output, there will then be one cell for each point of the input PointCloud form.

:::{note}
Plugins for this workspace can be installed with the following plugin package(s):
```shell script
gnomon-utils package install gnomon_package_tissueimage
```
:::