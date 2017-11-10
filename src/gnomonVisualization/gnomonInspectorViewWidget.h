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

#include <QScrollArea>

class gnomonActor;
class gnomonInspector;

class gnomonInspectorViewWidgetPrivate;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorViewWidget : public QScrollArea
{
    Q_OBJECT

public:
     gnomonInspectorViewWidget(QWidget *parent = nullptr);
    ~gnomonInspectorViewWidget();

public slots:
    void setActor(gnomonActor *actor, bool enabled);

    void setInspector(gnomonInspector *inspector, bool enabled);

public:
    gnomonInspectorViewWidgetPrivate *d;
};

//
// gnomonInspectorViewWidget.h ends here
