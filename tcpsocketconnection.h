#ifndef TCPSOCKETCONNECTION_H
#define TCPSOCKETCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QThread>

#include "message.h"
#include "servermessagehandeler.h"


class TcpSocketConnection :public QObject
{
    Q_OBJECT
public:
    TcpSocketConnection(QTcpSocket * connection,bool is_server,QString name);
    void sendMessage(QString type, QString Message="",QString sender="n");
    void dequeue_drafts(Message msg);
    void disconnectFromServer();
    ~TcpSocketConnection();

    //----------------//
    //for getting(or setting) server details: set the client boolean to false
    //for getting(or getting) client details: set the client boolean to true
    //by default its true
    void setName(QString n,bool client=true);
    QString getName(bool client=true);
    QString getIp(bool client=true);
    quint16 getPort(bool client=true);

private slots:
    void handleNewMessages();
    void handleDisconnect();
    void sendConfirm(qint64 Bytes);
    void sendTimeout();

    //only for server
    void checkForNewMessages();


signals:
    void newEvent(Message msg,TcpSocketConnection * connection);

private:

    serverMessageHandeler * smh;

    QTcpSocket * connection;
    QString name;//if is server => server name & if not => client name

    QString clientName;
    QString serverName;
    bool is_server;//false means member(client)

    QString serverIp;
    quint16 serverPort;
    QString clientIp;
    quint16 clientPort;


    QTimer * send_timeout;



    QList<Message> draft_messages;
    bool writing;


    //---------//
    //only for server
    QList<Message> recieved_messages;
    QTimer* readTimer;




};

#endif // TCPSOCKETCONNECTION_H
