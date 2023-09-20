#pragma once

#include <gnomonWorkspaceExport>

#include <QtCore>

class GNOMONWORKSPACE_EXPORT gnomonAbstractWorkspace : public QObject
{
    Q_OBJECT

public:
    gnomonAbstractWorkspace(QObject *parent = 0) : QObject(parent) { }
    virtual ~gnomonAbstractWorkspace(void) { }

    Q_PROPERTY(bool canBeDestroyed READ canBeDestroyed NOTIFY canBeDestroyedChanged);
signals:
    void started(void);
    void finished(bool success=true);
    void canBeDestroyedChanged(bool canBeDestroyed);

public slots:
    virtual void export_outputs(void) = 0;

public:
    bool canBeDestroyed(void) { return this->m_can_be_destroyed; };

protected:
    bool m_can_be_destroyed = true;
};
