#include "gnomonLogConnection.h"

#include <utility>

class gnomonLogConnectionPrivate {
public:
    explicit gnomonLogConnectionPrivate(QTcpSocket *socket, bool *server_alive);
    ~gnomonLogConnectionPrivate();

public:
    QTcpSocket *socket;
    QString text;
    bool *server_alive = nullptr;
};

gnomonLogConnectionPrivate::gnomonLogConnectionPrivate(QTcpSocket *socket, bool *server_alive):
    socket(socket), text(), server_alive(server_alive) {

}

gnomonLogConnectionPrivate::~gnomonLogConnectionPrivate() {
    if(*server_alive) {
        socket->close();
    }
}

// --- gnomonLogConnection ---------------------------------------------------------------------------------------------

gnomonLogConnection::gnomonLogConnection(QObject *parent, QTcpSocket *socket, bool *server_alive):
QObject(parent), d(new gnomonLogConnectionPrivate(socket, server_alive)) {
    connect(d->socket, &QTcpSocket::readyRead, [this]() {
        if(this->d->server_alive) {
            QByteArray data = this->d->socket->readAll();
            QString data_string = QString::fromUtf8(data);
            qDebug() << Q_FUNC_INFO << "Received : " << data_string;
            this->d->text.append(data_string);
            emit this->textChanged();
        }
    });
    connect(d->socket, &QAbstractSocket::errorOccurred, [=] (auto error) {
        qDebug() << "Socket Error: " << error;
    });
    connect(d->socket, &QTcpSocket::disconnected,
            d->socket, &QTcpSocket::deleteLater);

}

gnomonLogConnection::~gnomonLogConnection() {
    // do not delete socket
    delete d;
}

QString gnomonLogConnection::text() {
    return d->text;
}

void gnomonLogConnection::setText(QString txt) {
    d->text = std::move(txt);
    emit this->textChanged();
}

