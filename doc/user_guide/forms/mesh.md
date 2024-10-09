# Mesh

![Gnomon Mesh illustration](../../_static/user_guide/mesh.png){width=200px class="sd-rounded-1 sd-shadow-sm" align=center}


A gnomon Mesh is a data structure consisting in a set of topological elements (typically polygonal or polyhedral) built upon a list of points. 
The embedding of the points in the 3D space is what defines the resulting geometry of the object.
A gnomon Mesh can be either a surface mesh or a volumetric mesh.

## Default implementation

An implementation for gnomon Mesh can be found in the package [gnomon_package_tissuemesh](../../plugins/packages/gnomon_package_tissuemesh).
This package also provides a **reader** plugin, a **writer** plugin and two **visualization** plugins for this form, 
as well as several algorithm plugins relating to meshes.

The default reader for mesh forms is **gnomonMeshReaderPropertyTopomesh** that reads `.ply` files representing 3D meshes written in the (ASCII) [Polygon File Format](http://paulbourke.net/dataformats/ply/).

## Workspaces using Mesh

### Producers
- [Data Browsing](../workspaces/data_browsing)
- [Image Meshing](../workspaces/image_meshing)
- [Python Algorithm](../workspaces/python_algorithm)

### Consumers
- [Mesh Processing](../workspaces/mesh_processing)
- [Python Algorithm](../workspaces/python_algorithm)
