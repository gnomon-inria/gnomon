# Image Meshing Workspace

The Image Meshing Workspace allows, from an [Image](../form/image), [Binary Image](../form/binary_image) or [Cell Image](../form/cell_image) form passed as input, to compute a [Mesh form](../form/mesh), where the 2D or 3D elements represents surfaces or volumes of the objects of interest in the image.

Using the input form(s) present in the left view, the Image Meshing plugins with compute a Mesh, generally a 2D surface made of triangular elements, that will be displayed in the right output view. When the plugin operates on a Cell Image, the labels of the cells will generally be added as an attribute on the Mesh elements.

:::{note}
Plugins for this workspace can be installed with the following plugin package(s):
```shell script
gnomon-utils package install gnomon_package_tissueimagemesh
```
:::