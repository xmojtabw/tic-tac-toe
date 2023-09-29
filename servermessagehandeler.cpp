#include "servermessagehandeler.h"


serverMessageHandeler::serverMessageHandeler(QTcpSocket *con):
socket(con)
{
    stop=false;
    readThread = std::thread{&serverMessageHandeler::reader,this};
}

serverMessageHandeler::~serverMessageHandeler()
{
    if(!stop)
    {
        stop=true;
        readThread.join();
        qDebug()<<"thread joins";
    }

}

void serverMessageHandeler::reader()
{
    while(socket->state()==QTcpSocket::ConnectedState&&!stop)
    {
        if(socket->bytesAvailable()>0&&socket->bytesAvailable()<4090)
        {
            QByteArray buffer=socket->readAll();
//            qDebug()<<"buffer :"<<buffer;
//            qDebug()<<buffer.size();
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
//                    qDebug()<<"message data :"<<messageData;
        //            qDebug()<<"rec data"<<received_data;

                    //lock
                    QMutexLocker locker(&inboxMutex);
                    Message msg(messageData);
                    inbox.push_front(msg);
                }
                else
                {
                    // Not enough data for a complete message yet
                    break;
                }
            }
        }
    }
    qDebug()<<"thread func finished";

}

void serverMessageHandeler::stopThread()
{
    stop=true;
}

QList<Message> serverMessageHandeler::getMessages()
{
    // Lock the inbox for safe access
    QMutexLocker locker(&inboxMutex);

    QList<Message> messages = inbox;
    inbox.clear();

    return messages;
}


