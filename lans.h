#ifndef LANS_H
#define LANS_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QNetworkInterface>
#include <QOperatingSystemVersion>
namespace Ui {
class Lans;
}

class Lans : public QWidget
{
    Q_OBJECT

public:
    explicit Lans(QWidget *parent = nullptr);
    void update_networks();
    ~Lans();

private slots:
    void on_Networks_itemDoubleClicked(QListWidgetItem *item);
    void on_refreshButton_clicked();

signals:
    void send_network_ip(QString ip);
private:
    Ui::Lans *ui;
    QListWidget * lan_list;
};

#endif // LANS_H
