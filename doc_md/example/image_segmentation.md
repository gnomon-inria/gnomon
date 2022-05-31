# Image Segmentation Scenario

To test Image Segmentation, here are the steps:

  1. **Browser** Load image p58-t0_imgFus_down_interp_2x.inr.gz
  2. Open a new Workspace **Segmentation**, choose the algo *seededWatershedSegmentation*
  3. Launch a run
  4. Once it's finished, the output visualization will show the result. Switch the view to 2D (-> sync) and slider to compare the input and output.
  5. Open a new Workspace **CellImageAnalysis**, choose the algo *cellPropertyTissueAnalysis* 
  6. Launch a run
  7. In the 3D view, switch the visualization algotihm to  *...MarchingCubes* and display the property layer
  8. Export the pipeline as JSON

