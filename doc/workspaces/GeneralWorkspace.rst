==============================
General Workspace organization
==============================
A workspace makes it possible to apply a series of plugin algorithms to specific input data in a visual and interactive manner.

In general, a workspace consists of three main zones:

#. input zones,
#. output zone,
#. configuration panel.

The input 3D viewer (left) contains the intensity image to be segmented, while the output (rightmost) viewer displays the result of the selected segmentation plugin.  Users can select the plugin from a plugin list appearing in the drop-down menu on the right-hand side configuration panel. A brief description of the plugin appears below together with a set of parameters with default values that control the plugin algorithm. Once the parameters are set to their desired values, users can apply the algorithm to the intensity image shown in the leftmost viewer by clicking on the "Apply" button. The result is displayed on the rightmost viewer (note that depending on the algorithm and the input image, this operation may take a variable amount of time).

Users can select the plugin from a plugin list appearing in the drop-down menu on the right-hand side configuration panel. A brief description of the plugin appears below together with a set of parameters with default values that control the plugin algorithm.

**Importing an input form:**  After opening an intensity image in the Browser Workspace and having exported it to the form manager, users can import it into the left viewer of the Segmentation Workspace by drag-and-drop or through the DOWN arrow button on the image icon.

**Selecting, setting and running a plugin:** After selecting the desired segmentation algorithm on the plugin Panel on the rightmost part of the Workspace, users can set the parameters to specific values or leave them to their default values.

Clicking on the Apply button runs the plugin algorithm on the input image. After completion, the resulting segmented image appears on the right viewer of the workspace.

**Visualizing the result:** Users can then select their preferred visualization option in the visualization menu of the right viewer. For instance, they can change the colormap used to identify each segmented object, or they can toggle on/off the surface mesh visualization option.

When surface meshes are visualized, a Reticle button appears on the left among the possible user-image interaction styles. By activating this style, users can click on each segmented object to obtain information associated with it (such as, for instance, its unique numerical ID).
