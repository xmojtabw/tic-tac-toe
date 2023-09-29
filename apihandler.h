#ifndef APIHANDLER_H
#define APIHANDLER_H
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWidget>

class APIHandler:public QObject
{
    Q_OBJECT
public:
    APIHandler();
    void reTry();

private slots:
    void getResponse();

signals:
    void getTimeDate(QString time, QString date);
private:
    QNetworkAccessManager manager;
    QUrl apiUrl;
    QNetworkRequest request;
    QNetworkReply *reply;
    QJsonDocument jsonResponse;
    QJsonObject jsonObject;
    std::string date_time;
};

#endif // APIHANDLER_H
