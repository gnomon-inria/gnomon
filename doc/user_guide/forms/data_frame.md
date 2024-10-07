# Data Frame

![Gnomon Data Frame illustration Image](../../_static/user_guide/data_frame__image.png){width=200px class="sd-rounded-1 sd-shadow-sm" align=center}

A Data Frame represents a two-dimensional tabular data. This data structure contains labeled axes (rows and columns). 

## Default implementation
The package [gnomon_package_data](../../plugins/packages/gnomon_package_data) implements this form as well plugins for
reading, writing and visualization for this form.

The reader loads CSV files with named columns.  
This reader supports one extension `csv`.  

## Workspaces using Data Frame

### Producers
- [Data Browsing](../workspaces/data_browsing)
- [Cell Image Quantification](../workspaces/cell_image_quantification)
- [Point Cloud Quantification](../workspaces/point_cloud_quantification)
- [Python Algorithm](../workspaces/python_algorithm)

### Consumers
- [Python Algorithm](../workspaces/python_algorithm)
