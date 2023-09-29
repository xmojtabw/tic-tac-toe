#include "lans.h"
#include "ui_lans.h"

Lans::Lans(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Lans)
{
    ui->setupUi(this);
    update_networks();
}
void Lans::update_networks()
{
    //-------------------------//
    ui->Networks->clear();
    //-------------------------//
    QString ipAddress;

    QList<QNetworkInterface> networkInterfaces = QNetworkInterface::allInterfaces();
    //putting all network inter faces to a QList


    foreach (QNetworkInterface networkInterface, networkInterfaces)
    {

        if (networkInterface.flags().testFlag(QNetworkInterface::IsUp))
         //if network is up
        {

            QString network_type;
            QString network_ip;
            switch (networkInterface.type()) {

                case(QNetworkInterface::Wifi):
                {
                    network_type="Wifi";
                }
                break;
                case(QNetworkInterface::Ethernet):
                {
                    network_type="Ethernet";
                }
                break;
                case(QNetworkInterface::Virtual):
                {
                    network_type="Virtual";
                }
                break;
                case(QNetworkInterface::Unknown):
                {
                    network_type="Unknown";
                }
                break;
                case(QNetworkInterface::CanBus):
                {
                    network_type="CanBus";
                }
                break;
                case(QNetworkInterface::Fddi):
                {
                    network_type="Fddi";
                }
                break;
                case(QNetworkInterface::Loopback):
                {
                    network_type="Local host";
                }
                break;
                default:
                {
                    network_type="Other";
                }
            }

            QList<QNetworkAddressEntry> addressEntries = networkInterface.addressEntries();
            foreach (QNetworkAddressEntry addressEntry, addressEntries)
            {
                if (addressEntry.ip().protocol() == QAbstractSocket::IPv4Protocol )
                   {
                     network_ip=addressEntry.ip().toString();
                     QListWidgetItem * item;
                     if(QOperatingSystemVersion::currentType()==QOperatingSystemVersion::Windows)
                     {
                         qDebug()<<"windows";
                         qDebug()<<networkInterface.humanReadableName();
                         item = new QListWidgetItem(network_ip+" : "+
                                                                      network_type+
                                                                      " # "+
                                                                      networkInterface.humanReadableName());


                     }
                     else
                     {
                         item = new QListWidgetItem(network_ip+" : "+network_type);
                     }

                     item->setTextAlignment(Qt::AlignCenter);
                     ui->Networks->addItem(item);

                   }
            }
        }
    }
    //-----------------//

}
Lans::~Lans()
{

    delete ui;
}

void Lans::on_Networks_itemDoubleClicked(QListWidgetItem *item)
{
    emit send_network_ip(item->text());
    this->close();
}


void Lans::on_refreshButton_clicked()
{
    update_networks();
}

