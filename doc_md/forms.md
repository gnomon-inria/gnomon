# Forms

In Gnomon, **Forms** are archetypes of data structures that are commonly used to represent the state of a developing biological system. Several examples of such Forms are provided by the platform, defining a generic interface of how to interact with various computational representations, independently of their underlying implementation.

## Available Forms

The following Forms interfaces are currently supported by the Gnomon platform:
* [Image](form/image) (multi-channel 3D intensity images)
* [CellImage](form/cell_image) (connected-regions 3D labelled images)
* [BinaryImage](form/binary_image) ("black or white" 3D binary mask images)
* [PointCloud](form/point_cloud) (3D point clouds with point attributes)
* [LString](form/lstring) (branching structure proceeding from a L-System)
* [Mesh](form/mesh) (3D meshes made of polygonal or polyhedral cells)
* [DataFrame](form/data_frame) (multi-row data table of named columns)
* [DataDict](form/data_dict) (flexible key-value structure)

## Adding Custom Forms

It is possible to extend the possible forms, yet it currently requires significant additions to the source code of Gnomon, for which we detail the procedure: [](howto/form_abstraction)


:::{toctree}
:maxdepth: 1
:hidden:
form/binary_image
form/cell_image
form/data_dict
form/data_frame
form/image
form/lstring
form/mesh
form/point_cloud
howto/form_abstraction
:::