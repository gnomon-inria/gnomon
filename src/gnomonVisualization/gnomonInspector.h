// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <gnomonVisualizationExport.h>

#include <QSplitter>

class gnomonInspectorPrivate;
class gnomonInspectorViewTree;

class gnomonActorMeshCellComplex;
class gnomonActorMeshCellGraph;
class gnomonActorMeshCellImage;
class gnomonActorVolume;

class GNOMONVISUALIZATION_EXPORT gnomonInspector : public QSplitter
{
    Q_OBJECT

public:
     gnomonInspector(QWidget *parent = Q_NULLPTR);
    ~gnomonInspector(void);

 public:
    gnomonInspectorViewTree *tree(void);

 public slots:
    void onActorMeshCellComplexSelected(gnomonActorMeshCellComplex *actor);
    void onActorMeshCellGraphSelected(gnomonActorMeshCellGraph *actor);
    void onActorMeshCellImageSelected(gnomonActorMeshCellImage *actor);
    void onActorVolumeSelected(gnomonActorVolume *actor);

public:
    gnomonInspectorPrivate *d;
};

//
// gnomonInspector.h ends here
