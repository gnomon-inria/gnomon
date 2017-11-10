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

#include "gnomonInspectorViewWidget.h"

#include "gnomonActor.h"
#include "gnomonActorMesh.h"
#include "gnomonActorVolume.h"

#include "gnomonInspector.h"
#include "gnomonInspectorVolume.h"
#include "gnomonInspectorMesh.h"
#include "gnomonActorMeshCellComplex.h"
#include "gnomonActorMeshCellGraph.h"

#include "gnomonClutEditor.h"

#include <QStackedWidget>

#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkSmartPointer.h>
#include <vtkImageAccumulate.h>

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidgetPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInspectorViewWidgetPrivate
{
public:
    QObject * inspector_parent;
};

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidget
// ///////////////////////////////////////////////////////////////////

gnomonInspectorViewWidget::gnomonInspectorViewWidget(QWidget *parent) : QScrollArea(parent), d(new gnomonInspectorViewWidgetPrivate)
{
    this->setWidgetResizable(true);
    this->setMinimumHeight(400);
}

gnomonInspectorViewWidget::~gnomonInspectorViewWidget()
{
    delete d;
}

void gnomonInspectorViewWidget::setInspector(gnomonInspector *inspector, bool enabled)
{
    if(inspector == nullptr) {
        return;
    }

    this->takeWidget();
    this->setWidget(inspector);
    inspector->setEnabled(enabled);

    return;
}

//
// gnomonInspectorViewWidget.cpp ends here
