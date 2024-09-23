# 2. Load a Data File

When you open a new session in a **Project** (either [the first time you create it](1-1-new-project) or when you [create a new blank session](1-3-load-recent-project), you may start in the [**Data Browsing Workspace**](../workspaces/data_browsing) if you chose it as the default starting Workspace.

The Data Browsing Workspace is a space dedicated to the loading of data files containing **Forms**[{fas}`book-open;sd-text-primary fa-2xs`](form-definition) from your local system. Loading the data will create a new Form (image, mesh and so on) that will be displayed in the central 3D **View**[{fas}`book-open;sd-text-primary fa-2xs`](view-definition) where you will be able to interact with it.

## 2.1 Loading from the Project Browser

In the menu on the right, a Project Browser lets you navigate in your local file system. There are two options to load a data file from this component:
* You can simply double-click on it
* Or you can  drag&drop it into the central View

When you open a file for the first time, Gnomon will try to guess what [**Form**](../concepts.md#forms) it contains based on its extension. However in some cases the extension alone is not enough, and you will be asked to select what **Reader** to use.

:::{admonition} Example
For instance a `.tif` file can contain an **Itensity Image** coming directly from a microscope acquisition, but it can also contain a **Labelled Image** issuing from a cell segmentation process.
:::

The Reader you choose the first time that you read a given file will be saved and used by default for this file from ow on If you wish to change, you can use the `Load as` button to access the Reader selection once again. 


## 2.2 Loading External files

## 2.3 Loading multiple files

If several files are selected, Gnomon will try to read them as a **Temporal Sequence**, so you need to make sure that they all have the same format (and actually correspond to a time series of the same type of Form). You can achieve the same result by using the `Load...` button and select multiple files.