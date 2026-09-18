# Point Cloud Data

![Gnomon Image illustration](../../_static/user_guide/point_cloud_image.png){width=200px class="sd-rounded-1 sd-shadow-sm" align=center}

A Point cloud data structure hold points and their attached properties.

## Default implementation

The package [gnomon_package_tissueimage](../../plugins/packages/gnomon_package_tissueimage) offers an implementation for 
this form as well as a **reader** plugin, a **writer** plugin, a **visualization** plugin and several algorithm plugins.

The default reader of point cloud form is **gnomonPointCloudReaderDataFrame**. It loads a CSV file as 3D point cloud.  
This reader supports one extension `csv`.  

## Workspaces using Point Cloud

### Producers
- [Data Browsing](../workspaces/data_browsing)
- [Point Detection](../workspaces/point_detection)
- [Python Algorithm](../workspaces/python_algorithm)

### Consumers
- [Point Cloud Quantification](../workspaces/point_cloud_quantification)
- [Segmentation](../workspaces/segmentation)
- [Python Algorithm](../workspaces/python_algorithm)
