# 3. Run a first Algorithm

Now that you have loaded your data, what you generally want to do next is to process it, using several consecutive computational steps. In Gnomon, you will achieve this by creating **Workspaces**[{fas}`book-open;sd-text-primary fa-2xs`](workspace-definition) that visually represent the execution of one of these steps.

## 3.1 Export the loaded Form

To make the **Form** you just loaded accessible from outside the [**Data Browsing Workspace**](../workspaces/data_browsing), the first thing you need to do is to export it to the **World**, a space that gathers all the Forms available to the whole Gnomon application.

::::{admonition} Step 1
:class: tutorial

Click on the <span class="sd-bg-secondary sd-text-light sd-rounded-circle sd-badge sd-px-2"> {fas}`caret-up; fa-1x` </span> Export button in the upper right corner of the **View** to send the {bdg-link-success}`Image <../forms/image.html>` Form you previously loaded from the Data Browsing Workspace into the World.

:::{raw} html
    <video width="93%" height="auto" controls muted>
        <source
            src="https://gitlab.com/gnomon-inria/gnomon-videos/-/raw/main/tutorial/GnomonTutorial_3_1.mp4"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

The Form "Image 1" now appears in the World with a thumbnail image displaying the way it was visualized. For more information, you can have a look at [what it is possible to do with Forms in the World](../concepts/world).

## 3.2 Create a new Workspace

It is now possible to use the Form in other Workspaces, notably to set as the *input* Form of the **Algorithm**[{fas}`book-open;sd-text-primary fa-2xs`](algorithm-definition) they operate. At any moment it is possible to create a new Workspace and move to the next step of your processing.

In our case, the type of processing we would like to perform is to identify the different cells in the image. This process is called **Image Segmentation** where each region of the image corresponding to a cell is assigned a unique label. We will have to open a Workspace that allows to perform this operation. 

::::{admonition} Step 2
:class: tutorial

Press the <span class="sd-outline-secondary sd-text-secondary sd-rounded-2 sd-badge"> {fas}`folder-plus; fa-1x`&nbsp; **NEW WORKSPACE** </span> button to open the dialog that lets you open new Workspaces.

In the Workspace list on the left, select the "Segmentation" Workspace (you can read explanations on the selected Workspace in the panel on the right). Click on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> **OPEN** </span> button the create a new Segmentation Workspace.

:::{raw} html
    <video width="93%" height="auto" controls muted>
        <source
            src="https://gitlab.com/gnomon-inria/gnomon-videos/-/raw/main/tutorial/GnomonTutorial_3_2.mp4"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

The application opens the [Segmentation Workspace](../workspaces/segmentation) you just created, and (provided it is of a supported Form type) immediately sets a Form as input of the Algorithm by putting the active Form from the World into the left *input* **View** of the Workspace.

## 3.3 Run the Algorithm

The Workspace is now ready to apply the Algorithm it embeds on the Form passed as input. There are subsequent choices to be made by the user:
* First, select a **Plugin** from the list to define which Segmentation Algorithm is going to be used
* In a second time, choose values for the different **Parameters**[{fas}`book-open;sd-text-primary fa-2xs`](parameter-definition) required by the selected Plugin
This configuration is performed in the *Workspace Menu* located in the right panel of the Gnomon window

::::{admonition} Step 3
:class: tutorial

In the *Algorithm* drop-down list, select the *Auto Watershed* Plugin. You may click on the {fas}`circle-info; fa-1x` information button to get more details on the Plugin and its parameters.

Set the *Gaussian Sigma* Parameter to *0.6* by typing the value and pressing Enter. Toggle the *Padding* section by clicking on the {fas}`plus; fa-1x` icon, then set the *Orientation* parameter to *down* and the *Padding* Parameter to True.

Press the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`play; fa-1x`&nbsp; **RUN** </span> button to apply the Algorithm on the input Form.

:::{raw} html
    <video width="93%" height="auto" controls muted>
        <source
            src="https://gitlab.com/gnomon-inria/gnomon-videos/-/raw/main/tutorial/GnomonTutorial_3_3.mp4"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

The Plugin will now compute a {bdg-link-success}`CellImage <../forms/cell_image.html>` Form from the input {bdg-link-success}`Image <../forms/image.html>` Form using the values of the Parameters you supplied.

:::{dropdown} Algorithm Logs
:color: secondary
:icon: bug

You can access the logs of the Algorithm during its execution by expanding the panel containing the progress bar. Click on the middle handle to open it and display a log console. Click on it again to collapse it back.
:::

Once it is computed, the *output* Form is displayed in the right View of the Workspace where you can interact with it.

## 3.4 Save the resulting Form 

At this stage, the output Form of the Algorithm only exists inside the current Workspace, and is not accessible to the rest of the application. This allows to re-run the Algorithm with different Parameter values until the result is satisfactory. This is generally the moment when you would like to export it to the World to *validate* the output of the Workspace, and possibly save the result on your disk.


::::{admonition} Step 4
:class: tutorial

Click on the <span class="sd-bg-secondary sd-text-light sd-rounded-circle sd-badge sd-px-2"> {fas}`caret-up; fa-1x` </span> Export button in the upper right corner of the output View where to export the segmented image to the World.  In the thumbnail that just appeared in the World, right click to display the menu, and click on the <span class="sd-outline-dark sd-text-dark sd-rounded-2 sd-badge"> **Save** </span> item to open a file dialog.

The dialog should open in your project directory. Navigate to the `data/` folder, select the `p58-t1_imgFus_down_interp_2x.inr.gz` file and change the filename in the dialog into `p58-t1_imgSeg_down_interp_2x.tif`, then press on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> **SAVE** </span> button.

:::{raw} html
    <video width="93%" height="auto" controls muted>
        <source
            src="https://gitlab.com/gnomon-inria/gnomon-videos/-/raw/main/tutorial/GnomonTutorial_3_4.mp4"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

The World will write the Form you computed onto your disk, relying on a suitable **Writer**[{fas}`book-open;sd-text-primary fa-2xs`](writer-definition) to write the data in the format specified by the file *extension*. Congratulations! You have been able to save the result of your work to use it with other applications ...or in another Gnomon session! You can now move on to the next level to discover [**How to replay your pipeline on more data**](4_replay_pipeline).
