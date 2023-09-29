#include "client.h"
#include "ui_client.h"



Client::Client(QWidget *parent,QString usr_name) :
    QWidget(nullptr),
    ui(new Ui::Client),
    last_page(parent),
    username(usr_name)
{
    ui->setupUi(this);
    lan_page=new Lans();
    connect(lan_page,&Lans::send_network_ip,this, &Client::get_network_ip);
    //------------------//
    //------------------//
     tcp_client = new QTcpSocket();
     connect(tcp_client,&QTcpSocket::connected,
             this,&Client::tcpConnected);
    //------------------//

    //------------------//


}

Client::~Client()
{
    delete ui;
    delete lan_page;

}

void Client::on_BackButton_clicked()
{
    last_page->show();
    this->~Client();
}

void Client::get_network_ip(QString ip){
    int pos=ip.toStdString().find(" : ");
    QString extracted_ip=QString(ip.toStdString().substr(0,pos).c_str());
    ui->IPLineEdit->setText(extracted_ip);
}

void Client::on_LanButton_clicked()
{
    lan_page->show();
}


void Client::tcpConnected()
{
    qDebug()<<"tcp connected";
    tcp_connection = new TcpSocketConnection(tcp_client,false,username);
    connect(tcp_connection,&TcpSocketConnection::newEvent,
            this,&Client::tcpConfirmation);
    tcp_connection->sendMessage("join",password);
}

void Client::tcpConfirmation(Message msg, TcpSocketConnection *connection)
{
    if(msg.get_type()=="joinconfirm")
    {

        qDebug()<<"join confirmed by server";
        connection->setName(username);
        connection->setName(msg.get_sender_name(),false);
        chat_page = new ClientGamePage(connection,username);
        chat_page->show();
        this->close();
    }
    else if(msg.get_type()=="wrongpass")
    {

    }
    else if(msg.get_type()=="wrongusername")
    {

    }
    else
    {

    }
}


void Client::on_LocalhostButton_clicked()
{
   ui->IPLineEdit->setText(QHostAddress(QHostAddress::LocalHost).toString());
}


void Client::on_nextButton_clicked()
{
    QString ip = ui->IPLineEdit->text();
    int port = ui->PortSpinBox->value();
    tcp_client->connectToHost(QHostAddress(ip),port);
    username=ui->usernameLineedit->text();
    password=ui->passwordLineedit->text();
}

