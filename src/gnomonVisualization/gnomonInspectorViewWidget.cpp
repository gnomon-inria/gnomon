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

#include "gnomonInspectorViewWidget.h"

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidgetPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInspectorViewWidgetPrivate
{

};

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidget
// ///////////////////////////////////////////////////////////////////

gnomonInspectorViewWidget::gnomonInspectorViewWidget(QWidget *parent) : QScrollArea(parent), d(new gnomonInspectorViewWidgetPrivate)
{
    this->setWidgetResizable(true);
    this->setMinimumHeight(200);
}

gnomonInspectorViewWidget::~gnomonInspectorViewWidget()
{
    delete d;
}

void gnomonInspectorViewWidget::setInspector(QWidget *inspector)
{
    if(inspector == nullptr)
        return;

    this->setWidget(inspector);

    return;
}

//
// gnomonInspectorViewWidget.cpp ends here
