.. _SegmentationWorkspace:

============================
Image Segmentation Workspace
============================


General presentation
====================

This workspace produces segmentations of intensity images using different algorithms.
It is particularly adapted to segment 3D images of multicellular tissues at cell resolution.

* **Input**: intensity image (typically coming from microscopy (confocal, light-sheet) data, representing either 2D or 3D samples).
* **Output**: a segmented image, in which each compartment of the original intensity image is identified and labeled with a unique label (numerical Id).

Outlay of the Image Segmentation workspace
==========================================

.. figure:: Images/SegmentationWorkspace.png
    :align: center
    :scale: 25
    :alt: alternate text
    :figclass: align-center

    Image Segmentation workspace

The workspace consists of three main specific zones:

* input :ref:`Viewer`,
* output :ref:`Viewer`,
* configuration panel.

The input :ref:`Viewer` (left) contains the intensity image to be segmented, while the output (rightmost)
viewer displays the result of the selected segmentation plugin.
Once the parameters are set to their desired values, users can apply the algorithm to the intensity image
shown in the leftmost viewer by clicking on the "Apply" button. The result is displayed on the rightmost
viewer (note that depending on the algorithm and the input image, this operation may take a variable amount of time).


Example of use: Watershed segmentation of an intensity image
============================================================

**Prerequisite:** The :ref:`FormManager` must contain at least one intensity image.

**Step 1:** Open an Image Segmentation workspace.

**Step 2:** Drag and drop one intensity image from the form manager to the
input :ref:`Viewer`.

**Step 3:** Select the *seededWatershedSegmentation* algorithm at the top
of the configuration panel.

**Step 4:** Click on the *Apply* button at the bottom of the configuration
panel.

**Step 5:** Drag and drop the input image into the output viewer. The intensity and
segmented images will overlay, facilitating the assessment of the quality of segmentation achieved.

**Step 6:** (Optional) You can then play with the h-min parameter in order to modify
the number of seeds. The smaller h-min the higher the number of seeds (and vice-versa).

**Step 7:** The segmented image can be exported to the form manager using the up-arrow at the top-right corner of the output 3d viewer.

Related workspaces
==================

**Upstream of the Image Segmentation workspace:** Three-dimensional intensity images, inputs of the Segmentation Workspace, might  directly come from a file via the **Form Browser Workspace** or might have been produced as outputs of the **Image Fusion Workspace** or the **Image Preprocessing Workspace**.

**Downstream of the Image Segmentation workspace:** segmented images, can then be quantitatively analyzed in the **Cell Image Analysis Workspace** or further processed in the **Cell Image Meshing Workspace**.
