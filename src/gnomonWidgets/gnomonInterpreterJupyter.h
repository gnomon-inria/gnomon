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

class GNOMONWIDGETS_EXPORT gnomonInterpreterJupyter : public QFrame
{
    Q_OBJECT

public:
     gnomonInterpreterJupyter(QWidget *parent = nullptr);
    ~gnomonInterpreterJupyter(void);

public:
    void output(const QString& result);

public:
    void addWidget(QWidget *);

public:
    static gnomonInterpreterJupyter *downcast(QWidget *widget)
    {
        return dynamic_cast<gnomonInterpreterJupyter *>(widget);
    }

private:
    class gnomonInterpreterJupyterPrivate *d;
};

//
// gnomonInterpreterJupyter.h ends here
