#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&api,&APIHandler::getTimeDate,
            this,&MainWindow::getTimeDate);

    QPixmap backgroundImage(":/pics/black.png");
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    ui->hour->setAutoFillBackground(true);
    ui->hour->setPalette(palette);
    ui->minute->setAutoFillBackground(true);
    ui->minute->setPalette(palette);
    ui->second->setAutoFillBackground(true);
    ui->second->setPalette(palette);

    secondTimer = new QTimer();
    secondTimer->setInterval(1000);
    secondTimer->setSingleShot(false);
    connect(secondTimer,&QTimer::timeout,
            this,&MainWindow::increase);

}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::on_ServerJoinButton_clicked()
{

        server_page = new Server(this);
        server_page->show();
        this->close();

}

void MainWindow::getTimeDate(QString date, QString time)
{

    ui->dateLabel->setText(date);
    QString hour = QString(time.toStdString().substr(0,2).c_str());
    QString min = QString(time.toStdString().substr(3,2).c_str());
    QString sec = QString(time.toStdString().substr(6,2).c_str());


    ui->hour->display(hour);
    ui->minute->display(min);
    ui->second->display(sec);
    secondTimer->start();
}


void MainWindow::on_ClientJoinButton_clicked()
{

        client_page = new Client(this);
        client_page->show();
        this->close();
}

void MainWindow::increase()
{
    int sec=ui->second->value();
    int min=ui->minute->value();
    if(sec==59)
    {
        if(min==59)
        {
            api.reTry();
        }
        else
        {
            ui->minute->display(min+1);
            ui->second->display(0);
        }
    }
    else
    {
        ui->second->display(sec+1);
    }
}







