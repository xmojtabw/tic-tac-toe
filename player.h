#ifndef PLAYER_H
#define PLAYER_H
#include <QWidget>

class Player
{
public:
    Player(QString usernsme="not defined",QString password="not defined");
//    Player(const Player& other);
//    Player& operator=(const Player& other);
    int getTotalplayes()const;
    int getWins()const;
    int getDraws()const;
    int getLoses()const;
    void increse(const char& status);//increse total games and win or lose or draw
    //d = draw
    //l = lose
    //w = win


    const QString &getUsername() const;


private:
    QString username;
    QString password;
    int wins;
    int loses;
    int draw;
};

#endif // PLAYER_H
