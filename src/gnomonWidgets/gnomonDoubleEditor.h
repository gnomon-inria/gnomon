// Version: $Id: 80be09c7cc19c2fc912fa461752bfbc2a211d0d6 $
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
#include <QtGui>
#include <QtWidgets>

#include <gnomonWidgetsExport.h>

// /////////////////////////////////////////////////////////////////
// gnomonDoubleEditor
// /////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonDoubleEditor : public QWidget
{
    Q_OBJECT

public:
     gnomonDoubleEditor(QWidget *parent = 0);
    ~gnomonDoubleEditor(void);

    const QString& name(void) const;
    double value(void) const;

    void setName(const QString& name);
    void setValue(double value);
    void setRange(double min, double max);

signals:
    void valueChanged(double val);

private:
    class gnomonDoubleEditorPrivate *d;
};

//
// gnomonDoubleEditor.h ends here
