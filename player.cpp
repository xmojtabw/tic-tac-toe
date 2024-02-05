#include "player.h"

Player::Player(QString ur,QString ps):
    username(ur),
    wins(0),loses(0),draw(0)
{
    password=qHash(ps);
}

Player::Player(QString u, size_t h_pass):username(u),password(h_pass),
    wins(0),loses(0),draw(0)
{

}

bool Player::operator==(const Player &other)
{
    if(other.username==username&&other.password==password)
    {
        return true;
    }
    return false;
}

Player &Player::operator=(const Player &other)
{
    username=other.username;
    password=other.password;
    wins=other.wins;
    loses=other.loses;
    draw=other.draw;
    return *this;
}
Player::Player(const Player &other)
{
    username=other.username;
    password=other.password;
    wins=other.wins;
    loses=other.loses;
    draw=other.draw;
}


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

size_t Player::getPass() const
{
    return password;
}

void Player::increse(const char &status)
{
    if(status=='w')wins++;
    else if(status=='l')loses++;
    else if(status=='d')draw++;
    else return;
}

bool Player::auth(QString _username, QString pass)const
{
    if(pass=="")
    {
        //to ckeck if the username is exist
        if(username==_username)return true;
    }
    else
    {
        size_t h_pass=qHash(pass);
        if(username==_username&&h_pass==password)return true;
    }
    return false;
}

const QString &Player::getUsername() const
{
    return username;
}



