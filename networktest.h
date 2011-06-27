#ifndef NETWORKTEST_H
#define NETWORKTEST_H

#include <QObject>
#include <QtNetwork/QAbstractSocket>

class NetworkTest : public QObject
{
    Q_OBJECT

public:
    NetworkTest(QObject *parent = 0);

    bool testNetworkConnection(int timeout = 0, const QString &host = QString(), int port = 80);

private slots:
    void onHostFound();
    void onConnected();
    void onError(QAbstractSocket::SocketError error);
};

#endif // NETWORKTEST_H
