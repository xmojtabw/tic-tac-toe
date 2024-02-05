#include "clientgamepage.h"
#include "ui_clientgamepage.h"

ClientGamePage::ClientGamePage
(TcpSocketConnection*connection,QString un, QWidget *parent ):
    QWidget(parent),
    ui(new Ui::ClientGamePage),
    con(connection),
    ic(":/icons/person.png"),
    admin_ic(":/icons/admin.png"),
    circle(":/pics/circle.png"),
    cross(":/pics/cross.png"),
    username(un),rematch(false),
    buttonsLock(true)
{
    ui->setupUi(this);
    connect(con,&TcpSocketConnection::newEvent,
            this,&ClientGamePage::handelNewEvent);



    QListWidgetItem * member_item =new QListWidgetItem(
             con->getName(false)+'\n'+
             con->getIp(false) + ':'+
             QString::number(con->getPort(false)) );

    // need to change
    member_item->setBackground(QBrush(QColor(Qt::GlobalColor::darkCyan)));
    member_item->setTextAlignment(Qt::AlignLeft);
    member_item->setIcon(admin_ic);

    ui->membersList->addItem(member_item);

    clearTheBoard();
    QPixmap backgroundImage(":/pics/back3.png");
    QPalette palette;
    palette.setBrush(QPalette::Window, backgroundImage);
    ui->boardWidget->setAutoFillBackground(true);
    ui->boardWidget->setPalette(palette);


    QPixmap blackImage(":/pics/black.png");
    QPalette paletteBlack;
    paletteBlack.setBrush(QPalette::Window, blackImage);
    ui->yourWinsLcd->setAutoFillBackground(true);
    ui->yourWinsLcd->setPalette(paletteBlack);
    ui->opWinsLcd->setAutoFillBackground(true);
    ui->opWinsLcd->setPalette(paletteBlack);
    ui->yourWinsLcd->display("00");
    ui->opWinsLcd->display("00");

    ui->yournameLabel->setText(username);
    ui->opponentname->setText("unknown");



}
void ClientGamePage::clearTheBoard()
{

    move=0;
    rematch = false;
    for(int i=0;i<9;i++){
        disablebuttons[i]=false;
    }
    ui->a1->setIcon(QPixmap(0,0));
    ui->a2->setIcon(QPixmap(0,0));
    ui->a3->setIcon(QPixmap(0,0));
    ui->b1->setIcon(QPixmap(0,0));
    ui->b2->setIcon(QPixmap(0,0));
    ui->b3->setIcon(QPixmap(0,0));
    ui->c1->setIcon(QPixmap(0,0));
    ui->c2->setIcon(QPixmap(0,0));
    ui->c3->setIcon(QPixmap(0,0));
    ui->yournameLabel->setText(username);
}
void ClientGamePage::playerMoves(int place,bool i_select)
{
    ui->yournameLabel->setText(username);
    qDebug()<<"palce : "<<place<<i_select;
    if(i_select&&buttonsLock)
    {
        qDebug()<<"its not your turn";
        return;//when player clicks on a button and it's not him turn
    }
    if(move==9)return ;
    if(!disablebuttons[place])//if the button hasn't been selected yet
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
        if(i_select){
            //this player has selected
            ui->yournameLabel->setText(ui->yournameLabel->text());
            con->sendMessage("play",QString::number(place));
            buttonsLock = true;
        }
        else
        {

            buttonsLock = false;
        }
        disablebuttons[place]=true;
        move++;
    }
}
ClientGamePage::~ClientGamePage()
{
    con->sendMessage("quit");
    delete ui;
}
void ClientGamePage::handelNewEvent(Message msg,TcpSocketConnection * connection)
{
    qDebug()<<"event:"<<msg.get_message()<<"sender"<<msg.get_sender_name() << msg.get_type();
    if(msg.get_type()=="normalmessage"&&msg.get_sender_name()!=username)
    {
        QListWidgetItem * message_item =
                new QListWidgetItem(">>> "+msg.get_sender_name()+" :"
                                    +'\n' +msg.get_message());
        message_item->setBackground(QBrush(QColor(Qt::GlobalColor::yellow)));
        message_item->setTextAlignment(Qt::AlignLeft);
        message_item->setIcon(ic);

        ui->chatList->addItem(message_item);
    }
    else if(msg.get_type()=="initmember")
    {
        QListWidgetItem * member_item =new QListWidgetItem(
                    msg.get_message());

        // need to change

        member_item->setBackground(QBrush(QColor(Qt::GlobalColor::cyan)));
        member_item->setTextAlignment(Qt::AlignLeft);
        member_item->setIcon(ic);


        ui->membersList->addItem(member_item);
    }
    else if(msg.get_type()=="start")
    {
        clearTheBoard();
        if(msg.get_sender_name()==username)
        {
            //your turn to play
            buttonsLock=false;
            ui->yournameLabel->setText(username+" <<<");
            ui->opponentname->setText(msg.get_message());
        }
        else
        {
            //other player turn
            ui->yournameLabel->setText(username);
            ui->opponentname->setText(msg.get_sender_name());
            buttonsLock=true;
        }
    }
    else if(msg.get_type()=="play"&&msg.get_sender_name()!=username)
    {
        qDebug()<<" oppent played :" <<msg.get_sender_name()<<msg.get_message();
        playerMoves(msg.get_message().toInt(),false);
        ui->yournameLabel->setText(username+" <<<");
    }
    else if(msg.get_type()=="wins")
    {

        buttonsLock= true;
        move=9;
        QListWidgetItem * message_item =
                new QListWidgetItem(">>> "+msg.get_message()+" won <<<");
        message_item->setBackground(QBrush(QColor(Qt::GlobalColor::red)));
        message_item->setTextAlignment(Qt::AlignCenter);
        ui->chatList->addItem(message_item);
        if(username==msg.get_message())
        {
            //you are the winner
            ui->yourWinsLcd->display(ui->yourWinsLcd->value()+1);
        }
        else
        {
            //your opponent is the winner
            ui->opWinsLcd->display(ui->opWinsLcd->value()+1);
        }
    }
    else if(msg.get_type()=="draw")
    {
        QListWidgetItem * message_item =
                new QListWidgetItem(">>> draw <<<");
        message_item->setBackground(QBrush(QColor(Qt::GlobalColor::red)));
        message_item->setTextAlignment(Qt::AlignCenter);
        ui->chatList->addItem(message_item);
        buttonsLock = true;
        move=9;
    }
    else if(msg.get_type()=="quit")
    {
        qDebug()<<"in quit";
        QListWidgetItem * message_item =
                new QListWidgetItem(">>> "+msg.get_sender_name()
                                    +" left <<<");
        message_item->setBackground(QBrush(QColor(Qt::GlobalColor::red)));
        message_item->setTextAlignment(Qt::AlignCenter);
        ui->chatList->addItem(message_item);
        int i=0;
        while(true)
        {
            QListWidgetItem * item=ui->membersList->item(i);
            if(item==nullptr)break;
            qDebug()<<item->text();
            if(item->text().contains(msg.get_sender_name()))
            {
                delete item;
                item=nullptr;
                break;
            }
            i++;
        }
        buttonsLock = true;
        move=9;
    }
    else if(msg.get_type()=="disconnected")
    {
        QListWidgetItem * message_item =
                new QListWidgetItem(">>> server left <<<");
        message_item->setBackground(QBrush(QColor(Qt::GlobalColor::red)));
        message_item->setTextAlignment(Qt::AlignCenter);
        ui->chatList->addItem(message_item);
        int i=0;
        while(true)
        {
            QListWidgetItem * item=ui->membersList->item(i);
            if(item==nullptr)break;
            if(item->text().contains("server"))
            {
                ui->membersList->removeItemWidget(item);
                break;
            }
        }
        buttonsLock = true;
        move=9;
    }
    else {
        qDebug()<<"wrong message in clinet";
    }
}
void ClientGamePage::on_sendButton_clicked()
{
    QString sending_message = ui->chatTextEdit->toPlainText();
    QListWidgetItem * message_item =new QListWidgetItem(sending_message);
    message_item->setBackground(QBrush(QColor(Qt::GlobalColor::cyan)));
    message_item->setTextAlignment(Qt::AlignRight);
    ui->chatList->addItem(message_item);
    ui->chatTextEdit->clear();
    //----send it to TCP socket;
    con->sendMessage("normalmessage",sending_message);
}
void ClientGamePage::on_backButton_clicked()
{
    this->~ClientGamePage();
}
void ClientGamePage::on_a1_clicked()
{
    playerMoves(0,true);
}
void ClientGamePage::on_a2_clicked()
{
    playerMoves(1,true);
}
void ClientGamePage::on_a3_clicked()
{
    playerMoves(2,true);
}
void ClientGamePage::on_b1_clicked()
{
    playerMoves(3,true);
}
void ClientGamePage::on_b2_clicked()
{
    playerMoves(4,true);
}
void ClientGamePage::on_b3_clicked()
{

    playerMoves(5,true);
}
void ClientGamePage::on_c1_clicked()
{
    playerMoves(6,true);
}
void ClientGamePage::on_c2_clicked()
{
    playerMoves(7,true);
}
void ClientGamePage::on_c3_clicked()
{
    playerMoves(8,true);
}
void ClientGamePage::on_resignButton_clicked()
{
    if(move&&move!=9)
    {
        buttonsLock=true;
        con->sendMessage("resign");
        move=0;
    }
}
void ClientGamePage::on_rematchButton_clicked()
{
    if(move&&!rematch)
    {
        con->sendMessage("rematch");
    }
    else
    {
       rematch = true;
    }
}

