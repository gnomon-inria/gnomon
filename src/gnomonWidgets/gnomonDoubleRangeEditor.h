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
// gnomonDoubleRangeEditor
// /////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonDoubleRangeEditor : public QWidget
{
    Q_OBJECT

public:
     gnomonDoubleRangeEditor(QWidget *parent = 0);
    ~gnomonDoubleRangeEditor(void);

    const QString& name(void) const;
    double valueMin(void) const;
    double valueMax(void) const;
    const QList<double>& value(void) const;

    void setName(const QString& name);
    void setValueMin(double value);
    void setValueMax(double value);
    void setRange(double min, double max);

signals:
    void valueMinChanged(double val);
    void valueMaxChanged(double val);

private:
    class gnomonDoubleRangeEditorPrivate *d;
};


//
// gnomonDoubleRangeEditor.h ends here