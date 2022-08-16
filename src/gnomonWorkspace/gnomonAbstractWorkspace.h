#pragma once

#include <gnomonWorkspaceExport>

#include <QtCore>

class GNOMONWORKSPACE_EXPORT gnomonAbstractWorkspace : public QObject
{
    Q_OBJECT

public:
    gnomonAbstractWorkspace(QObject *parent = 0) : QObject(parent)
    {

    }

    virtual ~gnomonAbstractWorkspace(void)
    {

    }

signals:
    void finished(void);

public slots:
    virtual void export_outputs(void) = 0;
};
