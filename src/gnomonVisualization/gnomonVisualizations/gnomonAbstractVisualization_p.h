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
};

//
// gnomonAbstractVisualization_p.h ends here
