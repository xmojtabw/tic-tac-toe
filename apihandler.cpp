#include "apihandler.h"

APIHandler::APIHandler():
    apiUrl("https://worldtimeapi.org/api/timezone/Asia/Tehran"),
    request(apiUrl)
{



    reply = manager.get(request);

    // Connect a slot to handle the response when it arrives.
   connect(reply, &QNetworkReply::finished,
                     this,&APIHandler::getResponse);

}

void APIHandler::reTry()
{
    reply = manager.get(request);
}

void APIHandler::getResponse()
{

    if (reply->error() == QNetworkReply::NoError)
    {
        // Read the response data and process it.
        QByteArray responseData = reply->readAll();
        jsonResponse = QJsonDocument::fromJson(responseData);
        jsonObject = jsonResponse.object();
        date_time = jsonObject["datetime"].toString().toStdString();
        int t_pos=date_time.find('T');
        std::string date = date_time.substr(0,t_pos);
        int dot_pos =date_time.find('.');
        std::string time = date_time.substr(t_pos+1,dot_pos-t_pos-1);



        emit getTimeDate(QString(date.c_str()),QString(time.c_str()));



    }
    else
    {
         // Handle the error.
         qDebug() << "Error:" << reply->errorString();
         reTry();
    }

}


