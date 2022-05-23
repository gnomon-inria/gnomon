# Image Python Plugin Scenario
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
