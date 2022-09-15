# ChangeLog

## version 0.71.0 - 2022-09-15
* Facilitate Workspace navigation (dialog if no exported Forms, switch button visibility)
* Display the log of running Algorithms within the application
* More information when reading a Form (reader description images, error toasts)
* Fixed issues with project reloading (pipeline info, non .json files)
* Pipeline info screen at new project creation
* Directly load a plugin by double clicking in the Workspace dialog
* Default auto-render/apply is now an option in the Settings
* Fixed the synchronization of views after running an algorithm several times
* Single channel image visualization with intensity histogram
* Include a spot detection Workspace
* Rename image channels through a Preprocessing plugin

## version 0.70.1 - 2022-07-19
* Handling of files with capitalized extensions and “.” characters
* Fixed the loading of .czi files
* Fixed the double click in the Browser panel
* Documentation of conda installation
* Automatic adjustment of image visualization ranges to min/max values
* Fixed the zoom in the 3D viewer and added a view shortcuts menu
* The World can now be expanded by hovering the mouse
* Removing a Form from the World now removes the corresponding node in the Pipeline
* Fixed the execution of custom Python plugins when reloading a project

## version 0.70.0 - 2022-04-15
* first morphonet api
* 3d and 4d numpy wrapper
* support time series
* add workspace cellular tracking 

## version 0.60.0 - 2022-03-15
* qt6
* monaco editor for python workspace
* can save/load pipelines
* visualize metadata
* better includes
* python 3.9


## version 0.20.0 - 2022-02-11
* refactor python gnomon module
* simplify and clarify decorators

## version 0.17.0 - 2021-09-17
* replace series by dict in Python
* use dtkCoreParameters
* check plugin versions
* update vtk to 9.0

## version 0.9.1 - 2018-11-27
* demo release before image series merge
* add gnomon application to add a workflow for working on images
* fusion with/without landmarks
* segmentation
* browsing and loading from filesystem
* colormaps per channels
* world with metadata on images
* blending with 2 images
* 
## version 0.9.0 - 2018-11-20
*initial release
