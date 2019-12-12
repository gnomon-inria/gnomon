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

#include "gnomonViewForm.h"

#include <QtCore>

class GNOMONVISUALIZATION_EXPORT gnomonViewFormPool : public QObject
{
    Q_OBJECT

public:
     gnomonViewFormPool(QObject *parent);
    ~gnomonViewFormPool(void);

public slots:
    void addView(gnomonViewForm *);

public slots:
    void linkAll(void);
    void unlinkAll(void);

private:
    class gnomonViewFormPoolPrivate *d;
};

//
// gnomonViewFormPool.h ends here
