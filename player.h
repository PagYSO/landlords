#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "cards.h"

class Player : public QObject
{
    Q_OBJECT
public:
    enum Role{lord,farmer};
    enum Sex{Man,Woman};
    enum Direction{left,right};
    enum Type{Robot,User,Unknown};
    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name,QObject *parent = nullptr);

    //名字
    void setName(QString name);
    QString name();

    //角色
    void setRole(Role role);
    Role role();

    //性别
    void setSex(Sex sex);
    Sex sex();

    //方向
    void setDirection(Direction dirc);
    Direction dirc();

    //类型
    void setType(Type type);
    Type type();
    \
    //分数
    void setScore(int score);
    int score();

    void setWin(bool flag);
    bool isWin();

    //当前对象的上家与下家
    void setPrevPlayer(Player* player);
    void setNextPlayer(Player* player);
    Player* PrevPlayer();
    Player* NextPlayer();

    //枪地主
    void grabLordBet(int point);

    //存储扑克牌
    void StoreDispatchCard(Card& card);
    void StoreDispatchCards(Cards& cards);

    //得到所有牌
    Cards getCards();
    //清空所有牌
    void clearCards();
    //出牌
    void playHand(Cards& cards);

    //设置出牌的玩家已打出的牌
    void setPendingInfo(Player* player,Cards& cards);
    Player* getPendPlayer();
    Cards getPendCards();

    //虚函数
    //利用多态来实现机器人准备叫地主与准备出牌
    virtual void prepareCallLord();
    virtual void preparePlayHand();
signals:

    //权限改为受保护的，使子类也能够继承
protected:
    int m_score;
    QString m_name;
    Role m_role;
    Sex m_sex;
    Direction m_dirc;
    Type m_type;
    bool m_isWin;
    Player* m_prev;
    Player* m_next;
    Cards m_cards;//存储多张扑克牌（玩家手中的牌）
    Cards m_pendcards;//打出的牌
    Player* m_pendplayer;//谁打出的牌
};

#endif // PLAYER_H
