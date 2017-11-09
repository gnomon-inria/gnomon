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

#pragma once

#include <gnomonVisualizationExport.h>

#include <QStackedWidget>

class gnomonActor;
class gnomonInspectorViewWidgetPrivate;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorViewWidget : public QStackedWidget
{
    Q_OBJECT

public:
     gnomonInspectorViewWidget(void);
    ~gnomonInspectorViewWidget(void);

public slots:
    void setActor(gnomonActor *actor, bool enabled);

public:
    gnomonInspectorViewWidgetPrivate *d;
};

//
// gnomonInspectorViewWidget.h ends here
