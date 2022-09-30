#pragma once

#include <QtCore>
#include <QTcpSocket>

#include <gnomonCoreExport>

class gnomonLogConnectionPrivate;

class GNOMONCORE_EXPORT gnomonLogConnection: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString text READ text NOTIFY textChanged);
    Q_PROPERTY(bool alive READ alive)

public:
    gnomonLogConnection() = delete;
    explicit gnomonLogConnection(QObject *parent, QTcpSocket* socket, bool *server_alive);
    virtual ~gnomonLogConnection() override;

signals:
    void textChanged();

public:
    QString text();
    void setText(QString txt);
    bool alive();

private:
    gnomonLogConnectionPrivate *d;
};
