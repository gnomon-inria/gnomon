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

#include "gnomonAbstractVisualization.h"
#include "gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore/gnomonMesh>
#include <gnomonCore/gnomonCoreParameter>

#include "gnomonViewForm.h"

#include "gnomonPolyDataMesh.h"
#include "gnomonActorPolyData.h"
#include "gnomonActor2DPolyData.h"

#include <vtkRenderer.h>


// /////////////////////////////////////////////////////////////////
// gnomonAbstractVisualization
// /////////////////////////////////////////////////////////////////

gnomonAbstractVisualization::gnomonAbstractVisualization(gnomonViewForm* view) : d(new gnomonAbstractVisualizationPrivate)
{
    d->view = view;
}

gnomonAbstractVisualization::~gnomonAbstractVisualization(void)
{
    delete d;

    d = NULL;
}

QMap<QString, gnomonCoreParameter *> gnomonAbstractVisualization::parameters(void) const
{
    return d->parameters;
}

void gnomonAbstractVisualization::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

//
// gnomonAbstractVisualization.cpp ends here
