#include "tcpsocketconnection.h"


TcpSocketConnection::TcpSocketConnection
(QTcpSocket *c, bool is_s, QString n):
    connection(c),name(n),is_server(false),writing(false)
{
    //this constructor is for clinet;

       clientIp=connection->localAddress().toString();
       clientPort=connection->localPort();
       clientName=name;
       serverIp=connection->peerAddress().toString();
       serverPort=connection->peerPort();

       send_timeout = new QTimer();
       send_timeout->setSingleShot(true);
       send_timeout->setInterval(300);
       connect(send_timeout,&QTimer::timeout,
               this,&TcpSocketConnection::sendTimeout);



   connect(c,&QTcpSocket::readyRead,
           this,&TcpSocketConnection::handleNewMessages);

   connect(this,&TcpSocketConnection::NewDraftMessage,
           this,&TcpSocketConnection::dequeueDraftMessages);

    connect(c,&QTcpSocket::disconnected,
            this,&TcpSocketConnection::handleDisconnect);
    connect(c,&QTcpSocket::bytesWritten,
            this,&TcpSocketConnection::sendConfirm);

}

TcpSocketConnection::TcpSocketConnection(qintptr socketDescriptor,QString n):
    //constructor for server
        name(n),socketDescriptor(socketDescriptor),
        is_server(true),writing(false)
{}
void TcpSocketConnection::sendMessage(QString t, QString m,QString s)
{
    //if sender is not defined we use local name as sender
    Message msg(m,s=="n"?name:s,t);
    draft_message_lock.lock();
    draft_messages.push_back(msg);
    draft_message_lock.unlock();
    if(!(send_timeout->isActive()||writing))
    {
        //the socket is free now you can emit signal to send messages on its thread
        emit NewDraftMessage();
    }
}


TcpSocketConnection::~TcpSocketConnection()
{
    // emit signal to stop thread
}
void TcpSocketConnection::setName(QString n,bool cli)
{cli?clientName=n:serverName = n;}
QString TcpSocketConnection::getName(bool cli)
{return cli?clientName:serverName;}
QString TcpSocketConnection::getIp(bool cli)
{return cli?clientIp:serverIp;}
quint16 TcpSocketConnection::getPort(bool cli)
{return (cli?clientPort:serverPort);}
QList<Message> TcpSocketConnection::getMessages()
{
    recieved_message_lock.lock();
    QList<Message> tmp = recieved_messages;
    recieved_messages.clear();
    recieved_message_lock.unlock();
    return tmp;
}
void TcpSocketConnection::handleNewMessages()
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
                if(is_server)
                {
                    recieved_message_lock.lock();
                    recieved_messages.push_back(Message(messageData));
                    recieved_message_lock.unlock();
                }
                else
                {
                    emit newEvent(Message(messageData),this);
                }

            }
            else
            {
                // Not enough data for a complete message yet
                break;
            }
        }
}
void TcpSocketConnection::handleDisconnect()
{
//    if(is_server)
//    {
//        smh->stopThread();
//    }
    if(is_server)
    {
        recieved_message_lock.lock();
        recieved_messages.push_back(Message("",clientName,"disconnected"));
        recieved_message_lock.unlock();
    }
    else
    {
        emit newEvent(Message("",clientName,"disconnected"),this);
    }
    qDebug()<<"disconnect";


}
void TcpSocketConnection::sendConfirm(qint64 Bytes)
{

    send_timeout->stop();
    if(draft_messages.size())
    {
       //has some messages
        dequeueDraftMessages();
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
void TcpSocketConnection::dequeueDraftMessages()
{
    if(draft_messages.size())
    {
        writing=true;


        draft_message_lock.lock();
        Message msg = draft_messages.first();
        draft_messages.pop_front();
        draft_message_lock.unlock();


        QByteArray sending_buffer=msg.to_string().c_str();
        QByteArray framedmessage;
        QDataStream stream(&framedmessage, QIODevice::WriteOnly);
        stream << static_cast<quint32>(sending_buffer.size());
        framedmessage.append(sending_buffer);
        send_timeout->start();
        connection->write(framedmessage);
    }
    else
    {
        writing = false;
    }
}
void TcpSocketConnection::startManage()
{
    qDebug()<<"start message has been called";

    connection = new QTcpSocket();
    connection->setSocketDescriptor(socketDescriptor);



    connect(connection,&QTcpSocket::readyRead,
            this,&TcpSocketConnection::handleNewMessages);

    connect(this,&TcpSocketConnection::NewDraftMessage,
            this,&TcpSocketConnection::dequeueDraftMessages);

     connect(connection,&QTcpSocket::disconnected,
             this,&TcpSocketConnection::handleDisconnect);
     connect(connection,&QTcpSocket::bytesWritten,
             this,&TcpSocketConnection::sendConfirm);

    serverIp=connection->localAddress().toString();
    serverPort=connection->localPort();
    clientIp=connection->peerAddress().toString();
    clientPort=connection->peerPort();

    send_timeout = new QTimer();
    send_timeout->setSingleShot(true);
    send_timeout->setInterval(300);
    connect(send_timeout,&QTimer::timeout,
            this,&TcpSocketConnection::sendTimeout);

    //add other things if you want
}

