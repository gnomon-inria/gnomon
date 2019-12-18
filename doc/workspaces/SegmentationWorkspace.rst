============================
Image Segmentation Workspace
============================

General presentation
====================

This workspace produces segmentations of intensity images using different algorithms. It is particularly adapted to segment 3D images of multicellular tissues at cell resolution.

* **Input**: intensity image (typically coming from microscopy (confocal, light-sheet) data, representing either 2D or 3D samples).
* **Output**: a segmented image, in which each compartment of the original intensity image is identified and labeled with a unique numerical ID.

Outlay of the Image Segmentation workspace
==========================================

.. figure:: Images/SegmentationWorkspace.png
    :align: center
    :scale: 25
    :alt: alternate text
    :figclass: align-center

    Image Segmentation workspace

The workspace consists of three main specific zones:

* input :ref:`3DViewer`,
* output 3D viewer and
* configuration panel.

The input 3D viewer (left) contains the intensity image to be segmented, while the output (rightmost) viewer displays the result of the selected segmentation plugin.
Once the parameters are set to their desired values, users can apply the algorithm to the intensity image shown in the leftmost viewer by clicking on the "Apply" button. The result is displayed on the rightmost viewer (note that depending on the algorithm and the input image, this operation may take a variable amount of time).


Example of use: Watershed segmentation of an intensity image
============================================================

**Prerequisite:** The form manager (LINK) must contain at least one intensity image.

**Step 1: selection of input intensity image**

**Step 5: Overlaying intensity and segmentation images:** The original intensity image from the Form Manager can be drag-and-dropped into the right viewer. As a  result, the intensity and segmented images are overlayed, facilitating the assessment of the quality of segmentation achieved.

Related workspaces
==================

**Upstream of the Image Segmentation workspace:** Three-dimensional intensity images, inputs of the Segmentation Workspace, might  directly come from a file via the **Browser workspace** or might have been produced as outputs of the **Fusion workspace**.

**Downstream of the Image Segmentation workspace:** segmented images, can then be quantitatively analyzed in the **Cell analysis workspace** or further processed in the **Cell reconstruction workspace**.
