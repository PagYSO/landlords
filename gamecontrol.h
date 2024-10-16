#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include <QObject>
#include "robot.h"
#include "userplayer.h"
#include "cards.h"

class GameControl : public QObject
{
    Q_OBJECT
public:
    explicit GameControl(QObject *parent = nullptr);

    //游戏状态
    enum GameStatus{
        //发牌
        DispatchCards,
        //叫地主
        CallingLord,
        //出牌
        PlayingHand
    };
    //玩家状态
    enum PlayerStatus{
        ThinkForLord,
        ThinkForPlayHand,
        Winning
    };

    //初始化玩家
    void PlayerInit();

    //得到玩家的实例对象
    Robot* getLeftRobot();
    Robot* getRightRobot();
    UserPlayer* getUserPlayer();

    void setCurrentPlayer(Player* player);
    Player* getCurrentPlayer();

    //得到出牌玩家与出的牌
    Player* getPendPlayer();
    Cards getPendCards();

    //初始化扑克牌
    void initallCards();

    //每次发一张牌
    Card pickOne();
    //最后三张底牌
    Cards lastThree();
    //重置卡牌数据
    void reset();

    //准备叫地主
    void startCallLord();
    //成为地主
    void becomLord(Player* player);

    //清空的分
    void clearScore();

    //处理叫地主

signals:

private:
    Robot* m_robotLeft;
    Robot* m_robotRight;
    UserPlayer* m_userplayer;
    Player* m_curplayer;
    Player* m_pendplayer;
    Cards m_pendCards;
    Cards m_allCards;
};

#endif // GAMECONTROL_H
