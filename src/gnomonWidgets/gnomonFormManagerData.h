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

class gnomonFormManagerItem;

class gnomonFormManagerData : public QObject
{
    Q_OBJECT

public:
     gnomonFormManagerData(QObject *parent = nullptr);
    ~gnomonFormManagerData(void);

public slots:
    QScrollArea *compute(void);

public:
    gnomonFormManagerItem *reference;

public:
    QMap<QString,QString> data;

private:
    class gnomonFormManagerDataPrivate *d;
};

//
// gnomonFormManagerData.h ends here
