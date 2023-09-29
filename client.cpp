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

void Client::connectToHost()
{
    qDebug()<<tcp_client->state();
    if(tcp_client->state()==QAbstractSocket::UnconnectedState)
    {
        qDebug()<<"in connect to host unconnectec";
        QString ip = ui->IPLineEdit->text();
        int port = ui->PortSpinBox->value();
        tcp_client->connectToHost(QHostAddress(ip),port);
        username=ui->usernameLineedit->text();
        password=ui->passwordLineedit->text();
    }
    else
    {
        qDebug()<<"in connect to host connecting";
        delete tcp_client;
        tcp_client = new QTcpSocket();
        connect(tcp_client,&QTcpSocket::connected,
                this,&Client::tcpConnected);
        QString ip = ui->IPLineEdit->text();
        int port = ui->PortSpinBox->value();
        tcp_client->connectToHost(QHostAddress(ip),port);
        username=ui->usernameLineedit->text();
        password=ui->passwordLineedit->text();
    }
}

Client::~Client()
{
    delete ui;
    delete lan_page;
    delete tcp_connection;

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
    if(is_newplayer)
    {
        tcp_connection->sendMessage("signup",password);
    }
    else
    {
        tcp_connection->sendMessage("signin",password);
    }

}

void Client::tcpConfirmation(Message msg, TcpSocketConnection *connection)
{
    if(msg.get_type()=="authconfirm")
    {

        qDebug()<<"join confirmed by server";
        connection->setName(username);
        connection->setName(msg.get_sender_name(),false);
        if(is_newplayer)
        {
            connection->sendMessage("join",password);//send our info
        }
        else
        {
            connection->sendMessage("join");//don't need to send info
        }

        chat_page = new ClientGamePage(connection,username);
        disconnect(connection,&TcpSocketConnection::newEvent,
                   this,&Client::tcpConfirmation);
        chat_page->show();
        this->close();
    }
    else if(msg.get_type()=="wrongauth")
    {
        if(is_newplayer)
        {

            disconnect(connection,&TcpSocketConnection::newEvent,
                       this,&Client::tcpConfirmation);
            qDebug()<<"before delte";
            QMessageBox::critical(nullptr,"Error","username is already exist");

        }
        else
        {

            disconnect(connection,&TcpSocketConnection::newEvent,
                       this,&Client::tcpConfirmation);
            QMessageBox::critical(nullptr,"Error","wrong username or pass");
        }

        //wrong username or password or if is new player username is already exist


    }
    else
    {
        qDebug()<<"wrong message";
    }
}


void Client::on_LocalhostButton_clicked()
{
   ui->IPLineEdit->setText(QHostAddress(QHostAddress::LocalHost).toString());
}


void Client::on_nextButton_clicked()
{
    //sign in
    is_newplayer= false;
    connectToHost();
}


void Client::on_signupButton_clicked()
{

    is_newplayer= true;
    connectToHost();
}

