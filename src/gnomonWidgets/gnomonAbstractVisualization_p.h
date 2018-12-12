
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


#include <gnomonWidgetsExport.h>

#include <QtWidgets>

#include <gnomonCore/gnomonCoreParameter>

#include "gnomonViewForm.h"


class gnomonAbstractVisualizationPrivate
{
public:
    gnomonViewForm* view;
    
public:
    QMap<QString, gnomonCoreParameter *> parameters;

public:
    QMetaObject::Connection connectSliceOrientation;
    QMetaObject::Connection connectSlice;
};


//
// gnomonAbstractVisualization_p.h ends here