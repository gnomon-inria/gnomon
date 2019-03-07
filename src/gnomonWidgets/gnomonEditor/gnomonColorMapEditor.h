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
// gnomonColorMapEditor
// /////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonColorMapEditor : public QWidget
{
    Q_OBJECT

public:
     gnomonColorMapEditor(QWidget *parent = 0);
    ~gnomonColorMapEditor(void);

    const QString& name(void) const;
    const QMap<double, QColor>& value(void) const;

    void setName(const QString& name);
    void setValue(const QMap<double, QColor>& value);
    void setValue(const QString& value);

signals:
    void valueChanged(const QMap<double, QColor>& val);
    
public slots:
    void importColorMap(const QString& clut);

private:
    class gnomonColorMapEditorPrivate *d;
};

//
// gnomonColorMapEditor.h ends here
