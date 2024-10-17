#ifndef STRATEGY_H
#define STRATEGY_H

#include "player.h"
#include "PlayHand.h"

class strategy
{
public:
    strategy(Player* player,const Cards& cards);

    Cards makeStrategy();
    Cards firstPlay();

    //根据上家的牌型获取一组更大的牌
    Cards getGreaterCards(PlayHand type);
    //判断是否打出牌
    bool whetherToBeat(Cards& cs);

    //根据指定的点数和数量，查找相同点数的卡牌。
    Cards findSamePointCards(Card::CardPoint point,int count);
    //根据数量查找卡牌，返回符合条件的卡牌集合。
    QVector<Cards> findCardsByCount(int count);
    //获取指定范围内的卡牌，通常根据点数范围。
    Cards getRangeCards(Card::CardPoint begin,Card::CardPoint end);
    //查找特定类型的卡牌，可能与出牌类型和是否需要打有关。
    QVector<Cards> findCardType(PlayHand hand,bool beat);

    //从记录中选择顺序单张牌，可能用于组合或策略选择。
    void pickSeqSingles(QVector<QVector<Cards>> &allSeqRecord, const QVector<Cards> &seqSingle, const Cards& cards);
    //从可用的单张或顺序卡牌中选择最佳组合。
    QVector<Cards> pickOptimalSeqSingles();
private:
    Player* m_player;
    Cards m_cards;
private:
    using function=Cards(strategy::*)(Card::CardPoint point);
    struct CardInfo{
        Card::CardPoint begin;
        Card::CardPoint end;
        int extra;
        bool beat;
        int number;
        int base;
        function getSeq;
    };
    QVector<Cards> getCards(Card::CardPoint point, int number);
    QVector<Cards> getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type);
    QVector<Cards> getPlane(Card::CardPoint begin);
    QVector<Cards> getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type);
    QVector<Cards> getSepPairOrSeqSingle(CardInfo &info);
    Cards getBaseSeqPair(Card::CardPoint point);
    Cards getBaseSeqSingle(Card::CardPoint point);
    QVector<Cards> getBomb(Card::CardPoint begin);

};

#endif // STRATEGY_H
