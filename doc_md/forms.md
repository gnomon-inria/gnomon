# Forms

:::{toctree}
:maxdepth: 1
:hidden:
form_abstraction
form/binary_image
form/cell_image
form/data_dict
form/data_frame
form/image
form/lstring
form/point_cloud
:::

In Gnomon, **Forms** are archetypes of data structures that are commonly used to represent the state of a developing biological system. Several examples of such Forms are provided by the platform, defining a generic interface of how to interact with various computational representations, independently of their underlying implementation.

## Available Forms

The following Forms interfaces are currently supported by the Gnomon platform:
* [Image](form/image) (multi-channel 3D intensity images)
* [CellImage](form/cellimage) (connected-regions 3D labelled images)
* [BinaryImage](form/binaryimage) ("black or white" 3D binary mask images)
* [PointCloud](form/pointcloud) (3D point clouds with point attributes)
* [LString](form/lstring) (branching structure proceeding from a L-System)
* [DataFrame](form/data_frame) (multi-row data table of named columns)
* [DataDict](form/data_dict) (flexible key-value structure)

## Adding Custom Forms

It is possible to extend the possible forms, yet it currently requires significant additions to the source code of Gnomon, for which we detail the procedure: [](form_abstraction) 