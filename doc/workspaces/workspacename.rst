=================
Browser Workspace
=================

Description
==============

The Gnomon browser workspace is the place to find, import and open datasets in order to implement different functionalities of the Gnomon platform. It enables a user to view and interactively work with datasets, change visualisation parameters and create thumbnails of an imported dataset for the form manager to be used with other workspaces.

* **Input**: (Drag and drop) A 3D data file (e.g . image stack) in the formats inr.gz, .tif or  cgi on to the "viewer" part of the Browser

* **Output**: A 3D representation of the image on the viewer, with possibility to interact with it (rotation, zoom, translation).

Outlay of the Browser workspace 
=======================

.. image::

    gnomon_browser1.png
    gnomon_browser2.png

The workspace comprises of three main  areas:

* 3D viewer area (left)
* A file tree area showing the files contained within the directory gnomon-data (right side pane)
* Form manager area (top).

A file can be imported to the 3D viewer space by dragging and dropping a file from the file tree on the right. The visualisation parameters can be set in the viewer menu by clicking on the "Image" item on top left of the viewer area. This opens up a panel with different visualization parameters. After setting the parameters, clicking “Render” will apply the desired visualisation settings on the imported file in the viewer area. 

Clicking the “XYZ axes” item on the viewer menu (top left) will enable setting the view of the imported object to one of the XY, YZ or ZX planes. Clicking the "cube" item beside this will bring back the 3D visualization.

Clicking the “question mark” item on top right of the viewer area shows a list of keyboard shortcuts for user interaction with the imported dataset. The “Up arrow” button alongside this item may be clicked, which would result in a thumbnail being created in the form manager at the top of the main window with the image as it is currently rendered in the viewer.




Example of use
==============

Describe the flow of user interactions/operations focusing on the "default" algorithm of the workspace.

Typical follow-ups
==============

Explain how this is linked to the work in other workspaces (by citing their documentation pages).