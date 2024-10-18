#include "buttongroup.h"
#include "ui_buttongroup.h"


ButtonGroup::ButtonGroup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::initButton()
{
    //开始游戏
    ui->start->setImage(":/images/start-1.png",":/images/start-2.png",":/images/start-3.png");
    //出牌
    ui->playcard->setImage(":/images/chupai_btn-1.png",":/images/chupai_btn-2.png",":/images/chupai_btn-3.png");
    ui->pickcards->setImage(":/images/chupai_btn-1.png",":/images/chupai_btn-2.png",":/images/chupai_btn-3.png");
    //不要
    ui->pass->setImage(":/images/pass_btn-1.png",":/images/pass_btn-2.png",":/images/pass_btn-3.png");
    //不枪
    ui->notcallLord->setImage(":/images/buqiang-1.png",":/images/buqiang-2.png",":/images/buqiang-3.png");
    //1,2,3分
    ui->onescore->setImage(":/images/1fen-1.png",":/images/1fen-2.png",":/images/1fen-3.png");
    ui->twoscore->setImage(":/images/2fen-1.png",":/images/2fen-2.png",":/images/2fen-3.png");
    ui->threescore->setImage(":/images/3fen-1.png",":/images/3fen-2.png",":/images/3fen-3.png");
    //设置按钮大小
    QVector<myButton*> btns;
    btns<<ui->start<<ui->playcard<<ui->pickcards<<ui->pass
       <<ui->notcallLord<<ui->onescore<<ui->twoscore
      <<ui->threescore;
    for(int i=0;i<btns.size();i++){
        btns[i]->setFixedSize(90,45);
    }

    connect(ui->start,&myButton::clicked,this,&ButtonGroup::startGame);
    connect(ui->playcard,&myButton::clicked,this,&ButtonGroup::PlayHand);
    connect(ui->pickcards,&myButton::clicked,this,&ButtonGroup::PlayHand);
    connect(ui->pass,&myButton::clicked,this,&ButtonGroup::pass);
    connect(ui->notcallLord,&myButton::clicked,this,[=]{
        emit betPoint(0);
    });
    connect(ui->onescore, &myButton::clicked, this, [=]()
    {
        emit betPoint(1);
    });
    connect(ui->twoscore, &myButton::clicked, this, [=]()
    {
        emit betPoint(2);
    });
    connect(ui->threescore, &myButton::clicked, this, [=]()
    {
        emit betPoint(3);
    });

}

void ButtonGroup::ChangePages(Pages type)
{
    ui->stackedWidget->setCurrentIndex(type);
}
