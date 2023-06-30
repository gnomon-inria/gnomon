# Mesh

## Description
>A gnomon Mesh is a data structure modelling the geometry of a biological object using smaller discrete elements. It consists in a set of topological elements (typically polygonal or polyhedral) built upon a list of points. The embedding of the points in the 3D space is what defines the resuting geometry of the object.
The default data class implementing Mesh in gnomon is `gnomonMeshDataPropertyTopomesh` using the mesh implementation provided by the [cellcomplex](https://mosaic.gitlabpages.inria.fr/cellcomplex/examples/property_topomesh.html) Python package.

## Default reader plugin
>The default reader for mesh forms is **gnomonMeshReaderPropertyTopomesh** that reads `.ply` files representing 3D meshes written in the (ASCII) [Polygon File Format](http://paulbourke.net/dataformats/ply/).


## Gnomon Mesh Example

![Gnomon Mesh illustration](../_static/mesh.png)

## Plugins which take Mesh as input

>Here is a non exhaustive list of some algorithms which take this form as input.
- meshSmoothingCellcomplex
- isotropicRemeshingCellcomplex

## Plugins which produce Mesh as output
> Here is a non exhaustive list of some algorithms which produce this form as output.
- imageSurfaceMesh

## A Use Case, Smoothing a mesh
```python
from copy import deepcopy

from dtkcore import d_int

import gnomon.core
from gnomon.utils import algorithmPlugin
from gnomon.utils.decorators import meshInput, meshOutput

from cellcomplex.property_topomesh.optimization import property_topomesh_vertices_deformation
from cellcomplex.property_topomesh.analysis import compute_topomesh_property


@algorithmPlugin(version="0.2.0", coreversion="0.71.0")
@meshInput('mesh_in', data_plugin="gnomonMeshDataPropertyTopomesh")
@meshOutput('mesh_out', data_plugin="gnomonMeshDataPropertyTopomesh")
class meshLaplacianSmoothingCellcomplex(gnomon.core.gnomonAbstractMeshFilter):
    """
    Apply an iterative laplacian smoothing to a mesh.

    """

    def __init__(self):
        super().__init__()

        self._parameters = {}
        self._parameters['iterations'] = d_int('Iterations', 10, 1, 100, "The number of steps in the optimization process")

        self.mesh_in = {}
        self.mesh_out = {}

    def run(self):
        self.mesh_out = {}

        for time in self.mesh_in.keys():
            in_topomesh = self.mesh_in[time]
            out_topomesh = deepcopy(in_topomesh)

            omega_forces = 
            if self['method'] == 'laplacian':
                omega_forces['laplacian_smoothing'] = 0.33
            elif self['method'] == 'taubin':
                omega_forces['taubin_smoothing'] = 0.65

            property_topomesh_vertices_deformation(
                out_topomesh,
                omega_forces={'laplacian_smoothing': 0.33},
                iterations=self['iterations']
            )
            
            for degree in [1, 2, 3]:
                compute_topomesh_property(out_topomesh, 'barycenter', degree)
            for degree in [0, 1, 2, 3]:
                positions = out_topomesh.wisp_property('barycenter', degree)
                for k, dim in enumerate(['x', 'y', 'z']):
                    out_topomesh.update_wisp_property('barycenter_'+dim, degree,
                                                      dict(zip(positions.keys(), positions.values()[:, k])))

            self.mesh_out[time] = out_topomesh

```