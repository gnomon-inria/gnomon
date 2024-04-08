# Point Cloud Quantification Workspace

The Point Cloud Quantification Workspace allows to compute properties over the points of a [Point Cloud form](../form/point_cloud) and to display them in a 2D interactive figure.

Depending on the chosen plugin, the properties are computed either only on the Point Cloud itself, or using an optional [Image](../form/image) or an optional [Mesh](../form/mesh) as input. In any case, point properties are added to the input Point Cloud, and a [Data Frame form](../form/data_frame) representing the data asociated with each point is returned as an output.