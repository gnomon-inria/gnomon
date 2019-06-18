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

#include "gnomonVisualizationExport.h"

#include <QtCore>
#include <QWidget>

class dtkImage;
class gnomonActor;
class gnomonActorMeshCellComplex;
class gnomonActorMeshCellGraph;
class gnomonActorMeshCellImage;
class gnomonActorVolume;
class gnomonCellComplex;
class gnomonCellGraph;
class gnomonCellImage;
class gnomonInspector;

class GNOMONVISUALIZATION_EXPORT gnomonViewManager : public QObject
{
    Q_OBJECT

public:
    gnomonActor *actor(dtkImage *);
    gnomonActor *actor(gnomonCellComplex *);
    gnomonActor *actor(gnomonCellGraph *);
    gnomonActor *actor(gnomonCellImage *);

public:
    gnomonActor *insert(dtkImage *);
    gnomonActor *insert(gnomonCellComplex *);
    gnomonActor *insert(gnomonCellGraph *);
    gnomonActor *insert(gnomonCellImage *);

public:
    void remove(dtkImage *);
    void remove(gnomonCellComplex *);
    void remove(gnomonCellGraph *);
    void remove(gnomonCellImage *);

public:
    gnomonInspector *inspector(void);

public:
    QList<dtkImage *> images(void);
    QList<gnomonCellComplex *> cellcomplexes(void);
    QList<gnomonCellGraph *> cellgraphs(void);
    QList<gnomonCellImage *> cellimages(void);

signals:
    void inserted(dtkImage *);
    void inserted(gnomonCellComplex *);
    void inserted(gnomonCellGraph *);
    void inserted(gnomonCellImage *);

signals:
    void inserted(gnomonActor *);

signals:
    void removed(dtkImage *);
    void removed(gnomonCellComplex *);
    void removed(gnomonCellGraph *);
    void removed(gnomonCellImage *);

public slots:
    void clear(void);

public slots:
    void update(void);

private:
     gnomonViewManager(void);
    ~gnomonViewManager(void);

private:
    class gnomonViewManagerPrivate *d;

private:
    friend class gnomonView;
};

//
// gnomonViewManager.h ends here
