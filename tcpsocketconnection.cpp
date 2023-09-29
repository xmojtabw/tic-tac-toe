#include "tcpsocketconnection.h"


TcpSocketConnection::TcpSocketConnection
(QTcpSocket *c, bool is_s, QString n):
    connection(c),name(n),is_server(is_s),writing(false)
{
   if(is_server)
   {
       serverIp=connection->localAddress().toString();
       serverPort=connection->localPort();
       serverName=name;
       clientIp=connection->peerAddress().toString();
       clientPort=connection->peerPort();

       smh = new serverMessageHandeler(c);
       readTimer = new QTimer();
       readTimer->setInterval(200);
       readTimer->setSingleShot(false);
       connect(readTimer,&QTimer::timeout,
               this,&TcpSocketConnection::checkForNewMessages);
       readTimer->start();



   }
   else
   {
       clientIp=connection->localAddress().toString();
       clientPort=connection->localPort();
       clientName=name;
       serverIp=connection->peerAddress().toString();
       serverPort=connection->peerPort();

       connect(c,&QTcpSocket::readyRead,
               this,&TcpSocketConnection::handleNewMessages);
   }


    connect(c,&QTcpSocket::disconnected,
            this,&TcpSocketConnection::handleDisconnect);
    connect(c,&QTcpSocket::bytesWritten,
            this,&TcpSocketConnection::sendConfirm);


    send_timeout = new QTimer();
    send_timeout->setSingleShot(true);
    send_timeout->setInterval(300);
    connect(send_timeout,&QTimer::timeout,
            this,&TcpSocketConnection::sendTimeout);

}

void TcpSocketConnection::sendMessage(QString t, QString m,QString s)
{
    //if sender is not defined we use local name as sender
    Message msg(m,s=="n"?name:s,t);


    if(send_timeout->isActive()||writing)
    {
        draft_messages.push_front(msg);

    }
    else
    {
        QByteArray sending_buffer=msg.to_string().c_str();
        QByteArray framedmessage;
        QDataStream stream(&framedmessage, QIODevice::WriteOnly);
        stream << static_cast<quint32>(sending_buffer.size());
        framedmessage.append(sending_buffer);
        send_timeout->start();
        connection->write(framedmessage);
    }
}
void TcpSocketConnection::dequeue_drafts(Message msg)
{
    writing=true;
    QByteArray sending_buffer=msg.to_string().c_str();
    QByteArray framedmessage;
    QDataStream stream(&framedmessage, QIODevice::WriteOnly);
    stream << static_cast<quint32>(sending_buffer.size());
    framedmessage.append(sending_buffer);
    send_timeout->start();
    connection->write(framedmessage);
}

TcpSocketConnection::~TcpSocketConnection()
{
//    if(is_server)
//        delete smh;
//    connection->deleteLater();
//    send_timeout->deleteLater();
//    readTimer->deleteLater();
}

void TcpSocketConnection::checkForNewMessages()
{
    recieved_messages.append(smh->getMessages());
    while(recieved_messages.size())
    {
        qDebug()<<recieved_messages.size();
        Message pending_emit=recieved_messages.first();
        recieved_messages.pop_front();
        emit newEvent(pending_emit,this);
        if(!recieved_messages.size())break;
        QThread::msleep(50);//for not emit fastly

    }
}


void TcpSocketConnection::setName(QString n,bool cli)
{cli?clientName=n:serverName = n;}
QString TcpSocketConnection::getName(bool cli)
{return cli?clientName:serverName;}
QString TcpSocketConnection::getIp(bool cli)
{return cli?clientIp:serverIp;}
quint16 TcpSocketConnection::getPort(bool cli)
{return (cli?clientPort:serverPort);}

void TcpSocketConnection::handleNewMessages()
{


    {
        QByteArray buffer=connection->readAll();
        //----------------------//
        QByteArray received_data;
        received_data.append(buffer);
        int size=sizeof(quint32);
        while(received_data.size()>=size)
        {
            QDataStream stream(&received_data, QIODevice::ReadOnly);
            quint32 messageSize;
            stream >> messageSize;
            if (received_data.size() >= size+ messageSize)
            {
                QByteArray messageData = received_data.mid(size, messageSize);
                received_data = received_data.mid(size + messageSize);

                // Process the messageData here
//                qDebug()<<"message data :"<<messageData;
//                qDebug()<<"rec data"<<received_data;
                emit newEvent(Message(messageData),this);
            }
            else
            {
                // Not enough data for a complete message yet
                break;
            }
        }
    }





}

void TcpSocketConnection::handleDisconnect()
{
    if(is_server)
    {
        smh->stopThread();
    }
    qDebug()<<"disconnect";
    emit newEvent(Message("",clientName,"disconnected"),this);

}

void TcpSocketConnection::sendConfirm(qint64 Bytes)
{

    send_timeout->stop();
    if(draft_messages.size())
    {
       //has some messages
        Message temp=draft_messages.back();
        draft_messages.pop_back();
        dequeue_drafts(temp);

    }
    else
    {
        //queue is empty
        writing=false;
    }
}

void TcpSocketConnection::sendTimeout()
{
    qDebug()<<"send time out";
}

