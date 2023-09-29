#include "servergamehandeler.h"
#include "ui_servergamehandeler.h"

ServerGameHandeler::ServerGameHandeler
(QHostAddress ip, qint16 port,QWidget *parent) :
    QWidget(nullptr),
    ui(new Ui::ServerGameHandeler),
    last_page(parent),
    ic(":/icons/person.png"),
    admin_ic(":/icons/admin.png"),
    cross(":/pics/cross.png"),
    circle(":/pics/circle.png"),
    server_port(port)
{
    ui->setupUi(this);

    //--------------------//
//    readTimer = new QTimer();
//    readTimer->setInterval(200);
//    readTimer->setSingleShot(false);


    //--------------------//
    tcp_server = new QTcpServer();
    tcp_server->listen(ip,server_port);
    connect(tcp_server,&QTcpServer::newConnection,
            this,&ServerGameHandeler::handelNewConnection);

    //--------------------//

    QListWidgetItem * member_item =new QListWidgetItem(
                "server\n"
                +ip.toString()
                +":"
                +QString::number(port));
    member_item->setBackground(QBrush(QColor(Qt::GlobalColor::darkCyan)));
    member_item->setTextAlignment(Qt::AlignLeft);
    member_item->setIcon(admin_ic);
    ui->listOfMembers->addItem(member_item);

    //------------------------//
    clearTheBoard();
    QPixmap backgroundImage(":/pics/back3.png");
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    ui->boardWidget->setAutoFillBackground(true);
    ui->boardWidget->setPalette(palette);

    //--------------------------//
    connect(&db,&DataBase::notify,
            this,&ServerGameHandeler::authNewConnections);
}

void ServerGameHandeler::sendToAllMembers(QString t,QString msg,QString s)
{
    foreach(TcpSocketConnection * tsc,members)
    {
        tsc->sendMessage(t,msg,s);
    }
}
void ServerGameHandeler::shareNewMember(TcpSocketConnection * new_member)
{

    foreach(TcpSocketConnection * member ,members)
    {
        if(member!=new_member)
        {
            new_member->sendMessage("initmember",
                                    member->getName() +':'+member->getIp()
                                    ,"server");
        }

    }
    qDebug()<<new_member->getName() +':'+new_member->getIp();
    sendToAllMembers("initmember",
                     new_member->getName() +':'+new_member->getIp());

}
void ServerGameHandeler::startNewGame()
{
    clearTheBoard();
    if(player_one.getTotalplayes()%2)
    {
        sendToAllMembers("start",player_one.getUsername());
    }
    else
    {
        sendToAllMembers("start",player_two.getUsername());
    }
}
char ServerGameHandeler::playerSelected(int place)
{
    if(move%2)
    {
        switch (place)
        {
        case 0:ui->a1->setIcon(cross);break;
        case 1:ui->a2->setIcon(cross);break;
        case 2:ui->a3->setIcon(cross);break;
        case 3:ui->b1->setIcon(cross);break;
        case 4:ui->b2->setIcon(cross);break;
        case 5:ui->b3->setIcon(cross);break;
        case 6:ui->c1->setIcon(cross);break;
        case 7:ui->c2->setIcon(cross);break;
        case 8:ui->c3->setIcon(cross);break;
        default:break;
        }
    }
    else
    {
        switch (place)
        {
        case 0:ui->a1->setIcon(circle);break;
        case 1:ui->a2->setIcon(circle);break;
        case 2:ui->a3->setIcon(circle);break;
        case 3:ui->b1->setIcon(circle);break;
        case 4:ui->b2->setIcon(circle);break;
        case 5:ui->b3->setIcon(circle);break;
        case 6:ui->c1->setIcon(circle);break;
        case 7:ui->c2->setIcon(circle);break;
        case 8:ui->c3->setIcon(circle);break;
        default:break;
        }
    }

    int row = place/3;
    int col = place%3;
    char current_move;

    if(move%2)
    {
        //red move
        current_move='r';
    }
    else
    {
        //green move
        current_move='g';
    }
    board[row][col]=current_move;
    int items=0;

    for(int i=0;i<3;i++)
    {
        if(board[i][col]==current_move)items++;
    }
    if(items==3)return current_move;
    items=0;

    for(int i=0;i<3;i++)
    {
        if(board[row][i]==current_move)items++;
    }
    if(items==3)return current_move;
    items=0;



    for(int i=0;i<3;i++)
    {
        if(board[i][i]==current_move)items++;
    }
    if(items==3)return current_move;
    items=0;



    for(int i=0; i<3;i++)
    {
        if(board[i][2-i]==current_move)items++;
    }
    if(items==3)return current_move;

    if(move<8)
    {
        //no winner , continue
        move++;
        return 'n';

    }
    else
    {
        //all moves have done and there is no winner
        return 'd';
    }


}
void ServerGameHandeler::clearTheBoard()
{
    move = 0;
    for(int i=0 ;i<3;i++)
    {
        for(int j=0;j<3;j++)board[i][j]='n';
    }
    rematch[0]=rematch[1]=false;
    ui->a1->setIcon(QPixmap(0,0));
    ui->a2->setIcon(QPixmap(0,0));
    ui->a3->setIcon(QPixmap(0,0));
    ui->b1->setIcon(QPixmap(0,0));
    ui->b2->setIcon(QPixmap(0,0));
    ui->b3->setIcon(QPixmap(0,0));
    ui->c1->setIcon(QPixmap(0,0));
    ui->c2->setIcon(QPixmap(0,0));
    ui->c3->setIcon(QPixmap(0,0));
}




//------------------//


ServerGameHandeler::~ServerGameHandeler()
{

    delete ui;


}
void ServerGameHandeler::on_SendButton_clicked()
{
    QString sending_message = ui->chatTextEdit->toPlainText();
    QListWidgetItem * message_item =new QListWidgetItem(sending_message);
    message_item->setBackground(QBrush(QColor(Qt::GlobalColor::cyan)));
    message_item->setTextAlignment(Qt::AlignRight);
    ui->chatList->addItem(message_item);
    ui->chatTextEdit->clear();
    //----send it to TCP socket;
    sendToAllMembers("normalmessage",sending_message);
}
void ServerGameHandeler::on_BackButton_clicked()
{
    last_page->show();
    this->~ServerGameHandeler();
}
void ServerGameHandeler::handelNewConnection()
{
    qDebug()<<"before creating tsc";
    TcpSocketConnection *  nc =
            new  TcpSocketConnection(tcp_server->nextPendingConnection()
                                     ,true,"server");
    waiters.append(nc);
    connect(nc,&TcpSocketConnection::newEvent,
            this,&ServerGameHandeler::handelNewEvent);
    qDebug() << "new connection";


}
void ServerGameHandeler::handelNewEvent(Message msg, TcpSocketConnection *con)
{
    if(msg.get_type()=="join")
    {



        {            

            //need to add more graphic
            QListWidgetItem * member_item =new QListWidgetItem(
                        con->getName()
                        +'\n'
                        +con->getIp()
                        +":"
                        +QString::number(con->getPort()));
            member_item->setBackground(QBrush(QColor(Qt::GlobalColor::cyan)));
            member_item->setTextAlignment(Qt::AlignLeft);
            member_item->setIcon(ic);

            ui->listOfMembers->addItem(member_item);
            members.append(con);
            waiters.removeOne(con);
            shareNewMember(con);
            if(msg.get_message()!="")//its new player
            {
                if(members.size()==1)
                {
                    player_one = Player(msg.get_sender_name(),msg.get_message());
                    db.addPlayer(player_one);
                }
                if(members.size()==2)
                {

                    player_two = Player(msg.get_sender_name(),msg.get_message());
                    db.addPlayer(player_one);
                    startNewGame();
                }
            }


        }
    }
    else if(msg.get_type()=="signin")
    {
        con->setName(msg.get_sender_name());
        db.find(msg.get_sender_name(),msg.get_message());
    }
    else if(msg.get_type()=="signup")
    {
        con->setName(msg.get_sender_name());
        db.find(msg.get_sender_name());
    }
    else if(msg.get_type()=="normalmessage")
    {

        sendToAllMembers("normalmessage",msg.get_message(),msg.get_sender_name());
       //---------------------------//
        QListWidgetItem * message_item =
                new QListWidgetItem(">>> "+msg.get_sender_name()+" :"
                                    +'\n' +msg.get_message());
        message_item->setBackground(QBrush(QColor(Qt::GlobalColor::yellow)));
        message_item->setTextAlignment(Qt::AlignLeft);
        message_item->setIcon(ic);
        ui->chatList->addItem(message_item);

    }
    else if(msg.get_type()=="play")
    {
        int player_move=msg.get_message().toInt();
        //forward the the message to members
        sendToAllMembers("play",msg.get_message(),msg.get_sender_name());
        char s=playerSelected(player_move);
        if(s=='r'||s=='g')
        {
            qDebug()<<s<< " :"<<msg.get_sender_name();
            sendToAllMembers("wins",msg.get_sender_name());
            if(s=='g')
            {
                player_one.increse('w');
                player_two.increse('l');
            }
            else
            {
                player_one.increse('l');
                player_two.increse('w');
            }


        }
        else if(s=='d')
        {
            qDebug()<<"we have draw";
            sendToAllMembers("draw","");
            player_one.increse('d');
            player_two.increse('d');
        }
        //else // 'n' nothing just continue





    }
    else if(msg.get_type()=="resign")
    {
        if(player_one.getUsername()==msg.get_sender_name())
        {
            sendToAllMembers("wins",player_two.getUsername());
        }
        else
        {
            sendToAllMembers("wins",player_one.getUsername());
        }

    }
    else if(msg.get_type()=="rematch")
    {
        if(msg.get_sender_name()==player_one.getUsername())rematch[0]=true;
        if(msg.get_sender_name()==player_two.getUsername())rematch[1]=true;
        if(rematch[0]&&rematch[1])
        {
            startNewGame();
        }
    }
    else if(msg.get_type()=="disconnected")
    {
        qDebug()<<"in dicon";
        qDebug()<<msg.get_sender_name();

        if(!members.contains(con))return; // not a member
        qDebug()<<"should display";
        sendToAllMembers("quit","",msg.get_sender_name());
        QListWidgetItem * message_item =
                new QListWidgetItem(">>> "+msg.get_sender_name()
                                    +" left <<<");

        message_item->setBackground(QBrush(QColor(Qt::GlobalColor::red)));
        message_item->setTextAlignment(Qt::AlignCenter);
        ui->chatList->addItem(message_item);
        qDebug()<<"should3 display";
        int i=0;
        while(true)
        {
            QListWidgetItem * item=ui->listOfMembers->item(i);
            if(item==nullptr)break;
            if(item->text().contains(msg.get_sender_name()))
            {
                ui->listOfMembers->removeItemWidget(item);
                delete item;
                item=nullptr;
                break;
            }
            i++;
        }
        members.removeOne(con);
        qDebug()<<members.size();
    }
    else
    {
        qDebug()<<"unknown message";
    }
}

void ServerGameHandeler::authNewConnections(int i,QString username)
{

    qDebug()<<"in auth handle"<<i<<username;
        foreach(const auto& con,waiters )
        {
            if(con->getName()==username)
            {
                if(i==-1)
                {
                    con->sendMessage("wrongauth");//wrong pass or username
                }
                else if(i ==-2)
                {
                    con->sendMessage("authconfirm");//new player
                }
                else
                {
                    con->sendMessage("authconfirm");//user auth was correct
                    //and not a new user
                    if(members.size()==0)
                    {
                        player_one= db[i];
                    }
                    else if(members.size()==1)
                    {
                        player_two= db[i];
                    }
                }

                break;
            }

        }



}



