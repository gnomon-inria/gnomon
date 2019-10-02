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
// 
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

class gnomonWorkspace;

template <typename T> class gnomonWorkspaceTemplatePrivate : public gnomonWorkspacePrivateSignals
{
public:
             gnomonWorkspaceTemplatePrivate(void);
    virtual ~gnomonWorkspaceTemplatePrivate(void);

public:
    virtual QString workspace(void) const = 0;
    virtual QStringList keys(void) const = 0;

public:
    dtkWidgetsMenu *menu(dtkWidgetsWorkspace *);

public:
    void configure(const QString& algorithm);

public:
    QString algorithm;

public:
    T *command = nullptr;

public:
    QFormLayout *layout = nullptr;
};

#include "gnomonWorkspaceTemplate_p.tpp"

//
// gnomonWorkspace.h ends here
