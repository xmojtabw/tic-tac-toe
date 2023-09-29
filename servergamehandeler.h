#ifndef SERVERGAMEHANDELER_H
#define SERVERGAMEHANDELER_H

#include <QWidget>
#include <QHostAddress>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QIcon>
#include <QPalette>

#include <thread>
#include "player.h"
#include "message.h"
#include "serverbroadsetpage.h"
#include "tcpsocketconnection.h"

typedef struct Event
{
    Event(Message m,TcpSocketConnection *c):
        msg(m),con(c){}
    Message msg;
    TcpSocketConnection *con;
}Event;

namespace Ui {
class ServerGameHandeler;
}

class ServerGameHandeler : public QWidget
{
    Q_OBJECT

public:
     ServerGameHandeler(QHostAddress ip,
                    qint16 port,QWidget *parent = nullptr);
     void sendToAllMembers(QString type,QString message,QString sender="server");
     void shareNewMember(TcpSocketConnection * new_member);
     void startNewGame();
     char playerSelected(int place);
     void clearTheBoard();

    ~ServerGameHandeler();
private slots:

     void handelNewConnection();
     void handelNewEvent(Message msg , TcpSocketConnection * connection);
     //--------------------//
     void on_changeModeButton_clicked();

     void on_SendButton_clicked();

     void on_BackButton_clicked();



private:
    Ui::ServerGameHandeler *ui;
    QWidget* last_page;
    QIcon ic;
    QIcon admin_ic;
    QIcon cross;
    QIcon circle;
    enum privacy_mode{public_,private_,close};
    privacy_mode privacy;

    //members(clients) who are waiting for server confirmation
    QList<TcpSocketConnection *> waiters;
    QTcpServer* tcp_server;
    QList<TcpSocketConnection *>members;
    qint16 server_port;


    //---------------//
    Player player_one;
    Player player_two;
    char board[3][3];
    int move;

};

#endif // SERVERGAMEHANDELER_H
