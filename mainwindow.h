#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "server.h"
#include "client.h"
#include "apihandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ServerJoinButton_clicked();
    void getTimeDate(QString time, QString date);
    void on_ClientJoinButton_clicked();
    void increase();


private:
    Ui::MainWindow *ui;
    Server* server_page;
    Client* client_page;
    APIHandler api;
    QTimer* secondTimer;
};
#endif // MAINWINDOW_H
