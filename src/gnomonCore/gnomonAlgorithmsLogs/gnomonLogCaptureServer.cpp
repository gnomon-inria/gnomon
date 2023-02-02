#include "gnomonLogCaptureServer.h"

#include "gnomonLogConnection"


class gnomonLogCaptureServerPrivate {

public:
    gnomonLogCaptureServerPrivate();
    ~gnomonLogCaptureServerPrivate();

public:
    //these  2 lists never contains the same connections.
    // first they are in pending then they may be opened, then they are closed
    QList<gnomonLogConnection*> pending_connections; 
    std::list<gnomonLogConnection*> opened_connections;
    static bool alive;
};
bool gnomonLogCaptureServerPrivate::alive = false;

gnomonLogCaptureServerPrivate::gnomonLogCaptureServerPrivate() {
    alive = true;
}

gnomonLogCaptureServerPrivate::~gnomonLogCaptureServerPrivate() {
    alive = false;

    for(auto *conn : pending_connections) {
        delete conn;
    }
    pending_connections.clear();

    for(auto *conn : opened_connections) {
        delete conn;
    }
    opened_connections.clear();
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
    this->d->pending_connections.push_back(connection);

    emit newPendingLogConnection();
}

gnomonLogConnection *gnomonLogCaptureServer::getPendingConnection() {
    //first delete old connections
    d->opened_connections.remove_if([](gnomonLogConnection *conn){
        if(conn->isClosed()) {
            delete conn;
            return true;
        }
        return false;
    });

    //then delete old pending connection
    //necessary if we create connections without calling this method
    // for example, when we cache data, no call to this!
    QList<gnomonLogConnection*>::iterator it = d->pending_connections.begin();
    while (it != d->pending_connections.end()) {
        if ((*it)->isClosed()) {
            delete (*it);
            it = d->pending_connections.erase(it);
        }
        else 
            ++it;
    }

    gnomonLogConnection *connection = nullptr;
    if(d->pending_connections.empty()) {
        auto socket = QTcpServer::nextPendingConnection();
        connection = new gnomonLogConnection(this, socket, &gnomonLogCaptureServerPrivate::alive);
    } else {
        connection = d->pending_connections.takeFirst();
    }
    d->opened_connections.push_back(connection);

    //QQmlEngine::setObjectOwnership(connection, QQmlEngine::CppOwnership);
    return connection;
}

gnomonLogCaptureServer *gnomonLogCaptureServer::instance(void) {
    if(!s_instance) {
        s_instance = new gnomonLogCaptureServer(0);
        if(!s_instance->listen(QHostAddress::LocalHost, 54600)) {
            qWarning() << Q_FUNC_INFO << "Not listening";
        }
        connect(s_instance, &QTcpServer::newConnection, s_instance, &gnomonLogCaptureServer::newServerConnectionHandler);
        connect(s_instance, &QTcpServer::acceptError, [=](QAbstractSocket::SocketError error) {
            qWarning() << Q_FUNC_INFO << "Error:" << error;
        });

        qInfo() << "log server listening on " << s_instance->serverAddress() << ":" << s_instance->serverPort();
    }
    return s_instance;
}

bool gnomonLogCaptureServer::newConnectionAvailable() {
    return !d->pending_connections.empty(); // || d->server.hasPendingConnections();
}

void gnomonLogCaptureServer::incomingConnection(qintptr handle) {
    QTcpServer::incomingConnection(handle);
}

//void gnomonLogCaptureServer::clear() {
//    while(!d->pending_connections.isEmpty()) {
//        auto *connection = d->pending_connections.dequeue();
//        delete connection;
//    }
//}


