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

#include "gnomonFormAdapterMenu.h"

#include <dtkThemes>
#include <dtkWidgets>

gnomonFormAdapterMenu::gnomonFormAdapterMenu(QVariantMap adapter_descs, QWidget *parent) : QQuickWidget(parent)
{
    this->engine()->addImportPath("qrc:/");

    QQmlContext *context = this->rootContext();
    context->setContextProperty("font", dtkFontAwesome::instance());
    context->setContextProperty("theme", dtkThemesEngine::instance());
    context->setContextProperty("adapter_descs", adapter_descs);

    this->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->setSource(QUrl("qrc:/gnomonFormAdapter.qml"));
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFocus(Qt::PopupFocusReason);
}


//
// gnomonFormAdapterMenu.cpp ends here