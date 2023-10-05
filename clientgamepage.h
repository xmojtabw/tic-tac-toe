#ifndef CLIENTGAMEPAGE_H
#define CLIENTGAMEPAGE_H

#include <QWidget>
#include <QIcon>

#include "tcpsocketconnection.h"

namespace Ui {
class ClientGamePage;
}

class ClientGamePage : public QWidget
{
    Q_OBJECT

public:
    ClientGamePage(TcpSocketConnection*connection,QString usrnm ,QWidget *parent = nullptr);
    void clearTheBoard();
    void playerMoves(int place,bool is_my_turn);
    ~ClientGamePage();

private slots:
    void handelNewEvent(Message msg,TcpSocketConnection * connection);

    void on_sendButton_clicked();

    void on_backButton_clicked();

    void on_a1_clicked();

    void on_a2_clicked();

    void on_a3_clicked();

    void on_b1_clicked();

    void on_b2_clicked();

    void on_b3_clicked();

    void on_c1_clicked();

    void on_c2_clicked();

    void on_c3_clicked();

    void on_resignButton_clicked();

    void on_rematchButton_clicked();

private:
    Ui::ClientGamePage *ui;
    TcpSocketConnection *con;
    QIcon ic;
    QIcon admin_ic;
    QIcon circle;
    QIcon cross;
    QString username;
    bool first_time;
    int move;
    bool rematch;
    bool buttonsLock;
    bool disablebuttons[9];
};

#endif // CLIENTGAMEPAGE_H
