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

#include <gnomonWidgetsExport>

#include <QtWidgets>

class gnomonViewVolumic;

class GNOMONWIDGETS_EXPORT gnomonGridLayout : public QGridLayout
{
    Q_OBJECT

public:
     gnomonGridLayout(QWidget *parent = nullptr);
    ~gnomonGridLayout(void);

public slots:
    void addView(void);
	void removeAllViews(void);

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
