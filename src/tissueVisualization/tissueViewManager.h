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

#include "tissueCoreExport.h"

#include <QtCore>

class tissueActor;
class tissueActorMesh;
class tissueActorVolume;

class vtkImageData;
class vtkPolyData;

class TISSUECORE_EXPORT tissueViewManager : public QObject
{
    Q_OBJECT

public:
    tissueActor *actor(vtkPolyData *mesh);
    tissueActor *actor(vtkImageData *volume);

public:
    tissueActor *insert(vtkPolyData *mesh);
    tissueActor *insert(vtkImageData *volume);

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
     tissueViewManager(void);
    ~tissueViewManager(void);

private:
    class tissueViewManagerPrivate *d;

private:
    friend class tissueView;
};

//
// tissueViewManager.h ends here
