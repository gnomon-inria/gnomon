# 1. Create a Project

When you launch the Gnomon application, you land on a home screen where you can have access to your recently opened **Projects**[{fas}`book-open;sd-text-primary fa-2xs`](project-definition). Any processing you will perform in Gnomon has to be related to a Project directory, inside which the necessary information will be stored.

(1-1-new-project)=
## 1.1 New Project

The first time you open Gnomon, the *Recent projects* section will be empty, and you generally won't have an existing Project directory to open. You will need to create a new project by clicking on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`plus; fa-1x`&nbsp; **NEW** </span> button:

:::{raw} html
    <video width="100%" height="auto" controls muted> 
        <source
            src="https://mybox.inria.fr/f/e719ccc8e5134e4e9908/"
            type="video/mp4" style="margin-left: 0px; margin-right: 0%"
        >
        Your browser does not support the video tag.  
    </video>
:::

The "New Project" dialog that pops lets you select a directory path on your local system that will become a Gnomon project. It also allows you to give a personalized name and a quick description to the project, and lest you choose in which **Workspace**[{fas}`book-open;sd-text-primary fa-2xs`](workspace-definition) you wish to start when starting a new session:
* The [**Data Browsing** Workspace](../workspaces/data_browsing) to [load data files form your system](2_load_data)
* The [Python Algorithm Workspace](../workspace/python_algorithm) to start by running a custom plugin
* The [L-System Model Workspace](../workspace/lsystem_model) to create a branching architecture from [`lpy` L-System code](https://lpy.readthedocs.io/en/latest/)
* The [Morphonet Workspace](../workspace/morphonet) to download datasets from the [MorphoNet 3D+t browser](https://morphonet.org/)

For the rest of this tutorial, we advise that you keep the **Data Browsing** Workspace as default option.

## 1.2 Opening an existing Project

In the case where you have a folder on your system that is already a Gnomon Project (*e.g.* a Project shared by another Gnomon user) you can directly load it by clicking on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`folder-open; fa-1x`&nbsp; **OPEN** </span> button. This opens a dialog that lets you browse your file system to select the path of the Project:

:::{raw} html
<video width="100%" height="auto" controls muted>
    <source
        src="https://mybox.inria.fr/f/e719ccc8e5134e4e9908/"
        type="video/mp4" style="margin-left: 0px; margin-right: 0%"
    >
    Your browser does not support the video tag.  
</video>
:::

:::{dropdown} Not a Gnomon Project
:color: info
:icon: info

If the folder you selected is actually not an existing Gnomon Project, you will be redirected to the "New Project" dialog to create it with a customized name and description. 

:::

(1-3-load-recent-project)=
## 1.3 Loading a recent Project

