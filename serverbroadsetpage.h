#ifndef SERVERBROADSETPAGE_H
#define SERVERBROADSETPAGE_H

#include <QWidget>

namespace Ui {
class ServerBroadSetPage;
}

class ServerBroadSetPage : public QWidget
{
    Q_OBJECT

public:
    explicit ServerBroadSetPage(QWidget *parent = nullptr);
    ~ServerBroadSetPage();

private slots:


    void on_applyButton_clicked();

    void on_cancelButton_clicked();

signals:
    void apply_changes_signal(bool automatic,float d_time,int u_port);

private:
    Ui::ServerBroadSetPage *ui;
};

#endif // SERVERBROADSETPAGE_H
