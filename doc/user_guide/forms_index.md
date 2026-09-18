# Forms

In Gnomon **Forms**[{fas}`book-open;sd-text-primary fa-2xs`](form-definition) are the data structures that represent the state of a developing biological system. They are usually stored and moved around as time series. Here you will find the list of the different types of Forms defined by Gnomon.

##  Implementation of Forms

Gnomon interacts with Forms only through their *interface*. Their implementation, meaning how the actual data is stored and served to different plugins is defined in **Data Plugins** which are a special kind of Plugin. Readers and Writers for the different Forms are also defined in plugins.

To find which plugin (and plugin package) you need for the form you want to manipulate, you may look at the documentation page for this Form, or alternatively browse through [the available plugin packages](../plugins/packages/index).

## Types of Forms

:::{list-table}
:widths: 10 30 10
:header-rows: 1

* - Name
  - Description
  - 
* 
  - {bdg-link-success}`Image <forms/image.html>`
  - A 3D image with multiple channels support
  - ![gnomonImage](../_static/user_guide/image.png){.bg-warning w=100px align=center}
* 
  - {bdg-link-success}`CellImage <forms/cell_image.html>`
  - A 3D segmented image. The value of the voxel represents its class
  - ![gnomonCellImage](../_static/user_guide/cell_image.png){.bg-warning w=100px align=center}
* 
  - {bdg-link-success}`BinaryImage <forms/binary_image.html>`
  - A 3D image where the values are either true or false
  - ![gnomonBinaryImage](../_static/user_guide/binary_image.png){.bg-warning w=100px align=center}
* 
  - {bdg-link-success}`DataDict <forms/data_dict.html>`
  - A versatile form which can be used to store any data
  - ![gnomonDataDict](../_static/user_guide/data_dict_image.png){.bg-warning w=100px align=center}
* 
  - {bdg-link-success}`DataFrame <forms/data_frame.html>`
  - Two-dimensional tabular data. This data structure contains labeled axes (rows and columns)
  - ![gnomonDataFrame](../_static/user_guide/data_frame__image.png){.bg-warning w=100px align=center}
* 
  - {bdg-link-success}`LString <forms/lstring.html>`
  - Axial tree. Used to represent the evolution of a branching structure
  - ![gnomonLString](../_static/user_guide/lstring.png){.bg-warning w=100px align=center}
* 
  - {bdg-link-success}`Mesh <forms/mesh.html>`
  - 3D surface or volumetric mesh
  - ![gnomonMesh](../_static/user_guide/mesh.png){.bg-warning w=100px align=center}
* 
  - {bdg-link-success}`Image <forms/point_cloud.html>`
  - Hold points and their properties
  - ![gnomonPointCloud](../_static/user_guide/point_cloud_image.png){.bg-warning w=100px align=center}

:::

:::{toctree}
:maxdepth: 1
:hidden:

forms/binary_image
forms/cell_image
forms/data_dict
forms/data_frame
forms/image
forms/lstring
forms/mesh
forms/point_cloud
:::