#include "serverbroadsetpage.h"
#include "ui_serverbroadsetpage.h"

ServerBroadSetPage::ServerBroadSetPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerBroadSetPage)
{
    ui->setupUi(this);
}

ServerBroadSetPage::~ServerBroadSetPage()
{
    delete ui;
}




void ServerBroadSetPage::on_applyButton_clicked()
{
    emit apply_changes_signal(ui->autoCheckbox->isChecked(),
                              ui->d_time->value(),
                              ui->u_port->value());
    this->close();
}


void ServerBroadSetPage::on_cancelButton_clicked()
{
    this->close();
}

