# Image Registration Workspace

The Image Registration Workspace allows to perform the temporal registration of an [Image form](../form/image) series of at least 2 time points.

:::::{hint}
::::{grid}
:gutter: 1
:margin: 0

:::{grid-item}
:columns: auto  
:child-align: center
You can find a detailed tutorial for the Registration Workspace in the
:::

:::{grid-item}
:columns: auto  
:child-align: center
```{button-link} ../example/image_registration.html
:color: primary
:outline:
Image Registration Scenario
```
::: 
::::
:::::

The output is a Image form that corresponds to the transformed Image series where the time points have been registered. The resulting geometrical trasformation is also output as a [Data Dict form](../form/data_dict).

:::{note}
Plugins for this workspace can be installed with the following plugin package(s):
```shell script
gnomon-utils package install gnomon_package_tissueimage
```
:::