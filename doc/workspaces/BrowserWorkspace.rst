=================
Browser Workspace
=================

Description
===========

The browser workspace is the place to find and import data in order to implement different functionalities of the Gnomon platform. It enables a user to interactively view a data file and change/set visualisation parameters. In this workspace one can create thumbnails of an imported dataset on the form manager which can then be used with other workspaces.

* **Input**: (Drag and drop) A 3D data file (e.g . image stack) in the formats inr.gz, .tif or  cgi on to the "viewer" part of the Browser

* **Output**: A 3D representation of the image on the viewer, with possibility to interact with it (rotation, zoom, translation).

Outlay of the Browser workspace
===============================

.. image::

    gnomon_browser.png

The workspace comprises of three main  areas:

* 3D viewer area (left)
* A file tree area showing the files contained within the current directory (right side pane)
* Form manager area (top).

A file can be imported to the 3D viewer space by dragging and dropping a file from the file tree on the right to the viewer area on the left pane. The visualisation parameters can be set in the viewer menu by clicking on the "Image" item on top left of the viewer area. This opens up a panel with different visualization parameters. After setting the parameters (values for color channels, alpha) , clicking “Render” will apply the desired visualisation settings on the imported file in the viewer area. Clicking "Clear" will erase the imported file from the viewer area.

Clicking the “XYZ axes” item on the viewer menu (top left) will enable setting the view of the imported object to one of the XY, YZ or ZX planes. Clicking the "cube" item beside this will bring back the 3D visualization.

Clicking the “question mark” item on top right of the viewer area shows a list of keyboard shortcuts for user interaction with the imported dataset. These include shortcuts for zooming in and out, switching to surface or wireframe rendering, resetting camera focus, rotation and translation of the imported object. The “Up arrow” button alongside this item may be clicked, which would result in a thumbnail being created in the form manager at the top of the main window with the image as it is currently rendered in the viewer.


Example of use
==============

**Importing a file and changing visualisation settings**  Select a file name in the file tree on the right. Drag and drop the file to the viewer area on the on the left. In the viewer menu bar on the left click on the "Image” item. A panel with the visualization parameters opens. Alter the parameter values  (e.g set Range : (50 ,200)) and click “Render”. The representation on the viewer area is updated with the new range settings. The viewer menu can be toggled back by clicking the “<“ button on bottom left.

**Switch between 2D and 3D views**  After importing a file, click the square button on top left of the viewer menu. With this the representation changes to to a 2D image slice view with a slider to the left. To view the XZ plane, click the “XZ” button and move the slider up/down to see the updates in the image slice with each slider position. To bring back the 3D representation, click on the “cube” button next to the square button.

**Creating thumbnail in form manager** After importing a file and setting visualisation parameters, press the UP arrow button on the top right of the viewer. This creates a thumbnail in the form manager at the top of the main window with the image as it is currently rendered in the viewer.

Related workspaces
==========================

**Downstream of the Browser workspace** To implement functions of other Gnomon workspaces, one must first import a file into the browser workspace and create its thumbnail in the form manager. Then the file can be used in other workspaces by dragging and dropping the thumbnail into respective workspaces.