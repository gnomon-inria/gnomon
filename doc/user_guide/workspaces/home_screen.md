# Home Screen

The Home screen is the landing page of the Gnomon application where you can access your recently opened **Projects**[{fas}`book-open;sd-text-primary fa-2xs`](project-definition), and create new ones.

(home-screen-new-project)=
## Creating a new Project

You can create a new Project by clicking on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`plus; fa-1x`&nbsp; **NEW** </span> button:

The "New Project" dialog that pops lets you select a directory path on your local system that will become a Gnomon Project. It also allows you to give a personalized name and a quick description to the project, and lets you choose in which **Workspace**[{fas}`book-open;sd-text-primary fa-2xs`](workspace-definition) you wish to begin when starting a new session:
* The [**Data Browsing** Workspace](../workspaces/data_browsing) to [load data files form your system](../tutorial/2_load_data)
* The [Python Algorithm Workspace](../workspace/python_algorithm) to start by running a custom plugin
* The [L-System Model Workspace](../workspace/lsystem_model) to create a branching architecture from [L-System `lpy` code](https://lpy.readthedocs.io/en/latest/)
* The [Morphonet Workspace](../workspace/morphonet) to download datasets from the [MorphoNet 3D+t browser](https://morphonet.org/)

## Opening an existing Project

In the case where you have a folder on your system that is already a Gnomon Project (*e.g.* a Project shared by another Gnomon user) you can directly load it by clicking on the <span class="sd-bg-primary sd-text-light sd-rounded-2 sd-badge"> {fas}`folder-open; fa-1x`&nbsp; **OPEN** </span> button. This opens a dialog that lets you browse your file system to select the path of the Project:

:::{dropdown} Not a Gnomon Project
:color: info
:icon: info

If the folder you selected is actually not an existing Gnomon Project, you will be redirected to the "New Project" dialog to create it with a customized name and description.
:::

(home-screen-load-recent-project)=
## Loading a recent Project
