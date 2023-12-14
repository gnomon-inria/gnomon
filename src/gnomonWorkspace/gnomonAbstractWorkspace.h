#pragma once

#include <gnomonWorkspaceExport>

#include <QtCore>

class GNOMONWORKSPACE_EXPORT gnomonAbstractWorkspace : public QObject
{
    Q_OBJECT

public:
    gnomonAbstractWorkspace(QObject *parent = 0) : QObject(parent) {
        timer = new QTimer(this);
        timer->setInterval(1000);
        connect(timer, &QTimer::timeout, this, &gnomonAbstractWorkspace::stateChanged);
        timer->start();
    }
    virtual ~gnomonAbstractWorkspace(void) {
        delete timer;
    }

    Q_PROPERTY(bool canBeDestroyed READ canBeDestroyed NOTIFY canBeDestroyedChanged);
    Q_PROPERTY(QString uuid READ uuid CONSTANT) // a read-only alias for objectName
    Q_PROPERTY(QJsonObject state READ serialize WRITE unSerialize NOTIFY stateChanged)

signals:
    void started(void);
    void finished(bool success=true);
    void canBeDestroyedChanged(bool canBeDestroyed);
    void stateChanged();

public slots:
    virtual void export_outputs(void) = 0;

public:
    bool canBeDestroyed(void) { return this->m_can_be_destroyed; };
    QString uuid() { return objectName(); };

    virtual QJsonObject serialize() = 0;
    virtual void unSerialize(const QJsonObject &state) = 0;


protected:
    bool m_can_be_destroyed = true;

private:
    QTimer *timer;
};
