# Image Registration Scenario

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
    