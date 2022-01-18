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

#include <gnomonWorkspaceExport>

#include <QtCore>

class gnomonViewForm;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceBrowser : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("description", "Browsing Workspace description from cxx")

public:
     gnomonWorkspaceBrowser(QObject *parent = nullptr);
    ~gnomonWorkspaceBrowser(void);

public:
    Q_PROPERTY(gnomonViewForm* view READ view);

signals:
    void available(const QVariantMap& readers);

public slots:
    void read    (const QString&);
    void readWith(const QString&);

public:
    Q_INVOKABLE QUrl defaultReadPath();
    Q_INVOKABLE gnomonViewForm *view(void);

private:
    class gnomonWorkspaceBrowserPrivate *d;
};

//
// gnomonWorkspaceBrowser.h ends here
