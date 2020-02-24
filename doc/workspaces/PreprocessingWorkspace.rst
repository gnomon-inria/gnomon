==============================
Image Pre-processing Workspace
==============================

Context of use
==============

Image pre-processing might be necessary as a pretreatment of various image algorithms. For example pre-proceessing might consist of removing noise or enhancing signal intensity in images before operations such as segmentation.

This workspace offers various plugin algorithms to carry out image pre-processing.

* **Input**: intensity image (typically coming from microscopy (confocal, light-sheet) data, representing either 2D or 3D samples).
* **Output**: a preprocessed image.

Outlay of the Preprocessing workspace
=====================================

.. figure:: Images/PreprocessingWorkspace.png
    :align: center
    :scale: 25
    :alt: alternate text
    :figclass: align-center

This workspace has three panels. The left one contains the image to be processed, the right one allows you to choose the pre-processing algorithm to use, and the central panel provides the post-processing rendering (output of the pre-processing algorithm).

Example of use
==============

By default, this workspace is used as follows. Drag and drop an image thumbnail from the Form Manager to the input panel. Then, choose a pre-processing algorithm - for example, `sliceContrastStretch`. Then choose which channel(s) the algorithm will be applied to, calibrate the parameters, and choose which direction to operate. The resulting image can then be manipulated and thumbnailed like a normal image -- changing the color schemes, camera, etc.

Typical follow-ups
==================
