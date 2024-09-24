# Segmentation Workspace

![workspace_segmentation.png](../../_static/user_guide/workspaces/workspace_segmentation.png){width=600px class="sd-rounded-1 sd-shadow-sm" align=center}

::::{admonition} Example
:class: sidebar tip

You can find a detailed tutorial for the Segmentation Workspace in the

:::{button-link} ../example/image_segmentation.html
:color: success
:outline:
Image Segmentation Example
:::
::::

The Segmentation Workspace allows to perform image segmentation by transforming an {bdg-link-success-line}`Image <../forms/image.html>` passed as input into a {bdg-link-success-line}`Cell Image <../forms/cell_image.html>`, where cells are represented as image regions carrying identical labels.

The plugins performing cell segmentation usually consist in two phases: a cell detection and an image segmentation. In some cases, plugins will accept a PointCloud input providing the result of the cell detection. In the resulting CellImage output, there will then be one cell for each point of the input PointCloud form.

:::{note}
Plugins for this workspace can be installed with the following plugin package(s):
```shell script
gnomon-utils package install gnomon_package_tissueimage
```
:::