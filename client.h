#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTimer>

#include "clientgamepage.h"
#include "lans.h"
#include "message.h"
#include "tcpsocketconnection.h"

//-----------------//

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr,QString usr_name="");
    ~Client();

private slots:

    void get_network_ip(QString ip);

    void tcpConnected();

    void tcpConfirmation(Message msg , TcpSocketConnection * connection);

    void on_LanButton_clicked();

    void on_BackButton_clicked();

    void on_LocalhostButton_clicked();

    void on_nextButton_clicked();

private:
    Ui::Client *ui;
    QWidget * last_page;
    Lans * lan_page;
    ClientGamePage * chat_page;

    QTcpSocket * tcp_client;
    TcpSocketConnection * tcp_connection;
    QString username;
    QString password;
};


#endif // CLIENT_H