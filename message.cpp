#include "message.h"
Message::Message():message("undefined"){}
Message::Message(QString msg):message(mark+':'+msg){}
Message::Message(QString msg,QString sender_name,QString type)
{
    message=mark;
    message+=":t:"+type;
    message+=":s:"+sender_name;
    message+=":m:"+msg;
    message+=":e:";
}
//-----geting whole message
QString Message::to_qstring(){return message;}
std::string Message::to_string(){ return message.toStdString();}
//-----get exact message
QString Message::get_message()
{
    std::string msg= message.toStdString();
    int pos_start=msg.find(":m:")+distance;
    int npos=msg.find(":e:")-pos_start;
    return converter(msg.substr(pos_start,npos));
}
//-----getting data about message
QString Message::get_sender_name()
{
    std::string msg= message.toStdString();
    int pos_start=msg.find(":s:")+distance;
    int npos=msg.find(":m:")-pos_start;
    return converter(msg.substr(pos_start,npos));
}
QString Message::get_type()
{
    std::string msg= message.toStdString();
    int pos_start=msg.find(":t:")+distance;
    int npos=msg.find(":s:")-pos_start;
    return converter(msg.substr(pos_start,npos));
}

//-----converter
QString Message::converter(std::string anything)const{
    return QString(anything.c_str());
}
