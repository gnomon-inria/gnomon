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

class gnomonAbstractForm;
class gnomonAbstractMatplotlibVisualization;

class GNOMONWIDGETS_EXPORT gnomonViewMatplotlib : public QFrame
{
    Q_OBJECT

public:
     gnomonViewMatplotlib(QWidget *parent = nullptr);
    ~gnomonViewMatplotlib(void);

public:
    void setForm(const QString&, gnomonAbstractForm *, gnomonAbstractMatplotlibVisualization *  = nullptr);

public:
    void addWidget(QWidget *);

public:
    static gnomonViewMatplotlib *downcast(QWidget *widget)
    {
        return dynamic_cast<gnomonViewMatplotlib *>(widget);
    }

signals:
    void formAdded(const QString&);

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

private:
    class gnomonViewMatplotlibPrivate *d;
};

//
// gnomonViewMatplotlib.h ends here
