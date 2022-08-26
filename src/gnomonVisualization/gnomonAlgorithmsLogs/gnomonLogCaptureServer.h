#pragma once

#include <QTcpServer>
#include <Qt>

#include <gnomonVisualizationExport>


class gnomonLogCaptureServerPrivate;
class gnomonLogConnection;

class GNOMONVISUALIZATION_EXPORT gnomonLogCaptureServer: public QTcpServer {
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

