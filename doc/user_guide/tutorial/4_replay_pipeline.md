# 4. Replay a Pipeline

While you were processing your data through **Workspaces**, Gnomon has been tracking your actions to construct a **Pipeline**[{fas}`book-open;sd-text-primary fa-2xs`](pipeline-definition), a graph that recapitulates the steps required to compute any of the **Forms** you manipulated. This Pipeline can be exported to be used as a tool of its own, notably to replay the same computational process on some other input data.

## 4.1 Interact with the generated Pipeline

The Pipeline corresponding to your current Session is constantly available in the collapsible upper panel on the Gnomon window, which is collapsed by default. If you expand it you will have access to the dependency graph as it is being constructed, and possibly define custom names for its different elements.

::::{admonition} Step 1
:class: tutorial

Expand the Pipeline panel, and adjust the display of the Pipeline Nodes. Select different Forms in the World to see where they appear in the Pipeline.

Hover the <span class="sd-bg-danger sd-text-light sd-rounded-2 sd-badge">cellImageFromImage1</span> Node and click on the {fas}`pen; fa-1x` icon to open a dialog to customize the Node. Modify its name into "segmentation", and modify the name of its output port into "SegmentedImage".

Click on the title to open a dialog to customize your Pipeline. Modify its name to "image_segmentation", and add a short description text.

:::{raw} html
    <video width="93%" height="auto" controls muted>
        <source
            src="https://mybox.inria.fr/f/e719ccc8e5134e4e9908/"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

Giving custom names to the steps and outputs is a good way to keep track of what you are doing, especially when the processing involves multiple steps. You can have look [here](../concepts/pipeline) to learn more about the logic behind the Pipeline.

## 4.2 Save the Pipeline as a JSON file

If you plan to repeat the processing on more data, or simply to keep a track of the processing you have been working on, saving the Pipeline in a local file is essential. Gnomon saves Pipelines in a [JSON formatted file](https://ecma-international.org/publications-and-standards/standards/ecma-404/) that stores all the information required to replay the computations.

::::{admonition} Step 2
:class: tutorial

In the upper rigt corner of the Pipeline panel, click on the {fas}`floppy-disk; fa-1x` icon to open a file dialog to select where to save your Pipeline. The dialog should open in the root directory of your Project. Create a `pipelines/` folder, navigate in it, and press the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> **SAVE** </span> button to save the `image_segmentation.json` file.

:::{raw} html
    <video width="93%" height="auto" controls muted>
        <source
            src="https://mybox.inria.fr/f/e719ccc8e5134e4e9908/"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

The Pipeline JSON file stores the information, notably the values of the Parameters, or the relative paths of the data files, in a human-readable format. This can be useful to know at a glance what has been used to generate a given file.

## 4.3 Replay a Pipeline as a new Session

With the information contained in Pipeline file, it is possible to re-create the set of Forms and Workspaces you generated with the Gnomon application, in other words to replay your Gnomon **Session**[{fas}`book-open;sd-text-primary fa-2xs`](session-definition). But more than that, it allows to replay *a different Session* by changing for instance the paths of the data that were loaded. 

::::{admonition} Step 3
:class: tutorial

In the upper right menu bar, click on the {fas}`square-xmark; fa-1x sd-text-danger` icon to close the Project and return to the [Home Screen](../workspaces/home_screen). In the **Gnomon Tutorial** Project card, click on the {fas}`ellipsis-vertical; fa-1x sd-px-2` icon to access more reloading options, and click on the {fas}`network-wired; fa-1x` icon to reload a session from a Pipeline file.

Select the `image_segmentation.json` file in the `pipelines/` folder. This opens a pipeline reloading dialog where you can configure the paths of the files. In the input panel on the left, click on the {fas}`folder-open; fa-1x sd-text-success` icon and select `p58-t2_imgFus_down_interp_2x.inr.gz`. In the output panel on the right, change the path to `p58-t2_imgSeg_down_interp_2x.tif`, then click on the <span class="sd-bg-success sd-text-light sd-rounded-2 sd-badge"> {fas}`play; fa-1x`&nbsp; **RUN** </span> button to replay the Session.

:::{raw} html
    <video width="93%" height="auto" controls muted>
        <source
            src="https://mybox.inria.fr/f/e719ccc8e5134e4e9908/"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

Gnomon will now automatically run the same computational steps as you performed manually, using the same Plugins and Parameter values. In the end, the resulting Form of this new processing is saved as a local file in your system, along with the process allowing to reproduce it.

Now that you have reached the end of this Gnomon Tutorial, you can explore [the available Workspaces](available-workspace-list) to find what would suit your data, or have a look at [the Gallery of Examples](../../gallery/index) to discover what it is possible to achieve with Gnomon. 