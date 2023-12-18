#include <gnomonVisualizationExport.h>

#include <dtkCore/dtkCoreParameters>


class gnomonAbstractView;


class gnomonAbstractVisualizationPrivate
{

public:
    gnomonAbstractView* view = nullptr;

public:
    dtkCoreParameters parameters;

public:
    QMetaObject::Connection connectTime;
    QMetaObject::Connection connectViewDestroyed;
};

//
// gnomonAbstractVisualization_p.h ends here
