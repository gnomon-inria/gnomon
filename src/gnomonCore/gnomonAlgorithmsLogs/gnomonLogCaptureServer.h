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

public slots:
    gnomonLogConnection *getPendingConnection();

public:
    bool newConnectionAvailable(void);
    static gnomonLogCaptureServer *instance(void);

protected:
    void incomingConnection(qintptr handle) override;

private slots:
    void newServerConnectionHandler(void);

private:
    static gnomonLogCaptureServer *s_instance;
    gnomonLogCaptureServerPrivate *d;
};

