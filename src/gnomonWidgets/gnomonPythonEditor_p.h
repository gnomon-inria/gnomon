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

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class dtkMacsHighlighter;
class dtkMacsWidget;
class dtkWidgetsMenuBar;

class gnomonPythonEditorPrivate
{
public:
    dtkMacsWidget *editor = nullptr;
    dtkMacsHighlighter *highlighter = nullptr;

    QHBoxLayout *layout = nullptr;
    dtkWidgetsMenuBar* script_menubar;
};

//
// gnomonPythonEditor_p.h ends here