#include "networktest.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <QDebug>

NetworkTest::NetworkTest(QObject *parent) :
    QObject(parent)
{

}

// test if there is a IPv4 address available
bool NetworkTest::hasNetworkConnection()
{
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();

    foreach(QNetworkInterface iface, ifaces) {
        if ( iface.flags().testFlag(QNetworkInterface::IsUp)
             && !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
            foreach (QNetworkAddressEntry entry, iface.addressEntries())
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
                    return true;
    }

    return false;
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
