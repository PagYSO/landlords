#include "gamepanel.h"
#include "ui_gamepanel.h"
#include <QRandomGenerator>

#include <QPainter>

GamePanel::GamePanel(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GamePanel)
{
    ui->setupUi(this);

    ui->btn_Group->initButton();
    ui->btn_Group->ChangePages(ButtonGroup::Start);

    //背景图
    int num=QRandomGenerator::global()->bounded(10);
    QString path=QString(":/images/background-%1.png").arg(num+1);
    m_bkimage.load(path);
    //背景的标题与大小
    this->setWindowTitle("欢乐斗地主");
    this->setFixedSize(1000,650);
    //游戏控制类实例化
    gameinit();
    //更新玩家的分
    updateScore();
    //切割游戏图片
    initCardMap();
    //初始化按钮组
    //初始化玩家在窗口的上下文环境



}

GamePanel::~GamePanel()
{
    delete ui;
}

void GamePanel::gameinit()
{
    m_gc=new GameControl(this);
    m_gc->PlayerInit();
    //得到三个玩家的实例对象
    Robot* LeftRobot=m_gc->getLeftRobot();
    Robot* RightRobot=m_gc->getRightRobot();
    Player* userPlayer=m_gc->getUserPlayer();
    //存储
    m_playerlist<<LeftRobot<<RightRobot<<userPlayer;
}

void GamePanel::updateScore()
{
    ui->scorePanel->setScore(m_playerlist[0]->score(),
                            m_playerlist[1]->score(),
            m_playerlist[2]->score());
}

void GamePanel::initCardMap()
{
    //加载大图
    QPixmap pixmap(":/images/card.png");
    //计算每张图片的大小
    m_size.setWidth(pixmap.width()/13);
    m_size.setHeight(pixmap.height()/5);
    //取出背景图
    m_BackImage=pixmap.copy(2*m_size.width(),4*m_size.height(),m_size.width(),m_size.height());
    //取出普通花色
    for(int i=0,suit=Card::CardSuit::Suit_begin+1;suit<Card::CardSuit::Suit_end;i++,suit++){
        for(int j=0,point=Card::CardPoint::Card_begin+1;point<Card::CardPoint::Card_SJoker;j++,point++){
            Card card((Card::CardPoint)point,(Card::CardSuit)suit);
            //裁减图片
            cropImage(pixmap,j*m_size.width(),i*m_size.height(),card);
        }
    }
    //取出大小王
    Card c;
    c.setPoint(Card::CardPoint::Card_SJoker);
    c.setSuit(Card::CardSuit::Suit_begin);
    cropImage(pixmap,0,4*m_size.height(),c);

    c.setPoint(Card::CardPoint::Card_BJoker);
    c.setSuit(Card::CardSuit::Suit_begin);
    cropImage(pixmap,m_size.width(),4*m_size.height(),c);

}

void GamePanel::cropImage(QPixmap &pix, int x, int y,Card& card)
{
    QPixmap sub=pix.copy(x,y,m_size.width(),m_size.height());
    CardPanel* panel=new CardPanel(this);
    panel->setImage(sub,m_BackImage);
    panel->setCard(card);
    panel->hide();
    m_cardMap.insert(card,panel);
}

void GamePanel::initButtonGroup()
{
    ui->btn_Group->initButton();
    ui->btn_Group->ChangePages(ButtonGroup::Start);

    connect(ui->btn_Group, &ButtonGroup::startGame, this, [=](){
        ui->btn_Group->ChangePages(ButtonGroup::Empty);
        m_gc->clearScore();
        updatePlayerScore();
        gameStatusPrecess(GameControl::DispatchCards);
        m_bgm->startBGM(80);
    });
    connect(ui->btn_Group, &ButtonGroup::PlayHand, this, &GamePanel::onUserPlayHand);
    connect(ui->btn_Group, &ButtonGroup::pass, this, &GamePanel::onUserPass);
    connect(ui->btn_Group, &ButtonGroup::betPoint, this, [=](int bet){
        m_gc->getUserPlayer()->grabLordBet(bet);
        ui->btn_Group->ChangePages(ButtonGroup::Empty);
    });

}

void GamePanel::initPlayerContext()
{
    //放置扑克牌的区域
    const QRect cardsRect[] =
    {
        QRect(90, 130, 100, height() - 200),//左侧机器人
        QRect(rect().right() - 190, 130, 100, height() - 200),//右侧机器人
        QRect(250, rect().bottom() - 120, width() - 500, 100)//自己
    };
    //出牌的区域
    const QRect playHandRect[] =
    {
        QRect(260, 150, 100, 100),
        QRect(rect().right() - 360, 150, 100, 100),
        QRect(150, rect().bottom() - 290, width() - 300, 105)
    };
    //头像的区域
    const QPoint roleImgPos[] =
    {
        QPoint(cardsRect[0].left()-80, cardsRect[0].height() / 2 + 20),
        QPoint(cardsRect[1].right()+10, cardsRect[1].height() / 2 + 20),
        QPoint(cardsRect[2].right()-10, cardsRect[2].top() - 10)
    };

    int index = m_playerlist.indexOf(m_gc->getUserPlayer());
    for(int i=0; i<m_playerlist.size(); ++i)
    {
        PlayerContext context;
        context.align = i==index ? Horizontal : Vertical;
        context.isFrontSide = i==index ? true : false;
        context.cardRect = cardsRect[i];
        context.playHandRect = playHandRect[i];
        context.info = new QLabel(this);
        context.info->resize(160, 98);
        context.info->hide();
        QRect rect = playHandRect[i];
        QPoint pt(rect.left() + (rect.width() - context.info->width()) / 2,
                  rect.top() + (rect.height() - context.info->height()) / 2);
        context.info->move(pt);
        context.roleImg = new QLabel(this);
        context.roleImg->resize(84, 120);
        context.roleImg->hide();
        context.roleImg->move(roleImgPos[i]);
        m_contextMap.insert(m_playerlist.at(i), context);
    }

}

void GamePanel::paintEvent(QPaintEvent *ev)
{
    QPainter p(this);
    p.drawPixmap(rect(),m_bkimage);
}

