# Cell Image Quantification Workspace

The Cell Image Quantification Workspace allows to compute properties over the cells of a [Cell Image form](../form/cell_image) and to display them in a 2D interactive figure.

Depending on the chosen plugin, the properties are computed either only on the Cell Image itself, or using an optional [Image](../form/image) or an optional [Mesh](../form/mesh) as input. In any case, cell properties are added to the input Cell Image, and a [Data Frame form](../form/data_frame) representing the data asociated with each cell is returned as an output.