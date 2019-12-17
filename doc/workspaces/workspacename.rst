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
* A file tree space showing the files contained within the directory gnomon-data (right side pane)
* Form manager space (top).

A file can be imported to the 3D viewer space by dragging and dropping a file from the file tree on the right. The visualisation parameters can be set in the viewer menu by clicking on the "Image" item on top left of the viewer area. This opens up a panel with different visualization parameters. After setting the parameters, clicking “Render” will apply the desired visualisation settings on the imported file in the viewer area. Clicking the “XYZ axes” item on the viewer menu will enable setting the view of the imported object to one of the XY, YZ or ZX planes.


Example of use
==============

Describe the flow of user interactions/operations focusing on the "default" algorithm of the workspace.

Typical follow-ups
==============

Explain how this is linked to the work in other workspaces (by citing their documentation pages).