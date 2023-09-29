#ifndef SERVERMESSAGEHANDELER_H
#define SERVERMESSAGEHANDELER_H
#include <QTcpSocket>
#include <QList>
#include <QMutex>

#include <thread>

#include "message.h"


class serverMessageHandeler
{
public:
    serverMessageHandeler(QTcpSocket * connection);
    ~serverMessageHandeler();
    void reader();
    void stopThread();
    QList<Message> getMessages();

private:
    QTcpSocket * socket;
    QList<Message> inbox;
    bool stop;
    QMutex inboxMutex;
    std::thread readThread;
};

#endif // SERVERMESSAGEHANDELER_H
