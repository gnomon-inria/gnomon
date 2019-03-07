
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

#include <gnomonCore/gnomonCoreParameter>

#include "gnomonViewMatplotlib.h"


class gnomonAbstractMatplotlibVisualizationPrivate
{

public:
    gnomonViewMatplotlib* view;
    
public:
    QMap<QString, gnomonCoreParameter *> parameters;
};


//
// gnomonAbstractMatplotlibVisualization_p.h ends here