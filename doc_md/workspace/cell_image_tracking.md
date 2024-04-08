# Cell Image Tracking Workspace

The Cell Image Tracking Workspace is used to compute the lineage of cells in a segmented [Cell Image form](../form/cell_image) series of at least 2 time points. For some plugins, a corresponding intensity [Image form](../form/image) series can be also used as an input.

The Cell Image Tracking Workspace always displays two consecutive time points of the Cell Image, and it also allows to manually select cells to specify input pairings. It outputs the lineage tree and a Cell Lmage series where each cell knows its parent.