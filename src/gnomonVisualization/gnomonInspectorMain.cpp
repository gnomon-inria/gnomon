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

#include "gnomonInspectorMain.h"

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidgetPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInspectorMainPrivate
{
public:

};

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidget
// ///////////////////////////////////////////////////////////////////

gnomonInspectorMain::gnomonInspectorMain(QWidget *parent) : QSplitter(Qt::Vertical, parent), d(new gnomonInspectorMainPrivate)
{
    this->setHandleWidth(2);
}

gnomonInspectorMain::~gnomonInspectorMain(void)
{

}

//
// gnomonInspectorMain.cpp ends here
