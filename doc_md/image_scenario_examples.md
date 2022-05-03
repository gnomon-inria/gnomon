# Image Scenario Examples

All the data used here can be found in [gnomon-data](https://gitlab.inria.fr:gnomon/gnomon-data.git)

## Image Segmentation Scenario

To test Image Segmentation, here are the steps:

  1. **Browser** Load image p58-t0_imgFus_down_interp_2x.inr.gz
  2. Open a new Workspace **Segmentation**, choose the algo *seededWatershedSegmentation*
  3. Launch a run
  4. Once it's finished, the output visualization will show the result. Switch the view to 2D (-> sync) and slider to compare the input and output.
  5. Open a new Workspace **CellImageAnalysis**, choose the algo *cellPropertyTissueAnalysis* 
  6. Launch a run
  7. In the 3D view, switch the visualization algotihm to  *...MarchingCubes* and display the property layer
  8. Export the pipeline as JSON


## Image Enhancement Scenario

Improving image segmentation with image enhancement methods

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


With imageEnhancement package:

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

## Image Registration Scenario

Perform iterative registration within one registration workspace

  1. **Browsing**
      - Load image CZI/qDII-CLV3-PIN1-PI-E37-LD-SAM7-T0-P3.czi
      - Export to manager
      - Load image CZI/qDII-CLV3-PIN1-PI-E37-LD-SAM7-T10-P3.czi
      - Export to manager

  2. **Registration** The layout of the workspace registration is as follow:

  |Input | 	Output  |
  |-----|:-----|
  |floating image | stack + push button |
  |reference image | output image + export button |
  
  - top view (floating) -> TO
  - bottom view (reference) -> T10
  - algo registrationTimagetk
    - Method -> rigid
    - Channel -> Ch1_EBFP
    - Apply -> It fills the output view (bottom right)
  - Sync output and reference view to compare
  - Press "Iterate ⇧" button to validate results
    - output integrates the stack (top right)
    - transformation and image are stored
    - stack elements are highlighted
    - output image moves to floating view (with an animation)
    - output view is cleared
  - algo registrationTimagetk
    - Method -> affine
    - Apply
      - Last image from the stack is passed as input as it was previously moved into the floating image view
      - Computes registration -> incremental transformation
      - Fills the output view with registered image
    - Press "Push ⇧" button to validate results
    - Come down one level in the stack
      - anterior image moves to the input view
      - output view is cleared
    - Change parameter values and press apply
    - Press "Push ⇧" button to validate results
        - posterior stack items are deleted (with an animation)
        - new output integrates the stack
    - Press Export to save the actual image into the world and add a new node in the pipeline. When exporting the pipeline, we also save the stack    composition to be able to redo, re-load the computation.            
  
  
## Python Workspace Scenario
 Dynamically implement an image resampling plugin

  1. **Browsing**
      - Load image YR01_t40_crop.inr.gz
      - Export to manager

  2. **Python Algorithm**
      - Change plugin name to isometricResampling
      - Change plugin documentation to Perform an isometric resampling on an image.
      - Add input form: gnomonImage / img
      - Add output form: gnomonImage / resampled_img
      - Add parameter: Double / voxelsize / Resampled voxelsize
      - Change parameter bounds to 0.1, 5.
      - After # add your imports before the next gnomon tag add the import line: ```python  from timagetk.algorithms.resample import isometric_resampling```
      - After the # implement the run method replace pass by the following lines: 
      ```python self.resampled_img[time] = isometric_resampling(img, method=self['voxelsize'])```
      - Swipe to the right in the menu
      - Put the image as input (if not done automatically)
      - Apply
      - Change voxelsize parameter
      - Apply
      - Export to manager

  3. Python Algorithm
      - Load [testHessianEigenvals.py](https://gitlab.inria.fr/gnomon/gnomon-scripts/-/blob/develop/2.x.x/gnomonFormAlgorithm/testHessianEigenvals.py)     - Show the parsed info
      - Swipe to the right in the menu
      - Apply
      - Export to manager

  4. Pipeline
      - Show the generated nodes
