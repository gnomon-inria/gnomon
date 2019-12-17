=========================
Cell Detection Workspace
=========================

General presentation
=========================

This workspace performs two types of cell detection: nuclei detection and seed detection of each cell of a 3D 
tissue image. 

* **Input**: 3D tissue image. For the nucliei detection, the image should be in the form of 3 colour channels,
whereas for seed detection, the image should be in grayscale and the membranes in the image should be marked clearly. 
* **Output**: Detected nuclei or seeds in point cloud format.

Outlay of the Cell Detection workspace
======================================

.. figure:: Images/CellDetectionWorkspace.png
    :align: center
    :scale: 25
    :alt: alternate text
    :figclass: align-center

The workspace consists of three main specific zones:

* input 3D viewer,
* output 3D viewer and
* configuration panel.

The input 3D viewer (left) contains the 3D tissue image, while the output (rightmost) viewer displays the result of the selected cell detection plugin. Users can select the plugin from a plugin list appearing in the drop-down menu on the right-hand side configuration panel. A brief description of the plugin appears below together with a set of parameters with default values that control the plugin algorithm. Once the parameters are set to their desired values, users can apply the algorithm to the intensity image shown in the leftmost viewer by clicking on the "Apply" button. The result is displayed on the rightmost viewer (note that depending on the algorithm and the input image, this operation may take a variable amount of time).

Each 3D viewer is equipped with two user bars:

* a vertical bar for visualization options on the left and
* a horizontal bar on top.

The visualization options are accessible in a menu from the vertical bar on the left of each figure. There, users can set the colormap used to display each image, or switch between the different ways of interacting with each image. For detected nuclei
and/or seeds in point cloud format, users can choose the opacity, colormap, zooming and other properties to visualize the
cloud of points. 

On top of each viewer, the horizontal bars provides icons to switch between a full 3D and a 2D-cut view of each image (for three-dimensional data). Also, the visualization of the input and output images can be synchronized by activating the two lock icons in each viewer. In this way, rotations, translations and zooms applied to the image in one viewer is automatically reflected on the image in the other viewer. In the same bar, users find a help button and the option to export each image to the form manager through the UP arrow button.

Example of use
==============

**Importing a 3D tissue image:** After opening a 3D tissue image in the Browser Workspace and having exported it to the form manager, users can import it into the left viewer of the Cell Detection Workspace by drag-and-drop or through the DOWN arrow button on the image icon.

**Selecting, setting and running a plugin:** After selecting the desired cell detection algorithm on the plugin Panel on the rightmost part of the Workspace, users can set the parameters to specific values or leave them to their default values.

Clicking on the Apply button runs the plugin algorithm on the input image. After completion, the resulting point cloud appears on the right viewer of the workspace.

**Visualizing the result:** Users can then select their preferred visualization option in the visualization menu of the right viewer. For instance, they can change the colormap to identify each point in the cloud, change the opacity, change the axis orientation view, etc. 

**Overlaying input 3D image and detected cell point cloud:** The original 3D tissue image from the Form Manager can be drag-and-dropped into the right viewer. As a  result, the 3D tissue image and the detected cell point cloud are overlayed, facilitating the assessment of the quality of detection achieved.