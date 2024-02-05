#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>
#include <QThread>
#include <QMessageBox>
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
    void connectToHost();
    ~Client();

private slots:

    void get_network_ip(QString ip);

    void tcpConnected();

    void tcpConfirmation(Message msg , TcpSocketConnection * connection);

    void on_LanButton_clicked();

    void on_BackButton_clicked();

    void on_LocalhostButton_clicked();

    void on_nextButton_clicked();

    void on_signupButton_clicked();

private:
    Ui::Client *ui;
    QWidget * last_page;
    Lans * lan_page;
    ClientGamePage * chat_page;

    bool is_newplayer;
    QTcpSocket * tcp_client;
    TcpSocketConnection * tcp_connection=nullptr;
    QString username;
    QString password;

    QThread * socket_thread;
};


#endif // CLIENT_H
