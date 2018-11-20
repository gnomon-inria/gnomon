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

#include <QtWidgets>

class gnomonViewVolumic;

class gnomonGridLayout : public QGridLayout
{
    Q_OBJECT

public:
     gnomonGridLayout(QWidget *parent = nullptr);
    ~gnomonGridLayout(void);

public slots:
    void addView(void);

public:
    QList<gnomonViewVolumic *> views(void);

protected:
    void clear(void);
    void update(void);

private:
    class gnomonGridLayoutPrivate *d;
};

//
// gnomonGridLayout.h ends here
