# Image Enhancement Scenario

:::{warning}
This section is under construction! Some information might be missing.
:::


## I Improving image segmentation with image enhancement methods

 1. **Browser**
      - Load image YR01_t40_crop.inr.gz
 2. **Segmentation**
      - algo *seededWatershedSegmentation*
      - h_min -> 2
      - background -> 1
      - volume_threshold -> ~1000
      - Apply
      - Show segmentation defects (hole on the surface)
  3. **Preprocess**
      - algo boundaryEdgeEnhancement
      - Edge Intensity: max
      - Threshold: 15
      - on image
  4. **Segmentation**
      - algo seededWatershedSegmentation
      - h_min -> 2
      - background -> 1
      - volume_threshold -> ~1000
      - Apply
      - Show segmentation defects


## II Improving image segmentation With *imageEnhancement* package:

  5. **Preprocess**
      - algo *anisotropic3dImageEnhancement*
      - iterations -> 15
      - tensor -> 10
  6. **Binarization**
      - algo *lsmContour*
      - stop criterion: 0.005
      - smoothing OFF
      - thresholds 0-8
  7. **Preprocess**
      - algo *edgeEnhancementBinaryimage*
      - on ani image without binary mask
      - drop binary mask and apply to show difference
  8. **Segmentation**
      - algo seededWatershedSegmentation
      - h_min -> 2
      - background -> 1
      - volume_threshold -> ~1000
      - Apply
      - Show segmentation enhancement

**Videos**

<table>
<tr> <th> First Scenario </th> <th> Second Scenario </th> </tr>
<tr>
  <td> <iframe width="560" height="315" src="http://www.youtube.com/embed/WZ574C1sqNU?rel=0" frameborder="0" allowfullscreen></iframe> </td>
  <td> <iframe width="560" height="315" src="http://www.youtube.com/embed/s7oJI8gqcpo?rel=0" frameborder="0" allowfullscreen></iframe> </td>
</tr>
</table>