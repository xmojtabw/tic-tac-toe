#include "database.h"


DataBaseWorker::DataBaseWorker(QVector<Player> &_players):
    players(_players){}

void DataBaseWorker::find(QString username, QString pass)
{
    qDebug()<<"in find";
    qDebug()<<pass;
    int i = 0;
    for (const auto& player : players)
    {
        if (player.auth(username, pass))
        {
            if(pass=="")
            {
                //you are a new player and your username is already exist
                emit notify(-1,username);
            }
            else
            emit notify(i,username);//your auth is correct
            return;

        }
        i++;
    }
    // Notify that all searches are done
    if(pass=="")emit notify( -2,username);//username doesn't exist so can signin
    else
        emit notify(-1,username);//wrong pass or username can't signup

}





DataBase::DataBase(QObject *parent)
    : QObject{parent},file("db.txt")
{
    if(QFile::exists("db.txt"))
    {

        if (file.open(QIODevice::ReadOnly)) {
            while(!file.atEnd())
            {
                QString line1=file.readLine();
                line1.removeLast();
                if(file.atEnd())
                {
                    break;
                }
                QString line2=file.readLine();
                line2.removeLast();
                size_t hpass = std::strtoull(line2.toStdString().c_str(),nullptr,10);
                Player t(line1,hpass);
                players.push_back(t);

            }
        }
        file.close();
    }


    worker = new DataBaseWorker(players);
    workerThread = new QThread;
    worker->moveToThread(workerThread);


    connect(worker, &DataBaseWorker::notify, this, &DataBase::handleDatabaseResult);

    workerThread->start();
}

DataBase::~DataBase()
{
    workerThread->quit();
    workerThread->wait();
    delete  workerThread;
    delete  worker;
}

void DataBase::addPlayer(Player& Player)
{
    players.push_back(Player);
}

Player &DataBase::operator[](const int &i)
{
    return players[i];
}


void DataBase::find(QString username,QString pass)
{
    QMetaObject::invokeMethod(worker, "find", Qt::QueuedConnection,
                              Q_ARG(QString, username), Q_ARG(QString, pass));
}

void DataBase::save()
{
    if (file.open(QIODevice::WriteOnly)) {
        for(auto &p:players)
        {
            std::string line1 = p.getUsername().toStdString()+'\n';
            std::string line2 = std::to_string(p.getPass())+'\n';
            file.write(line1.c_str());
            file.write(line2.c_str());
        }
    }
    file.close();
}

void DataBase::handleDatabaseResult(int index,QString username)
{
    emit notify(index,username);
}




