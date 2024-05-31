# Data Browsing Workspace

In the Data Browsing Workspace, you can navigate and import data in order to process them through the Gnomon platform. Here you can import local files as **Forms**, and then export them to the **Form Manager** to be used within other workspaces.

:::::{hint}
::::{grid}
:gutter: 1
:margin: 0

:::{grid-item}
:columns: auto  
:child-align: center
You can find a detailed tutorial for the Browsing Workspace in the
:::

:::{grid-item}
:columns: auto  
:child-align: center
```{button-link} ../example/browser.html
:color: primary
:outline:
Tutorial #2 : Load a data file
```
:::
::::
:::::

## Reading a data file 

A single file can be imported by dragging and dropping it either from the file browser (right space) or from your local file system to the 3D viewer (central space). The file will then be read using a **Reader** plugin, chosen based on the file extension. In case several plugins can read the file extension, a dialog will ask you which to use.

:::{note}
Plugins for this workspace can be installed with the following plugin package(s):
```shell script
gnomon-utils package install gnomon_package_tissueimage
gnomon-utils package install gnomon_package_tissuemesh
```
:::
