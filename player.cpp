#include "player.h"

Player::Player(QString ur,QString ps):
    username(ur),password(ps),
    wins(0),loses(0),draw(0){}

//Player::Player(const Player &other)
//{
//    username
//}

//Player &Player::operator=(const Player &other)
//{

//}

int Player::getTotalplayes() const
{
    return (wins+loses+draw);
}

int Player::getWins() const
{
    return wins;
}

int Player::getDraws() const
{
    return draw;
}

int Player::getLoses() const
{
    return loses;
}

void Player::increse(const char &status)
{
    if(status=='w')wins++;
    else if(status=='l')loses++;
    else if(status=='d')draw++;
    else return;
}

const QString &Player::getUsername() const
{
    return username;
}



