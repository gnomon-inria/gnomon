
// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:


#include <gnomonVisualizationExport.h>

#include <QtWidgets>

#include <dtkCore/dtkCoreParameter>

#include "gnomonView/gnomonViewMatplotlib.h"


class gnomonAbstractMatplotlibVisualizationPrivate
{

public:
    gnomonViewMatplotlib* view;

public:
    QMap<QString, dtkCoreParameter *> parameters;
};


//
// gnomonAbstractMatplotlibVisualization_p.h ends here