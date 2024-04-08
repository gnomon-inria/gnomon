# Point Detection Workspace

The Point Detection Workspace allows, from an [Image form](../form/image) passed as input, to extract a [Point Cloud form](../form/point_cloud), where objects in the image are represented by 3D point coordinates.

The plugins performing point detection generally apply on a particular channel of the input image, which has to be set as a parameter in case of a multichannel image. In the output PointCloud, each point corresponds to one detected object, and its 3D position typically marks its center.