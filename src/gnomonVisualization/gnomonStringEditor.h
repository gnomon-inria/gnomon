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
// gnomonStringEditor
// /////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonStringEditor : public QWidget
{
    Q_OBJECT

public:
     gnomonStringEditor(QWidget *parent = 0);
    ~gnomonStringEditor(void);

    const QString& name(void) const;
    const QString& value(void) const;

    void setName(const QString& name);
    void setValue(const QString& value);
    void setList(const QStringList& list);

signals:
    void valueChanged(const QString& val);

private:
    class gnomonStringEditorPrivate *d;
};


//
// gnomonStringEditor.h ends here