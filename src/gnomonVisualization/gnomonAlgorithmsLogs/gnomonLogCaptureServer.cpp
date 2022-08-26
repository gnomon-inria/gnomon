#include "gnomonLogCaptureServer.h"

#include <QtGui>

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
    if(!QTcpServer::listen(QHostAddress::LocalHost, 54601)) {
        qDebug() << Q_FUNC_INFO << "Not listening";
    }
    connect(this, &QTcpServer::newConnection, this, &gnomonLogCaptureServer::newServerConnectionHandler);
    connect(this, &QTcpServer::acceptError, [=](QAbstractSocket::SocketError error) {
        qDebug() << Q_FUNC_INFO << error;
    });
    qDebug() << Q_FUNC_INFO << "New connection handler initialized";
}

gnomonLogCaptureServer::~gnomonLogCaptureServer() {
    QTcpServer::close();
    s_instance = nullptr;
    delete d;
}

gnomonLogCaptureServer *gnomonLogCaptureServer::s_instance = nullptr;

void gnomonLogCaptureServer::newServerConnectionHandler(void) {
    qDebug() << Q_FUNC_INFO << " ========= Receiving new connection";
    auto socket = QTcpServer::nextPendingConnection();
    auto connection = new gnomonLogConnection(this, socket, &gnomonLogCaptureServerPrivate::alive);
    this->d->pending_connections.enqueue(connection);
    emit newConnection();
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
    return connection;
}

gnomonLogCaptureServer *gnomonLogCaptureServer::instance(void) {
    if(!s_instance) {
        s_instance = new gnomonLogCaptureServer(0);
    }
    return s_instance;
}

bool gnomonLogCaptureServer::newConnectionAvailable() {
    return !d->pending_connections.empty(); // || d->server.hasPendingConnections();
}

void gnomonLogCaptureServer::incomingConnection(qintptr handle) {
    QTcpServer::incomingConnection(handle);
}


