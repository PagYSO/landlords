#ifndef PLAYHAND_H
#define PLAYHAND_H

#include <cards.h>
#include <card.h>



class PlayHand
{
public:
    // 出牌组合或者方式
    enum HandType
    {
        Hand_Unknown,
        Hand_Pass,

        Hand_Single,
        Hand_Pair,

        Hand_Triple,
        Hand_Triple_Single,
        Hand_Triple_Pair,

        Hand_Plane,
        Hand_Plane_Two_Single,
        Hand_Plane_Two_Pair,

        Hand_Seq_Pair,
        Hand_Seq_Single,
        Hand_Bomb,
        Hand_Bomb_Single,
        Hand_Bomb_Pair,
        Hand_Bomb_Two_Single,

        Hand_Bomb_Jokers,
        Hand_Bomb_Jokers_Single,
        Hand_Bomb_Jokers_Pair,
        Hand_Bomb_Jokers_Two_Single
    };

    PlayHand();
    explicit PlayHand(Cards& cards);
    PlayHand(HandType type, Card::CardPoint pt, int extra);

    //获取出牌类型
    HandType getHandType();
    //获取手牌点数
    Card::CardPoint getCardPoint();
    int getExtra();

    //是否出牌能胜过对手
    bool canBeat(const PlayHand& other);

private:
    void classify(Cards& cards);
    void judgeCardType();
    bool isPass();
    bool isSingle();
    bool isPair();
    bool isTriple();
    bool isTripleSingle();
    bool isTriplePair();
    bool isPlane();
    bool isPlaneTwoSingle();
    bool isPlaneTwoPair();
    bool isSeqPair();
    bool isSeqSingle();
    bool isBomb();
    bool isBombPair();
    bool isBombTwoSingle();
    bool isBombJokers();

private:
    HandType m_type;
    Card::CardPoint m_pt;
    int m_extra;
    QVector<Card::CardPoint> m_oneCard;
    QVector<Card::CardPoint> m_twoCard;
    QVector<Card::CardPoint> m_threeCard;
    QVector<Card::CardPoint> m_fourCard;
};


#endif // PLAYHAND_H
