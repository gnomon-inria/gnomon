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
#include <QtWidgets>

// /////////////////////////////////////////////////////////////////////////////
// TODO: Wazza ?
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspacePrivateSignals : public QObject
{
    Q_OBJECT

signals:
    void algorithmChanged(const QString& algorithm);
};

// /////////////////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////////////////

template <typename T> class gnomonWorkspaceTemplatePrivate : public gnomonWorkspacePrivateSignals
{
public:
             gnomonWorkspaceTemplatePrivate(void);
    virtual ~gnomonWorkspaceTemplatePrivate(void);

public:
    virtual QString workspace(void) const = 0;
    virtual QStringList keys(void) const = 0;

public:
    // TODO: Substitute with Menu Framework
    // --
    // gnomonOverlayPane *pane(QWidget *parent);

public:
    void configure(QWidget *widget, const QString& algorithm);

public:
    QString algorithm;

public:
    T *command = nullptr;

public:
    QFormLayout *pane_item_params_layout = nullptr;
};

#include "gnomonWorkspaceTemplate_p.tpp"

//
// gnomonWorkspace.h ends here
