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

public:
    Q_INVOKABLE void close(); // to be called by the UI
    bool isClosed(); // for logCaptureServer to delete old connections
    
signals:
    void textChanged();

public:
    QString text();
    void setText(QString txt);
    bool alive();

private slots:
    void readConnection();

private:
    gnomonLogConnectionPrivate *d;
};
