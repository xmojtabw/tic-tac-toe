#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent,QString usr) :
    QWidget(nullptr),
    ui(new Ui::Server),
    last_page(parent),
    username(usr)
{
    ui->setupUi(this);
    lan_page=new Lans();
    connect(lan_page,&Lans::send_network_ip,this, &Server::get_network_ip);

}

Server::~Server()
{
    delete ui;
}

void Server::on_LocalHostButton_clicked()
{
    entered_ip=QHostAddress::LocalHost;
    ui->IPLineEdit->setText(entered_ip.toString());
}


void Server::on_LanButton_clicked()
{
    lan_page->show();
}
void Server::get_network_ip(QString ip){
    int pos=ip.toStdString().find(" : ");
    QString extracted_ip=QString(ip.toStdString().substr(0,pos).c_str());
    ui->IPLineEdit->setText(extracted_ip);
}

void Server::on_NextButton_clicked()
{
   entered_ip=QHostAddress(ui->IPLineEdit->text());
   entered_port = ui->PortSpinBox->value();
    if(entered_ip.protocol()==QAbstractSocket::IPv4Protocol&&!entered_ip.isNull())
    {
        //ip is valid
        chatpage = new ServerGameHandeler(entered_ip,entered_port,this);

//        //beacuse of centering the window ----
        chatpage->setGeometry(QStyle::alignedRect(
                              Qt::LeftToRight,
                              Qt::AlignCenter,
                              chatpage->size(),
                              //QGuiApplication::screens().first()->availableGeometry()
                              QGuiApplication::primaryScreen()->availableGeometry()
                                 ));
        //------------------------------------

        chatpage->show();
        this->close();
    }
    else if(entered_ip.protocol()==QAbstractSocket::IPv6Protocol&&
            !entered_ip.isNull())
    {
        //ipv6 is not supported yet
        QMessageBox::critical(nullptr,"Error","IPv6 is not supported yet");
    }
    else
    {
        QMessageBox::critical(nullptr,"Error","invalid IPv4 address");
    }
}

void Server::on_BackButton_clicked()
{
    last_page->show();
    this->~Server();
}

