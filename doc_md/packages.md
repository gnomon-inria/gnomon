# Gnomon distribution

To share algotrithms, gnomon provides abstractions that can be used to create **plugins**. Then a set of plugins that all together provide a high-level functionality (cell-segmenting and tracking, 3D plant architecture modeling, multi-cellular simulation, etc) are group into a **package**. 

## Plugins

Gnomon plugins refer to algorithms that can be used in the different workspace to process the input form. Each workspace defines its own list of plugins that are discovered dynamically by the workspace depending on the packages that have been downloaded in Gnomon. Parameters for this plugin are dynamically introspected and graphically presented to the user for fine tuning of the algorithm.

TODO image

The workspace right panel offers a selection of plugins (see popup menu) that are available in this workspace. The parameters below correspond to the parameters of the currently selected plugin.


## Gnomon packages
A Gnomon package is a set of data structures, plugins and possibly workspaces and form abstractions that are defined by a user or a group of users. A protocole to set-up a Gnomon package is provided in the Gnomon documentation.

~If a package is mature, the developers may want to register it in the Gnomon store. The package must then comply stricly with the Gnomon package publication recommendations and will be reviewed for acceptance in  the  store.~

