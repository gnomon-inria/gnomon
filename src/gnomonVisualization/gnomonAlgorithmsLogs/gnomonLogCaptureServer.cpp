#include "gnomonLogCaptureServer.h"

#include <memory>
#include <QtGui>

#include "gnomonLogConnection"


class gnomonLogCaptureServerPrivate {
    gnomonLogCaptureServerPrivate();
    ~gnomonLogCaptureServerPrivate();

public:
    QTcpServer server;
    QQueue<gnomonLogConnection*> pending_connections;
    QList<QTcpSocket*> sockets;
    static bool alive;
};
bool gnomonLogCaptureServerPrivate::alive = false;

gnomonLogCaptureServerPrivate::gnomonLogCaptureServerPrivate(): server() {
    server.listen(QHostAddress::LocalHost, 54600);
    alive = true;
}

gnomonLogCaptureServerPrivate::~gnomonLogCaptureServerPrivate() {
    alive = false;
    server.close();
    while(!pending_connections.isEmpty()) {
        auto *connection = pending_connections.dequeue();
        delete connection;
    }
}




// --- gnomonLogCaptureServer ------------------------------------------------------------------------------------------

gnomonLogCaptureServer::gnomonLogCaptureServer(QObject *parent): QObject(parent), d() {
    s_instance = this;
    connect(&d->server, &QTcpServer::newConnection, [this]() {
        auto socket = this->d->server.nextPendingConnection();
        auto connection = new gnomonLogConnection(this, socket, &gnomonLogCaptureServerPrivate::alive);
        this->d->pending_connections.enqueue(connection);
    });
}

gnomonLogCaptureServer::~gnomonLogCaptureServer() {
    s_instance = nullptr;
}

gnomonLogConnection *gnomonLogCaptureServer::getPendingConnection() {
    //TODO: check QML ownership
    return d->pending_connections.dequeue();
}

gnomonLogCaptureServer *gnomonLogCaptureServer::instance(void) {
    if(!s_instance) {
        s_instance = new gnomonLogCaptureServer(nullptr);
    }
    return s_instance;
}

#include "gnomonLogCaptureServer.moc"
