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

#include "gnomonCoreExport.h"

#include <QtCore>

class gnomonActor;
class gnomonActorMesh;
class gnomonActorMeshCellComplex;
class gnomonActorMeshCellGraph;
class gnomonActorVolume;

class gnomonCellComplex;
class gnomonCellGraph;
class vtkImageData;
class vtkPolyData;

class GNOMONCORE_EXPORT gnomonViewManager : public QObject
{
    Q_OBJECT

public:
    gnomonActor *actor(vtkPolyData *mesh);
    gnomonActor *actor(vtkImageData *volume);
    gnomonActor *actor(gnomonCellComplex *cellcomplex);
    gnomonActor *actor(gnomonCellGraph *cellgraph);

public:
    gnomonActor *insert(vtkPolyData *mesh);
    gnomonActor *insert(vtkImageData *volume);
    gnomonActor *insert(gnomonCellComplex *cellcomplex);
    gnomonActor *insert(gnomonCellGraph *cellgraph);

public:
    void remove(vtkPolyData *mesh);
    void remove(vtkImageData *volume);
    void remove(gnomonCellComplex *cellcomplex);
    void remove(gnomonCellGraph *cellgraph);

public:
    QList<vtkPolyData *> meshes(void);
    QList<vtkImageData *> volumes(void);
    QList<gnomonCellComplex *> cellcomplexes(void);
    QList<gnomonCellGraph *> cellgraphs(void);

signals:
    void inserted(vtkPolyData *mesh);
    void inserted(vtkImageData *volume);
    void inserted(gnomonCellComplex *cellcomplex);
    void inserted(gnomonCellGraph *cellgraph);

signals:
    void removed(vtkPolyData *mesh);
    void removed(vtkImageData *volume);
    void removed(gnomonCellComplex *cellcomplex);
    void removed(gnomonCellGraph *cellgraph);

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
