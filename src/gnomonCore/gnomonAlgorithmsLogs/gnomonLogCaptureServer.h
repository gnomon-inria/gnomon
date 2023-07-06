#pragma once

#include <QtCore>
#include <QTcpServer>

#include <gnomonCoreExport>


class gnomonLogCaptureServerPrivate;
class gnomonLogConnection;

class GNOMONCORE_EXPORT gnomonLogCaptureServer: public QTcpServer {
    Q_OBJECT
private:
    gnomonLogCaptureServer(QObject *parent);
public:
    virtual ~gnomonLogCaptureServer() override;

    Q_INVOKABLE gnomonLogConnection *getPendingConnection();
    //Q_INVOKABLE void clear();

signals:
    void newPendingLogConnection();

public:
    bool newConnectionAvailable(void);
    static gnomonLogCaptureServer *instance(void);

public:
    QHostAddress address(void);
    unsigned int port(void);
    QString completeAddress(void);

protected:
    void incomingConnection(qintptr handle) override;

private slots:
    void newServerConnectionHandler(void);

private:
    static gnomonLogCaptureServer *s_instance;
    gnomonLogCaptureServerPrivate *d;
};

