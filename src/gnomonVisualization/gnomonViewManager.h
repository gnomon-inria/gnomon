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
class gnomonActorVolume;

class vtkImageData;
class vtkPolyData;

class GNOMONCORE_EXPORT gnomonViewManager : public QObject
{
    Q_OBJECT

public:
    gnomonActor *actor(vtkPolyData *mesh);
    gnomonActor *actor(vtkImageData *volume);

public:
    gnomonActor *insert(vtkPolyData *mesh);
    gnomonActor *insert(vtkImageData *volume);

public:
    void remove(vtkPolyData *mesh);
    void remove(vtkImageData *volume);

public:
    QList<vtkPolyData *> meshes(void);
    QList<vtkImageData *> volumes(void);

signals:
    void inserted(vtkPolyData *mesh);
    void inserted(vtkImageData *volume);

signals:
    void removed(vtkPolyData *mesh);
    void removed(vtkImageData *volume);

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
