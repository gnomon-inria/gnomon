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

#include <gnomonParameter>

#include <QtCore>
#include <QtWidgets>

class gnomonOverlayPane;

template <typename T>
class gnomonWorkspaceTemplatePrivate
{
public:
             gnomonWorkspaceTemplatePrivate(void);
    virtual ~gnomonWorkspaceTemplatePrivate(void);

public:
    virtual QString workspace(void) const = 0;
    virtual QStringList keys(void) const = 0;

public:
    gnomonOverlayPane *pane(QWidget *parent);
    void configure(QWidget *widget, const QString& algorithm);

public:
    T *command = nullptr;
    QFormLayout *pane_item_params_layout = nullptr;
};

#include "gnomonWorkspaceTemplate_p.tpp"

//
// gnomonWorkspace.h ends here
