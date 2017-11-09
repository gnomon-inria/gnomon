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

#include <QSplitter>

class gnomonInspectorMainPrivate;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorMain : public QSplitter {
    Q_OBJECT
 public:
    gnomonInspectorMain();
    ~gnomonInspectorMain();

 public:
    gnomonInspectorMainPrivate *d;
};

//
// gnomonInspectorMain.h ends here
