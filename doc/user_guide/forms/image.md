# Image

![Gnomon Image illustration](../../_static/user_guide/image.png){width=200px class="sd-rounded-1 sd-shadow-sm" align=center}

In Gnomon, an Image form is a data structure representing a 3D multichannel intensity image.

## Properties
The following properties are accessible for Images

- Number of channels
- Dimensions
- Voxel Size

## Default implementation
This form is implemented in the package [gnomon_package_tissueimage](../../plugins/packages/gnomon_package_tissueimage).
In this package a **reader**, a **writer** and several **visualization** plugins for Images can be found.

The default reader of image form is **imageReaderTimagetk**. It can read 3D microscopy intensity image files.
Extensions supported by this reader are: `inr, inr.gz, mha, .mha.gz, tif, tiff, czi, lsm`.


## Workspaces using Images

### Producers
- [Data Browsing](../workspaces/data_browsing)
- [Image Preprocessing](../workspaces/image_preprocessing)
- [Image Registration](../workspaces/image_registration)
- [Python Algorithm](../workspaces/python_algorithm)

### Consumers
- [Binarization](../workspaces/binarization)
- [Cell Image Quantification](../workspaces/cell_image_quantification)
- [Cell Image Tracking](../workspaces/cell_image_tracking)
- [Image Meshing](../workspaces/image_meshing)
- [Point Cloud Quantification](../workspaces/point_cloud_quantification)
- [Point Detection](../workspaces/point_detection)
- [Image Preprocessing](../workspaces/image_preprocessing)
- [Image Registration](../workspaces/image_registration)
- [Segmentation](../workspaces/segmentation)
- [Python Algorithm](../workspaces/python_algorithm)