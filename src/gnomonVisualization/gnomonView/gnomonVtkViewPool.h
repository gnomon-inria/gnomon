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

#include <gnomonVisualizationExport>

#include "gnomonVtkView.h"

#include <QtCore>

class GNOMONVISUALIZATION_EXPORT gnomonVtkViewPool : public QObject
{
    Q_OBJECT

public:
     gnomonVtkViewPool(QObject *parent);
    ~gnomonVtkViewPool(void);

public slots:
    void addView(gnomonVtkView *);

public slots:
    void linkAll(void);
    void unlinkAll(void);

public:
    bool isLinked(void);

private:
    class gnomonVtkViewPoolPrivate *d;
};

//
// gnomonVtkViewPool.h ends here
