# Plugins

Plugins are the meat of Gnomon. 
This is where the different forms, visualization and algorithms are implemented.

## Available packages

| Package                         | Description                                                                                                        |                                             Source                                              |
|---------------------------------|--------------------------------------------------------------------------------------------------------------------|:-----------------------------------------------------------------------------------------------:|
| gnomon-package-tissueimage      | Visualization and manipulation of 3D intensity images as well as Segmentation images                               |   [{fab}`gitlab`](https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimage)    |
| gnomon-package-imageenhancement | Tools for binary image creation and visualization as well as level-set segmentation and anisotropic filtering      | [{fab}`gitlab`](https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-imageenhancement) |
| gnomon-package-data             | Implements the forms `gnomonDataDict` and `gnomonDatFrame`                                                         |       [{fab}`gitlab`](https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-data)       |
| gnomon-package-tissueimagemesh  | Tools for manipulating meshes relating to Tissue Images / Segmented Images                                         | [{fab}`gitlab`](https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissueimagemesh)  |
| gnomon-package-tissuemesh       | Visualization and manipulation of meshes. Implements the form `gnomonMesh`                                         |    [{fab}`gitlab`](https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-tissuemesh)    |
| gnomon-package-vtkmesh          | Reading and writing vtk meshes                                                                                     |     [{fab}`gitlab`](https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-vtkmesh)      |
| gnomon-package-treelsystem      | Visualization and manipulation of lsystems and lsystems models (eg. [LPy](https://github.com/openalea/lpy) models) |  [{fab}`gitlab`](https://gitlab.inria.fr/gnomon/gnomon-packages/gnomon-package-treelsystem)     |


## Making a plugin package


:::{toctree}
:maxdepth: 1
:hidden:

packages
:::