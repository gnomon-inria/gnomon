============================
Image Registration Workspace
============================

Context of use
==============

This workspace is dedicated to spatial or temporal registration using two or more intensity images (2D/3D).

This workspace is particularly well-adapted for 4D images (3D + time) where the last time-image is used
as *reference* and other images are iteratively registered on it.

- **input**: two or more intensity images (from microscopy confocal, light-sheet, ...) either 2D, 3D samples.

- **output**: two or more stacked intensity images registered. The number of output images is equaled to the number
of input images.

Outlay of the workspace
=======================

.. figure:: Images/RegistrationWorkspace.png
    :align: center
    :scale: 30
    :alt: alternate text
    :figclass: align-center

    Fig. 1: **Image Registration workspace**: Multi-input :ref:`Viewer` is placed on the left (three images imported),
    output :ref:`Viewer` on the center and the configuration panel on the right. Red area highlights the slider
    allowing to move along the output stacked images (here the second registered image i.e. the top-right
    corner input image registered).

The multi-input :ref:`Viewer` allows to add a custom number of images to register (>= 2 images).
By default three empty containers are available. Additional empty container can be obtained by
clicking on the '+' container. **Containers are ordered from top to bottom and left to right** (i.e. the last
image will be at the bottom-right corner image while the first one will be at the top-left corner).

In the configuration panel, linear (rigid/affine) and non-linear (deformable) transformations are available and can be
estimated using specific image channels (multi-modal images). Notice that the operation may take a long time to compute
if a lot of images need to be proceed and/or if deformable method has been selected.

When the registration is finished the stacked output registered images will appear in the output :ref:`Viewer`. A
slider situated at the bottom of the output image (red area in fig. 1) allows to navigate between the stacked registered
images. Notice that the number of output stacks is equaled to the number of input images. The last output stacked image
is the same as the last input image (because it is the *reference image*). Output stacked images can be saved or re-use
through the up-arrow button.

Example of use: Registration of 3D+t intensity images
=====================================================

**Prerequisite:** The :ref:`FormManager` must contain at least two intensity images.

**Step 1:** Open an **Image Registration workspace**. If more than three images need to be registered add
new containers by clicking on the '+' special container.

**Step 2:** Drag-and-drop in the input :ref:`Viewer` each images **in chronological order** from left to right and
from bottom to top (the first image is at the top-left corner while the last image is at the bottom-right corner).

**Step 3:** At the top of the configuration panel, select the *backwardRegistrationTimagetk* algorithm.

**Step 4:** Select the type of transformation: **rigid**/**affine** for linear and
**deformable** for non-linear. Notice that **deformable** will take a longer time to compute.

**Step 5:** Click on the *Apply* button and wait until an image appears in the output :ref:`Viewer`.

**Step 6:** Save the output stacked images using the up-arrow at the top-left corner of the output :ref:`Viewer`.

Related Workspace
=================

**Upstream of the Registration workspace**: 2D/3D multiple intensity images (different view angles, time-series,...)
might  directly come from a file via the :ref:`BrowserWorkspace` or might have been produced as outputs of the
**Pre-Processing workspace**.

**Downstream of the Registration workspace**: registered images, can then be used in the **Fusion workspace** or
in the :ref:`SegmentationWorkspace`.
