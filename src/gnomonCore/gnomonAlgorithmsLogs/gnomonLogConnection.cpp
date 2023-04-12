#include "gnomonLogConnection.h"

#include <utility>

class gnomonLogConnectionPrivate {
public:
    explicit gnomonLogConnectionPrivate(QTcpSocket *socket, bool *server_alive);
    ~gnomonLogConnectionPrivate();

public:
    QTcpSocket *socket = nullptr;
    QString text;
    bool *server_alive = nullptr;
    bool is_closed = false;
};

gnomonLogConnectionPrivate::gnomonLogConnectionPrivate(QTcpSocket *socket, bool *server_alive):
    socket(socket), text(), server_alive(server_alive) {

}

gnomonLogConnectionPrivate::~gnomonLogConnectionPrivate() {
    // it seems that the socket are already deleted by someone else ..
    //if(*server_alive && socket && socket->isOpen()) {
    //    socket->close();
    //}

    //delete socket;
    socket = nullptr;
}

// --- gnomonLogConnection ---------------------------------------------------------------------------------------------

gnomonLogConnection::gnomonLogConnection(QObject *parent, QTcpSocket *socket, bool *server_alive):
QObject(parent), d(new gnomonLogConnectionPrivate(socket, server_alive)) {
    if(socket){
        connect(d->socket, &QTcpSocket::readyRead, this, &gnomonLogConnection::readConnection);
        connect(d->socket, &QAbstractSocket::errorOccurred, [this] (auto error) {
            if(error != QAbstractSocket::RemoteHostClosedError)
                qWarning() << "socket error: " << error;
            this->close();
        });
        connect(d->socket, &QTcpSocket::disconnected,
                d->socket, &QTcpSocket::deleteLater);
    } else {
        qWarning() << "trying to create a gnomonLogConnection without a socket!";
    }
}

gnomonLogConnection::~gnomonLogConnection() {
    delete d;
    d = nullptr;
}

void gnomonLogConnection::close() {
    d->is_closed = true;
}

bool gnomonLogConnection::isClosed() {
    return d->is_closed;
}

QString gnomonLogConnection::text() {
    return d->text;
}

void gnomonLogConnection::setText(QString txt) {
    d->text = std::move(txt);
    emit this->textChanged();
}

bool gnomonLogConnection::alive() {
    if(d->socket) {
        return d->socket->isOpen();
    }
    return false;
}

void gnomonLogConnection::readConnection() {
    if(this->d && !this->d->is_closed && this->d->server_alive && this->d->socket) {
        QByteArray data = this->d->socket->readAll();
        QString data_string = QString::fromUtf8(data);
        this->d->text.append(data_string);
        emit this->textChanged();
    }
}

