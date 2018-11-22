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

#include "gnomonViewVolumic.h"

#include <QtCore>

class gnomonViewVolumicPool : public QObject
{
    Q_OBJECT

public:
     gnomonViewVolumicPool(QObject *parent);
    ~gnomonViewVolumicPool(void);

public slots:
    void addView(gnomonViewVolumic *);

private:
    class gnomonViewVolumicPoolPrivate *d;
};

//
// gnomonViewVolumicPool.h ends here
