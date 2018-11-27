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

#include <QtCore>
#include <QtWidgets>

class gnomonImageManagerItem;

class gnomonImageManagerData : public QObject
{
    Q_OBJECT

public:
     gnomonImageManagerData(QObject *parent = nullptr);
    ~gnomonImageManagerData(void);

public slots:
    QScrollArea *compute(void);

public:
    gnomonImageManagerItem *reference;

public:
    QVariantMap data;

private:
    class gnomonImageManagerDataPrivate *d;
};

//
// gnomonImageManagerData.h ends here
