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

#include <gnomonVisualizationExport.h>

// /////////////////////////////////////////////////////////////////
// gnomonStringListEditor
// /////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonStringListEditor : public QWidget
{
    Q_OBJECT

public:
     gnomonStringListEditor(QWidget *parent = 0);
    ~gnomonStringListEditor(void);

    const QString& name(void) const;
    const QStringList& value(void) const;

    void setName(const QString& name);
    void setValue(const QStringList& value);
    void setValues(const QStringList& values);

signals:
    void valueChanged(const QStringList& val);

private:
    class gnomonStringListEditorPrivate *d;
};


//
// gnomonStringListEditor.h ends here