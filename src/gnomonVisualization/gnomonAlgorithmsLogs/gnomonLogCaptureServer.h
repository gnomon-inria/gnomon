#pragma once

#include <memory>
#include <QTcpServer>
#include <Qt>

#include <gnomonVisualizationExport>


class gnomonLogCaptureServerPrivate;
class gnomonLogConnection;

class GNOMONVISUALIZATION_EXPORT gnomonLogCaptureServer: public QObject {
    Q_OBJECT
private:
    gnomonLogCaptureServer(QObject *parent);
public:
    virtual ~gnomonLogCaptureServer() override;

public slots:
    gnomonLogConnection *getPendingConnection();

public:
    static gnomonLogCaptureServer *instance(void);

private:
    static gnomonLogCaptureServer *s_instance;
    gnomonLogCaptureServerPrivate *d;
};

