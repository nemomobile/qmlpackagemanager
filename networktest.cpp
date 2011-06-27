#include "networktest.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

NetworkTest::NetworkTest(QObject *parent) :
    QObject(parent)
{

}

bool NetworkTest::testNetworkConnection(int timeout, const QString &host, int port)
{
    qDebug() << Q_FUNC_INFO << timeout << host << port;

    if (host.isEmpty()) {
        if (testNetworkConnection(10000, "repo.meego.com"))
            return true;
        return testNetworkConnection(5000, "www.google.com");
    }

    QTcpSocket *s = new QTcpSocket(this);
    connect(s, SIGNAL(hostFound()), this, SLOT(onHostFound()));
    connect(s, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(s, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onError(QAbstractSocket::SocketError)));
    s->connectToHost(host, port);
    return s->waitForConnected(timeout);
}

void NetworkTest::onHostFound()
{
//    qDebug() << Q_FUNC_INFO;
}

void NetworkTest::onConnected()
{
//    qDebug() << Q_FUNC_INFO;
}

void NetworkTest::onError(QAbstractSocket::SocketError error)
{
    qDebug() << Q_FUNC_INFO << error;
}
