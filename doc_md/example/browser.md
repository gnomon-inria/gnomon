# 2. Load a data file

:::{warning}
This section is under construction! Some information might be missing.
:::

When you create a new **Project**, you start in the [**Browsing Workspace**](../workspace/browsing), a space dedicated to the loading of data files (images, meshes and so on) from your local system. The data you load will be displayed in the central 3D **View** where you will be able to interact with it.

## File browser

In the menu on the right, a File browser lets you navigate in your local file system. There are two options to load a data file from this component:
* You can simply double-click on it
* Or you can  drag&drop it into the central View

When you open a file for the first time, Gnomon will try to guess what [**Form**](../concepts.md#forms) it contains based on its extension. However in some cases the extension alone is not enough, and you will be asked to select what **Reader** to use.

:::{admonition} Example
For instance a `.tif` file can contain an **Itensity Image** coming directly from a microscope acquisition, but it can also contain a **Labelled Image** issuing from a cell segmentation process.
:::

The Reader you choose the first time that you read a given file will be saved and used by default for this file from ow on If you wish to change, you can use the `Load as` button to access the Reader selection once again. 

## Load multiple files

Files can also be drag&dropped from your native system file browser, directly into the View. If several files are selected, Gnomon will try to read them as a **Temporal Sequence**, so you need to make sure that they all have the same format (and actually correspond to a time series of the same type of Form). You can achieve the same result by using the `Load...` button and select multiple files.