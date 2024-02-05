#ifndef PLAYER_H
#define PLAYER_H
#include <QWidget>
#include <iostream>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <fstream>


class Player
{
public:
    Player(QString usernsme="not defined",QString password="not defined");
    Player(QString username, size_t h_pass);
    bool operator==(const Player& other);
    Player(const Player& other);
    Player& operator=(const Player &other);
    int getTotalplayes()const;
    int getWins()const;
    int getDraws()const;
    int getLoses()const;
    size_t getPass()const;
    void increse(const char& status);//increse total games and win or lose or draw
    //d = draw
    //l = lose
    //w = win

    bool auth(QString username,QString pass="")const;
    const QString &getUsername() const;


private:
    QString username;
    size_t password;
    int wins;
    int loses;
    int draw;
};

#endif // PLAYER_H
