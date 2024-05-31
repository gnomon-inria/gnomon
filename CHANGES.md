# ChangeLog

## version 1.0.1 - 2024-05-31
* Improved `gnomon-utils package search`
* Fixed pipeline reloading in cli `gnomon-utils run`
* Fixed various issues in the Python Workspace
* Fixed inliststring checkbox not working
* Fixed issues in Workspace Morphonet

## version 1.0.0 - 2024-03-27
* Gather gnomon library and GUI in a single repository
* Include the notion of Project throughout the application:
    * Manage a hidden folder in each project directory
    * Manage data paths when reading external data files
    * Store the current session and make it possible to reload it
    * Serialize all forms and deserialize them when reloading session
    * Start a new session from apipeline, or a blank one
    * Manage code source files in Python/L-Py workspaces
    * Allow to define a thumbmail image per project
* Use metatypes to simplify Form creation and inspection
* Move from dtkImage to vtkImageData in Image Forms
    * Better memory management of Form data
* Manipulate forms and workspaces through UUIDs
* Allow to close workspaces when possible
* Configure pipeline input and output paths when replaying
* Display info on the current plugin in a dialog
* Explicit the notion of active view for each workspace
* Support time in 2D views (Matplotlib and Text)
* Zoom, pan and manage axes limits in Matplotlib View
* Track memory usage during execution
* Generalize progress bar to all algos and workspaces
* Display logs in a collapsible panel
* More intelligible workspace names
* Restyle 2D curve editors
* New command-line interface to install/update plugin packages
* Add wall properties in the CellImage abstraction
* Allow to pass a mesh as an input of quantification
* Switch from dark to light theme in GUI
* Various bugfixes

## version 0.81.1 - 2023-07-20
* fixing the registration workspace

## version 0.81.0 - 2023-07-06
* Enable screenshots in views
* Support opening several (interdependent) files in LPy workspace
* Edit all material properties (not just color) in LPy visualization
* Quantification workspace for (nuclei) point clouds
* More generic mesh API, mesh processing & solver workspaces
* Advanced 3D mesh visualization (color, isocontours, streams)
* More intuitive dialog for new project creation
* Fix pipeline reloading / running functionalities
* Additional buttons for 3D view camera/grid control
* Additional view camera menu for 3D view
* Fix reset and derivation length in LPy workspace
* Textual visualization of LStrings in LPy workspace
* Remove plugin forms/parameters from Python workspace menu

## version 0.80.0 - 2023-04-12
* LSP server for python workspace
* connect for python parameters
* fix morphonet curation
* manually pick cells for tracking workspace
* intelligible names for algorithms
* share visualization across workspaces
* uniformization of abstract views
* picker for curve2d
* fix lpy bugs
* lpy 2.13

## version 0.72.0 - 2023-02-03
* L-Py modelling workspace
* Semi-automatic cell tracking
* Intelligible names for plugins in the interface
* Close current project / remove recent projects
* Graphical parameters + texture parameters
* Highlight new workspaces compatible with active form
* Better memory management
* Description of forms in the documentation
* Pre-loading of plugins before creating project
* Active form by default in visu menu
* Draggers and sliders more visible
* A Gnomon icon on the launch bar! (linux)

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

## version 0.9.0 - 2018-11-20
* initial release
