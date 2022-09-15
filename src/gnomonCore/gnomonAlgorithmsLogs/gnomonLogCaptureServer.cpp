#include "gnomonLogCaptureServer.h"

#include "gnomonLogConnection"


class gnomonLogCaptureServerPrivate {

public:
    gnomonLogCaptureServerPrivate();
    ~gnomonLogCaptureServerPrivate();

public:
    QQueue<gnomonLogConnection*> pending_connections;
    static bool alive;
};
bool gnomonLogCaptureServerPrivate::alive = false;

gnomonLogCaptureServerPrivate::gnomonLogCaptureServerPrivate() {
    alive = true;
}

gnomonLogCaptureServerPrivate::~gnomonLogCaptureServerPrivate() {
    alive = false;
    while(!pending_connections.isEmpty()) {
        auto *connection = pending_connections.dequeue();
        delete connection;
    }
}

// --- gnomonLogCaptureServer ------------------------------------------------------------------------------------------

gnomonLogCaptureServer::gnomonLogCaptureServer(QObject *parent): QTcpServer(parent), d(new gnomonLogCaptureServerPrivate()) {
}

gnomonLogCaptureServer::~gnomonLogCaptureServer() {
    this->close();
    s_instance = nullptr;
    delete d;
}

gnomonLogCaptureServer *gnomonLogCaptureServer::s_instance = nullptr;

void gnomonLogCaptureServer::newServerConnectionHandler(void) {
    auto socket = QTcpServer::nextPendingConnection();
    auto connection = new gnomonLogConnection(this, socket, &gnomonLogCaptureServerPrivate::alive);
    this->d->pending_connections.enqueue(connection);
    emit newPendingLogConnection();
}

gnomonLogConnection *gnomonLogCaptureServer::getPendingConnection() {
    //TODO: check QML ownership
    gnomonLogConnection *connection = nullptr;
    if(d->pending_connections.empty()) {
        auto socket = QTcpServer::nextPendingConnection();
        connection = new gnomonLogConnection(this, socket, &gnomonLogCaptureServerPrivate::alive);
    } else {
        connection = d->pending_connections.dequeue();
    }
    //QQmlEngine::setObjectOwnership(connection, QQmlEngine::CppOwnership);
    return connection;
}

gnomonLogCaptureServer *gnomonLogCaptureServer::instance(void) {
    if(!s_instance) {
        s_instance = new gnomonLogCaptureServer(0);
        if(!s_instance->listen(QHostAddress::LocalHost, 54600)) {
            qDebug() << Q_FUNC_INFO << "Not listening";
        }
        connect(s_instance, &QTcpServer::newConnection, s_instance, &gnomonLogCaptureServer::newServerConnectionHandler);
        connect(s_instance, &QTcpServer::acceptError, [=](QAbstractSocket::SocketError error) {
            qDebug() << Q_FUNC_INFO << "Error:" << error;
        });

        qDebug() << "log server listening on " << s_instance->serverAddress() << ":" << s_instance->serverPort();
    }
    return s_instance;
}

bool gnomonLogCaptureServer::newConnectionAvailable() {
    return !d->pending_connections.empty(); // || d->server.hasPendingConnections();
}

void gnomonLogCaptureServer::incomingConnection(qintptr handle) {
    QTcpServer::incomingConnection(handle);
}


