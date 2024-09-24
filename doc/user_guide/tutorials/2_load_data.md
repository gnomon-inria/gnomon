# 2. Load a Data File

When you open a new session in a **Project** (either [the first time you create it](1-1-new-project) or when you [create a new blank session](1-3-load-recent-project), you may start in the [**Data Browsing Workspace**](../workspaces/data_browsing) if you chose it as the default starting Workspace.

The Data Browsing Workspace is a space dedicated to the loading of data files containing **Forms**[{fas}`book-open;sd-text-primary fa-2xs`](form-definition) from your local system. Loading a data file relies on a **Reader**[{fas}`book-open;sd-text-primary fa-2xs`](reader-definition) to create a new Form (image, mesh and so on) that will be displayed in the central 3D **View**[{fas}`book-open;sd-text-primary fa-2xs`](view-definition), where you will be able to interact with it.

## 2.1 Loading from the Project Browser

In the menu on the right, the Project Browser lets you navigate and select files from the root directory of your Project. There are three options to load the selected data file from this component:
* Click on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`up-long; fa-1x`&nbsp; **LOAD** </span> button
* Double-click on the file
* Drag&drop the file into the View

:::{raw} html
    <video width="100%" height="auto" controls muted> 
        <source
            src="https://mybox.inria.fr/f/e719ccc8e5134e4e9908/"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::

When you open a file for the first time, Gnomon will try to guess what type of Form it contains based on its **extension**. However in some cases the file extension alone is not enough to know how to read it, and you will be asked to select what Reader to use.

:::{admonition} Example
:class: tip
For instance a file with the `.tif` extension can contain an {bdg-link-success}`Image <../forms/image.html>` Form coming directly from a microscope acquisition, but it may also contain a {bdg-link-success}`Cell Image <../forms/cell_image.html>` Form issuing from a cell segmentation process.
:::

The Reader you choose the first time that you read a given file will be saved and used by default for this file from now on. If you wish to change, you can use the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`file-arrow-up; fa-1x`&nbsp; **LOAD AS** </span> button to access the Reader selection once again. 

## 2.2 Loading External files

You also have the possibility to load data files that are not located inside the root directory of your Project. To do so, you can click on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`folder-plus; fa-1x`&nbsp; **BROWSE FILES...** </span> button, or simply drag&drop a file from the File Browser of your system.

But for Gnomon to be able to track these external data files, you will have to add a **parent directory** of the file you want to load as an **External Data Directory** of your Project. External Data Directories are secondary paths where Gnomon will search for relative paths, and  once they've been added to the project, they will appear in the Project Browser to make it easier to access their content.

:::{raw} html
    <video width="100%" height="auto" controls muted> 
        <source
            src="https://mybox.inria.fr/f/e719ccc8e5134e4e9908/"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::

:::{dropdown} Parent Directory
:color: primary
:icon: file-directory

The path you select to add to the **External Data Directories** of your Project does not necessarily have to be *the directory* containing the data file you want to load: it can be any of its **parent directories**.

In the case where external data files are already in a structured file architecture, we recommend that you add the root directory of this architecture to conveniently access all your files with only one External Data Directory.
:::


## 2.3 Loading multiple files

If several files are selected when you press the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`up-long; fa-1x`&nbsp; **LOAD** </span> button, or if you drag&drop several files from the Project Browser, Gnomon will try to read them as a single **Form** with multiple time points. You need to make sure that they all have the same format (and actually correspond to a time series of the same type of Form). 

You can also achieve the same result by selecting multiple files in the dialog that opens when you click on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`folder-plus; fa-1x`&nbsp; **BROWSE FILES...** </span> button.