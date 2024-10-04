# 2. Load a Form from a local file

When you open a new session in a **Project** (either [the first time you create it](home-screen-new-project) or when you [create a new blank session](home-screen-load-recent-project), you may start in the [**Data Browsing Workspace**](../workspaces/data_browsing) if you chose it as Initial Workspace in the project configuration. The Data Browsing Workspace is where you will be able to load the data files that you want to process as **Forms**[{fas}`book-open;sd-text-primary fa-2xs`](form-definition).

## 2.1 Download example data

For the rest of this Tutorial, you will be using example data files that will be included in your project directory. These files contain {bdg-link-success}`Image <../forms/image.html>` Forms that you will be able to load and interact with in the Data Browsing Workspace.

::::{admonition} Step 1
:class: tutorial

Download the  [**p58** sample data](p58-sample-data) <a href="https://gitlab.com/api/v4/projects/61953598/repository/archive/?path=forms/image/p58/" class="sd-outline-primary sd-rounded-1 sd-badge">{fas}`download; fa-1x`</a>, and extract the downloaded archive file. In your `gnomon_tutorial/` directory, create a new folder named `data/`, and move the three extracted files there.

:::{raw} html
    <video width="93%" height="auto" controls muted>
        <source
            src="https://gitlab.com/gnomon-inria/gnomon-videos/-/raw/main/tutorial/GnomonTutorial_2_1.mp4"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

The `data/` folder should now appear in the Project Browser, in the menu on the right.

## 2.2 Load a data file

The Project Browser lets you navigate all the data files that are inside the root directory of your Project, in order to load them. Loading a data file relies on a **Reader**[{fas}`book-open;sd-text-primary fa-2xs`](reader-definition) to create a new Form (image, mesh and so on) that will be displayed in the Workspace. For more details, you can have a look to the different options to [load the files from your Project](data-browsing-project-browser).

::::{admonition} Step 2
:class: tutorial

Expand the `data/` folder in the Project Browser, and double-click on the file `p58-t1_imgFus_down_interp_2x.inr.gz` to load it into the Workspace.

Given the file **extension** `inr.gz`, Gnomon tries to guess what type of Form it contains, but since there is not a unique possibility, it proposes you several compatible Readers. Since the file corresponds to an {bdg-link-success}`Image <../forms/image.html>` Form, select the <span class="sd-outline-dark sd-text-dark sd-rounded-2 sd-badge">**Intensity Image Reader**</span> option, and press on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> **OK** </span> button.

:::{raw} html
    <video width="100%" height="auto" controls muted> 
        <source
            src="https://gitlab.com/gnomon-inria/gnomon-videos/-/raw/main/tutorial/GnomonTutorial_2_2.mp4"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

The Image Form that you have loaded through the Reader is now displayed in the central 3D **View**[{fas}`book-open;sd-text-primary fa-2xs`](view-definition) of the Data Browsing Workspace. The Reader that you chose is now saved and will be used by default to load this file from now on.

## 2.3 Interact with the Visualization

Once the Form is loaded, its rendering in the View of the Workspace relies on a **Visualization**[{fas}`book-open;sd-text-primary fa-2xs`](visualization-definition). It is possible to configure the
Visualization through the *View Menu* located in the left panel of the Gnomon window.

::::{admonition} Step 3
:class: tutorial

In the View menu, make sure that the <span class="sd-outline-dark sd-text-dark sd-rounded-2 sd-badge">**Image**</span> Form is selected. The lower part of the menu allows you to change values of the Parameters of its Visualization. In the LUT drop-down list select the `viridis` colormap, then drag the slider located underneath to adjust the contrast. 

In the upper left corner of the View, click on the {far}`square; fa-1x` icon to switch from 3D to 2D rendering. Click on the <span class="sd-outline-dark sd-text-dark sd-rounded-1 sd-px-1 sd-badge">**XZ**</span> button to change the orientation of the 2D slice, then drag the slider on the left side to browse the image. Zoom in or out using the mouse wheel, hold <span class="sd-outline-dark sd-text-dark sd-rounded-1 sd-px-1 sd-badge">{fas}`up-long; fa-1x` Shift</span> and drag to pan in the image slice. Click on the {fas}`cube; fa-1x` icon to go back to a 3D rendering and use the mouse to rotate the 3D volume.

:::{raw} html
    <video width="100%" height="auto" controls muted> 
        <source
            src="https://gitlab.com/gnomon-inria/gnomon-videos/-/raw/main/tutorial/GnomonTutorial_2_3.mp4"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

Being able to explore your data in 3D is a very important first step. For a more detailed example, you can have a look at [the options you have to visualize an Image Form](../../gallery/image_visualization). Now, if you want to go further, you can read the next step of the tutorial to see [**How to actually run a processing step on your data**](3_run_algorithm).