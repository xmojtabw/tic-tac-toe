#include "servergamehandeler.h"
#include "ui_servergamehandeler.h"

ServerGameHandeler::ServerGameHandeler
(QHostAddress ip, qint16 port,QWidget *parent) :
    QWidget(nullptr),
    ui(new Ui::ServerGameHandeler),
    last_page(parent),
    ic(":/icons/person.png"),
    admin_ic(":/icons/admin.png"),
    privacy(public_),
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
}




//------------------//


ServerGameHandeler::~ServerGameHandeler()
{
    delete ui;

}






void ServerGameHandeler::on_changeModeButton_clicked()
{
    //not compilited
    if(privacy==public_)
    {

    }

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

        if(privacy==public_)

        {
            con->setName(msg.get_sender_name());
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
            con->sendMessage("joinconfirm");
            members.append(con);
            waiters.removeOne(con);
            shareNewMember(con);
            if(members.size()==1)
            {
                player_one = Player(msg.get_sender_name(),msg.get_message());
            }
            if(members.size()==2)
            {
                player_two = Player(msg.get_sender_name(),msg.get_message());
                startNewGame();
            }

        }
        else if(privacy==private_)
        {
            waiters.push_front(con);
            //not compilited
        }
        else
        {
            //ignore , nothing will happen
        }


        //add perrmissn later
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
            }

        }
        else if(s=='d')
        {
            qDebug()<<"we have draw";
            sendToAllMembers("draw","");
        }
        //else // 'n' nothing just continue





    }
    else
    {
        qDebug()<<"unknown message";
    }
}



