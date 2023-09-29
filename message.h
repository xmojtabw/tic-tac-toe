#ifndef MESSAGE_H
#define MESSAGE_H
#include <QString>
#
class Message
{
public:
    Message();
    Message(QString message);
    Message(QString message,QString sender_name,QString type);
//    Message(std::string message,std::string sender_name,std::string type);
    //-----geting whole message
    QString to_qstring();
    std::string to_string();
    //-----get exact message
    QString get_message();
    //-----getting data about message
    QString get_sender_name();
    QString get_type();
    //-----converter
    QString converter(std::string anything)const;//just for converting
private:
    int distance=3;// 3 is because of :n: has three letters
    QString mark="tic-tac-toe";
    QString message;
};

#endif // MESSAGE_H
