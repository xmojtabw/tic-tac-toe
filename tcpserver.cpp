#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent)
    : QTcpServer{parent}
{

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"in descriptor";
    QThread * thread = new QThread;
    TcpSocketConnection * c = new TcpSocketConnection(socketDescriptor);
    c->moveToThread(thread);
    connect(thread,&QThread::started,
            c,&TcpSocketConnection::startManage);
    thread->start();
    qDebug()<<"going to emit Tcp connected";
    emit TcpConnected(c);

}
