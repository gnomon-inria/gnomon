# Cell Image

![Gnomon Image illustration](../../_static/user_guide/cell_image.png){width=200px class="sd-rounded-1 sd-shadow-sm" align=center}

A gnomon Cell Image is a data structure which represents a segmented image. It is a 3D raster image where the values 
correspond to the class of a segment, in other words, each voxel of a cell has the same value which is not shared with
other cells.

## Properties
The following properties are accessible for Cell Images

- Cell count
- Cell ids
- Cell properties
- Adjacent cells
- Wall ids
- Wall properties
- Wall cell ids
 

## Default implementation
This form is implemented in the package [gnomon_package_tissueimage](../../plugins/packages/gnomon_package_tissueimage.md).
This package also contains a **reader**, a **writer** and a **visualization** plugin for Cell Image.
Extensions supported by this reader are: `.tif, .inr.gz, .inr`.

Multiple algorithms using Cell Images are also provided in this package

## Workspaces using Cell Images

### Producers
- [Data Browsing](../workspaces/data_browsing)
- [Segmentation](../workspaces/segmentation)
- [Cell Image Filter](../workspaces/cell_image_filter)
- [Cell Image Tracking](../workspaces/cell_image_tracking)
- [MorphoNet](../workspaces/morphonet)
- [Python Algorithm](../workspaces/python_algorithm)

### Consumers
- [Cell Image Filter](../workspaces/cell_image_filter)
- [Cell Image Quantification](../workspaces/cell_image_quantification)
- [Cell Image Tracking](../workspaces/cell_image_tracking)
- [Image Meshing](../workspaces/iamge_meshing)
- [Mesh Processing](../workspaces/mesh_processing)
- [MorphoNet](../workspaces/morphonet)
- [Python Algorithm](../workspaces/python_algorithm)
