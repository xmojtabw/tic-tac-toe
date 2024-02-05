#ifndef DATABASE_H
#define DATABASE_H
#include "player.h"
#include "tcpsocketconnection.h"
#include <QThread>
#include <QObject>
#include <QFile>

class DataBaseWorker:public QObject
{
    Q_OBJECT
public:
    DataBaseWorker(QVector<Player>& players);

public slots:
    void find(QString username,QString pass="");
signals:
    void notify(int index,QString username);

private:
    QVector<Player>& players;
};


class DataBase : public QObject
{
    Q_OBJECT
public:
    DataBase(QObject *parent = nullptr);
    ~DataBase();
    void addPlayer(Player& Player);
    Player &operator[](const int& i);
    void changeOnePlayer(Player &player);
    void find(QString username,QString pass="");
    void save();
private:

    DataBaseWorker* worker;
    QThread* workerThread;
    QVector<Player> players;
    QFile file;
signals:
    void notify(int index,QString username);

private slots:
    void handleDatabaseResult(int index,QString usernmae);
};

#endif // DATABASE_H
