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
    Q_PROPERTY(QJsonObject state READ serialize WRITE deserialize NOTIFY stateChanged)

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

    virtual QJsonObject serialize() {
        QJsonObject state;
        state.insert("canBeDestroyed", m_can_be_destroyed);
        return state;
    };
    virtual void deserialize(const QJsonObject &state) {
        m_can_be_destroyed = state.value("canBeDestroyed").toBool();
        emit canBeDestroyedChanged(m_can_be_destroyed);
    };


protected:
    bool m_can_be_destroyed = true;

private:
    QTimer *timer;
};
