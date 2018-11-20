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

#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"

#include <gnomonParameter>

#include <QString>
#include <QtWidgets>

template<class T>
class gnomonWorkspaceTemplatePrivate
{
public:
    gnomonWorkspaceTemplatePrivate();
    virtual ~gnomonWorkspaceTemplatePrivate();

public:
    virtual QString workspace() const = 0;
    virtual QStringList keys() const = 0;

public:
    T *command;

public:
    QFormLayout *pane_item_params_layout;

public:
    gnomonOverlayPane* pane(QWidget* parent);
    void configure(QWidget* widget, const QString& algorithm);
};

#include "gnomonWorkspaceTemplate.hpp"
//
// gnomonWorkspace.h ends here
