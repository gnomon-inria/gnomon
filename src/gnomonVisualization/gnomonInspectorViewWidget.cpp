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
public:

};

// ///////////////////////////////////////////////////////////////////
// gnomonInspectorViewWidget
// ///////////////////////////////////////////////////////////////////

gnomonInspectorViewWidget::gnomonInspectorViewWidget(QWidget *parent) : QScrollArea(parent), d(new gnomonInspectorViewWidgetPrivate)
{
    this->setFrameShape(QFrame::NoFrame);
    this->setWidgetResizable(true);
    this->setMinimumHeight(300);
}

gnomonInspectorViewWidget::~gnomonInspectorViewWidget(void)
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
