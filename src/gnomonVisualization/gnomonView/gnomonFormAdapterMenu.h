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

#include <gnomonVisualizationExport>

#include <QtWidgets>

#include <QtQuick>
#include <QtQuickWidgets>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonFormAdapterMenu : public QQuickWidget
{
    Q_OBJECT

public:
    gnomonFormAdapterMenu(QVariantMap, QWidget *parent = nullptr);

protected:
    void focusOutEvent(QFocusEvent *event) override
    {
        this->close();
        this->deleteLater();

        QQuickWidget::focusOutEvent(event);
    }
};

//
// gnomonFormAdapterMenu.h ends here