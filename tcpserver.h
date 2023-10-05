#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include "tcpsocketconnection.h"
#include <QObject>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
signals:
    void TcpConnected(TcpSocketConnection * con);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // TCPSERVER_H
