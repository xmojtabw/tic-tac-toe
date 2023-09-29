#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
//#include <QDesktopWidget>
#include <QStyle>
#include <QScreen>
#include <QApplication>
//#include <QTcpServer>
//#include <QTcpSocket>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QMessageBox>
#include "lans.h"
#include "servergamehandeler.h"

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
     Server(QWidget *parent = nullptr,QString username="");
    ~Server();

private slots:

    void on_LocalHostButton_clicked();
    void on_LanButton_clicked();
    void get_network_ip(QString ip);
    void on_NextButton_clicked();

    void on_BackButton_clicked();

private:
    Ui::Server *ui;
    QWidget* last_page;
    Lans * lan_page;
    ServerGameHandeler * chatpage;
    QString username;
    QHostAddress entered_ip;
    qint16 entered_port;
};

#endif // SERVER_H
