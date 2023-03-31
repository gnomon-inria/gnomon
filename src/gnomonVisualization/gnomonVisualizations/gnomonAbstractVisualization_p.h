#include <gnomonVisualizationExport.h>

#include <dtkCore/dtkCoreParameters>


class gnomonAbstractView;


class gnomonAbstractVisualizationPrivate
{

public:
    gnomonAbstractView* view = nullptr;

public:
    dtkCoreParameters parameters;
};

//
// gnomonAbstractVisualization_p.h ends here
