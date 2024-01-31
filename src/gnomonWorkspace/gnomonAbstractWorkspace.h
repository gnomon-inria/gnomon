#pragma once

#include <gnomonWorkspaceExport>

#include <QtCore>
#include <gnomonVisualization/gnomonManager/gnomonFormManager.h>

class GNOMONWORKSPACE_EXPORT gnomonAbstractWorkspace : public QObject
{
    Q_OBJECT

public:
    gnomonAbstractWorkspace(QObject *parent = 0) : QObject(parent) {
        connect(this, &QObject::objectNameChanged, [=] () {
            GNOMON_FORM_MANAGER->registerNewWorkspace(uuid());
        });
        connect(GNOMON_FORM_MANAGER, &gnomonFormManager::requestHibernation,
                this, &gnomonAbstractWorkspace::hibernate);
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

    virtual QJsonObject serialize() = 0;
    virtual void deserialize(const QJsonObject &state) = 0;

public slots:
    virtual void wakeUp() {
        GNOMON_FORM_MANAGER->registerWorkspaceWakeup(this->uuid());
        if(!awake) {
            restoreState();
            timer->start();
            deactivated_forms.clear();
        }
        awake = true;
    }

    virtual void hibernate(QString uuid) {
        if(uuid == this->uuid()) {
            timer->stop();
            awake = false;
        }
    };

    virtual void saveState(void) = 0;
    virtual void restoreState(void) = 0;

protected:
    bool m_can_be_destroyed = true;
    bool awake = true;
    QList<std::shared_ptr<gnomonAbstractDynamicForm>> deactivated_forms; // prevent the forms from being outright deleted

private:
    QTimer *timer;
};
