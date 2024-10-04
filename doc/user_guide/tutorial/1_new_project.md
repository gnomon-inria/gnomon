# 1. Create a Project

Any processing you will perform in Gnomon has to be related to a **Project**[{fas}`book-open;sd-text-primary fa-2xs`](project-definition), a directory inside which the necessary information will be stored. The first thing you will do when starting to use Gnomon is therefore to create a new Project.

:::{dropdown} Requirements
:color: success
:icon: info

To be able run this tutorial you will need to [have installed Gnomon ](../quickstart/install) and to install the [`gnomon-package-data`](../plugins/packages/gnomon_package_data) and  [`gnomon-package-tissueimage`](../plugins/packages/gnomon_package_image) [Plugin packages](../quickstart/install_packages).

:::

## 1.1 Open the New Project Dialog

When you launch the Gnomon application, you land on a [Home screen](../workspaces/home_screen) where you can have access to your recently opened Projects. However, the first time you open Gnomon, the *Recent projects* section will be empty, and you generally won't have an existing Project directory to open.

::::{admonition} Step 1
:class: tutorial

Let's create a new project by clicking on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`plus; fa-1x`&nbsp; **NEW** </span> button:

:::{raw} html
    <video width="93%" height="auto" controls muted> 
        <source
            src="https://gitlab.com/gnomon-inria/gnomon-videos/-/raw/main/tutorial/GnomonTutorial_1_1.mp4"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::
::::

## 1.2 Configure the Project

The "New Project" dialog that pops lets you select a directory on your local system that will become a Gnomon project. It also allows you to give it a name and a  description to the project, and to choose in which **Workspace**[{fas}`book-open;sd-text-primary fa-2xs`](workspace-definition)
to initialize a new session. For more information on the initial workspaces, you can have a look at [how to create a Project in general](home-screen-new-project).

::::{admonition} Step 2
:class: tutorial

Let's first create a directory for your Project: navigate to the directory of your choice in the file dialog, then create a new folder named `gnomon_tutorial` and select that folder with the dialog. Then, fill in the New Project dialog with the following details:
* **Name**: Gnomon Tutorial
* **Description**: A project to get started with Gnomon
* **Initial Workspace**: Data Browsing

Then press the <span class="sd-bg-success sd-text-light sd-rounded-2 sd-badge"> {fas}`folder-plus; fa-1x`&nbsp; **CREATE** </span> button.

:::{raw} html
<video width="93%" height="auto" controls muted>
    <source
        src="https://gitlab.com/gnomon-inria/gnomon-videos/-/raw/main/tutorial/GnomonTutorial_1_2.mp4"
        type="video/mp4" style="margin-left: 0px; margin-right: 0%"
    >
    Your browser does not support the video tag.  
</video>
:::

::::

When you press the <span class="sd-bg-success sd-text-light sd-rounded-2 sd-badge"> {fas}`folder-plus; fa-1x`&nbsp; **CREATE** </span> button, your project is created, and from now on it will appear in your list of Recent Projects. Learn more about how to load a recent project by taking a look at [the different loading options](home-screen-load-recent-project).

A new session immediately starts, and it will start in the [Data Browsing Workspace](../workspaces/data_browsing) since you chose it as Intitial Workspace. You can now proceed to the next step of this tutorial to learn [**How to load data files**](2_load_form).


