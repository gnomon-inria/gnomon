# Workspaces

**Workspaces** constitute the elementary bricks in the Gnomon user experience. You are going to chain processing steps to apply to your data by creating new Workspaces and by moving **Forms** from one Workspace to the next.

A Workspace is the graphical counterpart of an **Algorithm**[{fas}`book-open;sd-text-primary fa-2xs`](algorithm-definition), through which the user will be able to:

* Set the Form(s) corresponding to its input
* Tune the values of its **Parameters**
* Run it and access to logs and progress
* Retrieve the Form(s) corresponding to its output

::::::{card}
:class-body: sd-text-center sd-badge sd-text-danger sd-outline-danger

Workspace

:::::{grid}
:padding: 1
:gutter: 2

::::{grid-item-card}
:columns: 2
:margin: auto
:padding: 0
:class-body: sd-text-center sd-badge sd-text-secondary sd-outline-secondary

{fas}`eye; fa-xl`

Visualization

<br/> <br/>

:::{card}
:margin: auto
:class-body: sd-outline-warning-dark sd-text-warning-dark sd-rounded-1
{fas}`sliders; fa-xl`

Parameters
:::

<br/> <br/> <br/>

::::

::::{grid-item-card}
:columns: 4
:margin: auto
:padding: 0
:class-body: sd-text-center sd-badge  sd-text-secondary-dark sd-outline-secondary-dark

{fas}`display; fa-xl`

Input View

<br/> <br/>

:::{card}
:class-body: sd-outline-success sd-text-success sd-rounded-1
:margin: auto
:width: 50%
{fas}`images; fa-xl`

Input Form(s)
:::

<br/> <br/> <br/>

::::

::::{grid-item-card}
:columns: 4
:margin: auto
:padding: 0
:class-body: sd-text-center sd-badge sd-text-secondary-dark sd-outline-secondary-dark

{fas}`display; fa-xl`

Output View

<br/> <br/>

:::{card}
:margin: auto
:width: 50%
:class-body: sd-outline-success sd-text-success sd-rounded-1
{fas}`images; fa-xl`

Output Form(s)
:::

<br/> <br/> <br/>

::::

::::{grid-item-card}
:columns: 2
:margin: auto
:padding: 0
:class-body: sd-outline-warning sd-text-center sd-badge sd-text-warning

{fas}`gear; fa-xl`

Algorithm

<br/> <br/>

:::{card}
:margin: 0
:class-body: sd-outline-warning-dark sd-text-warning-dark sd-rounded-1
{fas}`sliders; fa-xl`

Parameters
:::

<br/> <br/> <br/>

::::
:::::

::::::

The typical layout of a Workspace consists of input and output Views[{fas}`book-open;sd-text-primary fa-2xs`](view-definition) at the center, rendering the input and output Forms of the Algorithm, with lateral menus to control the Algorithm (right) and the Visualization of the active view (left).

## Available workspaces


:::{list-table}
:widths: 12 10 10 24
:header-rows: 1

*   - Workspace Name
    - Input Forms
    - Output Forms
    - 
*   - [Data Browsing](workspaces/data_browsing)
    - 
    - {bdg-link-success}`Image <forms/image.html>`<br/> {bdg-link-success}`BinaryImage <forms/binary_image.html>` <br/> {bdg-link-success}`CellImage <forms/cell_image.html>`<br/> {bdg-link-success}`Mesh <forms/mesh.html>`<br/> {bdg-link-success}`CellComplex <forms/cellcomplex.html>`
    - ![gnomon_screenshot_segmentation.png](../_static/index/gnomon_screenshot_segmentation.png)
*   - [Preprocessing](workspaces/preprocessing)
    - {bdg-link-success}`Image <forms/image.html>`<br/> {bdg-link-success-line}`BinaryImage <forms/binary_image.html>`
    - {bdg-link-success}`Image <forms/image.html>`
    - ![gnomon_screenshot_segmentation.png](../_static/index/gnomon_screenshot_segmentation.png)
*   - [Binarization](workspaces/binarization)
    - {bdg-link-success}`Image <forms/image.html>`<br/>
    - {bdg-link-success}`BinaryImage <forms/binary_image.html>`
    - ![gnomon_screenshot_segmentation.png](../_static/index/gnomon_screenshot_segmentation.png)
*   - [Segmentation](workspaces/segmentation)
    - {bdg-link-success}`Image <forms/image.html>`
    - {bdg-link-success}`CellImage <forms/cell_image.html>`
    - ![gnomon_screenshot_segmentation.png](../_static/index/gnomon_screenshot_segmentation.png)
*   - [Image Registration](workspaces/image_registration)
    - {bdg-link-success}`Image <forms/image.html>` <br/> {bdg-link-success-line}`DataDict <forms/data_dict.html>`
    - {bdg-link-success}`Image <forms/image.html>` <br/> {bdg-link-success}`DataDict <forms/data_dict.html>`
    - ![gnomon_screenshot_segmentation.png](../_static/index/gnomon_screenshot_segmentation.png)
:::

:::{toctree}
:maxdepth: 1
:hidden:

workspaces/segmentation
:::
