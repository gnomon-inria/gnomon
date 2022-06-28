# Image Segmentation Scenario

Segment a 3D membrane-marker image and measure cell properties


1. **Browsing**
    - Load image p58-t0_imgFus_down_interp_2x.inr.gz
    - Export to manager


2. **Segmentation**
    - Drop the image from the manager it it has not been done automatically
    - Choose the plugin *seededWatershedSegmentation*
    - Set the following parameter values:
      - h_min -> 2
      - volume_threshold -> ~1000
    - Press Apply
   
Once the computation is finished, the output visualization will show the result. Switch the view to 2D (-> sync) and slider to compare the input and output.

3. **Cell Image Quantification** 
   - Choose the plugin *cellFeaturesTissueImage* 
   - Select the features *volume* and *layer* in the list
   - Press Apply
   
In the view menu of the 3D viewer, switch the visualization plugin to  *...MarchingCubes* and display the property *layer*

4. **Pipeline**
    - Open the pipeline panel at the bottom
    - Edit the pipeline name and information
    - Press on the save icon to export the pipeline as a JSON file


**Video**

<iframe width="560" height="315" src="https://www.youtube.com/embed/ayVMaLrO24A?rel=0" frameborder="0" allowfullscreen></iframe> 

