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

#include "gnomonColor/gnomonLookupTable.h"

// /////////////////////////////////////////////////////////////////
// gnomonLookupTableEditor
// /////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonLookupTableEditor : public QWidget
{
    Q_OBJECT

public:
     gnomonLookupTableEditor(QWidget *parent = 0);
    ~gnomonLookupTableEditor(void);

    gnomonLookupTable value(void) const;

    void setValue(gnomonLookupTable value);

signals:
    void valueChanged(gnomonLookupTable);

private:
    class gnomonLookupTableEditorPrivate *d;
};

//
// gnomonLookupTableEditor.h ends here
