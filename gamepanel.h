#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "cardpanel.h"

#include <QMainWindow>

#include <QLabel>
#include <QMap>
#include <gamecontrol.h>

QT_BEGIN_NAMESPACE
namespace Ui { class GamePanel; }
QT_END_NAMESPACE

class GamePanel : public QMainWindow
{
    Q_OBJECT

public:
    GamePanel(QWidget *parent = nullptr);
    ~GamePanel();

    //初始化信息
    void gameinit();
    //更新分数面板的得分
    void updateScore();
    //切割并存储图片的数据
    void initCardMap();
    void cropImage(QPixmap &pix,int x,int y,Card& card);
    //初始化按钮组
    void initButtonGroup();
    //初始化玩家上下文环境
    void initPlayerContext();
protected:
    void paintEvent(QPaintEvent* ev);
private:
    enum CardAlign{Horizontal, Vertical};
    struct PlayerContext{
        //扑克牌显示区域
        QRect cardRect;
        //出牌区域
        QRect playHandRect;
        //扑克牌对齐方式
        CardAlign align;
        //扑克牌显示正面还是背面
        bool isFrontSide;
        //游戏过程的提示信息
        QLabel* info;
        //玩家的头像
        QLabel* roleImg;
        //玩家刚打出的牌
        Cards lastCards;
    };
private:
    Ui::GamePanel *ui;
    QPixmap m_bkimage;
    GameControl* m_gc;
    QVector<Player*> m_playerlist;
    QMap<Card,CardPanel*> m_cardMap;
    QSize m_size;
    QPixmap m_BackImage;
    QMap<Player*, PlayerContext> m_contextMap;



};
#endif // GAMEPANEL_H
