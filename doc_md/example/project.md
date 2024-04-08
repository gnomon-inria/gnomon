# 1. Create a new project

:::{warning}
This section is under construction! Some information might be missing.
:::

When you launch the Gnomon application, you land on a home screen where you can have access to your recently opened **Projects**. A project in Gnomon represents a series of processing steps performed starting from one or several data files. It can generally be represented as a [**Pipeline**](../concepts.md#pipeline) where the steps are building blocks linked together.

## Load a project

Gnomon projects are saved in a readable `.json` format in which you can notably find the paths to the data files that have been read, or the values of the **Parameters** that have been used in the different computation steps. There are two possibilities to load an existing `.json` project file:
* If it appears in the list in the center, you can simply click on it
* You can also click on the *Load* button to open a file from your local system

When you open an existing project, its processing steps will be re-executed in the logical order, so that you will obtain the same result as the first time that it was originally run.

:::{note}
If you manually modify the values of the parameters, or if you change the file path in the `.json` file before loading it, you will of course get a different result. 
Note that it might be an interesting way to process several data files with the same computational pipeline. 
:::

## New project

However, the first time you open Gnomon, the *Recent projects* section will be empty, and you generally won't have a `.json` file to load. You will need to create a new project by clicking on the `New` button. This action lets you start from a blank pipeline, and brings to an empty  [**Browsing Workspace**](../workspace/browsing)  where you can [import your data and start designing your own processing pipeline...](browser)