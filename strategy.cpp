#include "strategy.h"
#include <QMap>

strategy::strategy(Player* player,const Cards& cards)
{
    m_player=player;
    m_cards=cards;
}

Cards strategy::makeStrategy()
{
    Player* pendPlayer=m_player->getPendPlayer();
    Cards pendCards=m_player->getPendCards();

    if(pendPlayer==m_player || pendPlayer==nullptr){
        return firstPlay();
    }
    else{
        PlayHand type(pendCards);
        Cards beatCards=getGreaterCards(type);
        bool shouldBeat=whetherToBeat(beatCards);
        if(shouldBeat){
            return beatCards;
        }
        else{
            return Cards();
        }
    }
    return Cards();
}

Cards strategy::firstPlay()
{
    PlayHand hand(m_cards);
    if(hand.getHandType()!=PlayHand::Hand_Unknown){
        return m_cards;
    }
    QVector<Cards> optimalSeq=pickOptimalSeqSingles();
    if(!optimalSeq.isEmpty()){
        int baseNum=findCardsByCount(1).size();
        Cards save=m_cards;
        save.remove(optimalSeq);
        int lastNum=strategy(m_player,save).findCardsByCount(1).size();
        if(baseNum>lastNum){
            return optimalSeq[0];
        }
    }

    bool hasPlane,hasTriple,hasPair;
    hasPair=hasPlane=hasTriple=false;
    Cards backup=m_cards;

    QVector<Cards> bombArray=findCardType(PlayHand(PlayHand::Hand_Bomb,Card::Card_begin,0),false);
    backup.remove(bombArray);
    QVector<Cards> planeArray = strategy(m_player, backup).findCardType(PlayHand(PlayHand::Hand_Plane, Card::Card_begin, 0), false);
    if(!planeArray.isEmpty()){
        hasPlane=true;
        backup.remove(planeArray);
    }

    QVector<Cards> seqTripleArray = strategy(m_player, backup).findCardType(PlayHand(PlayHand::Hand_Triple, Card::Card_begin, 0), false);
    if(!seqTripleArray.isEmpty())
    {
        hasTriple = true;
        backup.remove(seqTripleArray);
    }

    QVector<Cards> seqPairArray = strategy(m_player, backup).findCardType(PlayHand(PlayHand::Hand_Seq_Pair, Card::Card_begin, 0), false);
    if(!seqPairArray.isEmpty())
    {
        hasPair = true;
        backup.remove(seqPairArray);
    }

    if(hasPair){
        Cards maxPair;
        for(int i=0;i<seqPairArray.size();i++){
            if(seqPairArray[i].CardCount()>maxPair.CardCount()){
                maxPair=seqPairArray[i];
            }
        }
        return maxPair;
    }

    if(hasPlane){
        bool twoPairFond=false;
        QVector<Cards> pairArray;
        for(Card::CardPoint point=Card::Card_3;point<=Card::Card_10;point=Card::CardPoint(point+1)){
            Cards pair=strategy(m_player,backup).findSamePointCards(point,2);
            if(!pair.isEmpty()){
                pairArray.push_back(pair);
                if(pairArray.size()==2){
                    twoPairFond=true;
                    break;
                }
            }
        }
        if(twoPairFond){
            Cards tmp=planeArray[0];
            tmp.add(pairArray);
            return tmp;
        }
        else{
            bool twoSingleFond=false;
            QVector<Cards> singleArray;
            for(Card::CardPoint point = Card::Card_3; point <= Card::Card_10; point = Card::CardPoint(point + 1))
            {
                if(backup.PointCount(point)==1)
                {
                    Cards single = strategy(m_player, backup).findSamePointCards(point, 1);
                    if(!single.isEmpty())
                    {
                        singleArray.push_back(single);
                        if(singleArray.size() == 2)
                        {
                            twoSingleFond = true;
                            break;
                        }
                    }
                }
            }
            if(twoSingleFond)
            {
                Cards tmp = planeArray[0];
                tmp.add(singleArray);
                return tmp;
            }
            else
            {
                return planeArray[0];
            }
        }
    }

    if(hasTriple)
    {
        if(PlayHand(seqTripleArray[0]).getCardPoint() < Card::Card_A)
        {
            for(Card::CardPoint point = Card::Card_3; point <= Card::Card_A; point = Card::CardPoint(point+1))
            {
                int PointCount = backup.PointCount(point);
                if(PointCount == 1)
                {
                    Cards single = strategy(m_player, backup).findSamePointCards(point, 1);
                    Cards tmp = seqTripleArray[0];
                    tmp.add(single);
                    return tmp;
                }
                else if(PointCount == 2)
                {
                    Cards pair = strategy(m_player, backup).findSamePointCards(point, 2);
                    Cards tmp = seqTripleArray[0];
                    tmp.add(pair);
                    return tmp;
                }
            }
        }
        return seqTripleArray[0];
    }
    Player* nextPlayer = m_player->NextPlayer();
    if(nextPlayer->getCards().CardCount() == 1 && m_player->role()!= nextPlayer->role())
    {
        for(Card::CardPoint point = Card::CardPoint(Card::Card_end-1);
            point >= Card::Card_3; point = Card::CardPoint(point-1))
        {
            int PointCount = backup.PointCount(point);
            if(PointCount == 1)
            {
                Cards single = strategy(m_player, backup).findSamePointCards(point, 1);
                return single;
            }
            else if(PointCount == 2)
            {
                Cards pair = strategy(m_player, backup).findSamePointCards(point, 2);
                return pair;
            }
        }
    }
    else
    {
        for(Card::CardPoint point =  Card::Card_3;
            point < Card::Card_end; point = Card::CardPoint(point+1))
        {
            int PointCount = backup.PointCount(point);
            if(PointCount == 1)
            {
                Cards single = strategy(m_player, backup).findSamePointCards(point, 1);
                return single;
            }
            else if(PointCount == 2)
            {
                Cards pair = strategy(m_player, backup).findSamePointCards(point, 2);
                return pair;
            }
        }
    }
    return Cards();
}

Cards strategy::getGreaterCards(PlayHand type)
{
    Player* pendPlayer=m_player->getPendPlayer();
    if(pendPlayer != nullptr && pendPlayer->role() != m_player->role() && pendPlayer->getCards().CardCount() <= 3)
    {
        QVector<Cards> bombs = findCardsByCount(4);
        for(int i=0; i<bombs.size(); ++i)
        {
            if(PlayHand(bombs[i]).canBeat(type))
            {
                return bombs[i];
            }
        }
        Cards sj = findSamePointCards(Card::Card_SJoker, 1);
        Cards bj = findSamePointCards(Card::Card_BJoker, 1);
        if(!sj.isEmpty() && !bj.isEmpty())
        {
            Cards jokers;
            jokers << sj << bj;
            return jokers;
        }
    }
    Player* nextPlayer = m_player->NextPlayer();
    Cards remain = m_cards;
    remain.remove(strategy(m_player, remain).pickOptimalSeqSingles());


    auto beatCard = std::bind([=](const Cards & cards){
        QVector<Cards> beatCardsArray = strategy(m_player, cards).findCardType(type, true);
        if(!beatCardsArray.isEmpty())
        {
            if(m_player->role() != nextPlayer->role() && nextPlayer->getCards().CardCount() <= 2)
            {
                return beatCardsArray.back();
            }
            else
            {
                return beatCardsArray.front();
            }
        }
        return Cards();
    }, std::placeholders::_1);

    Cards cs;
    if(!(cs = beatCard(remain)).isEmpty())
    {
        return cs;
    }
    else
    {
        if(!(cs = beatCard(m_cards)).isEmpty()) return cs;
    }
    return Cards();
}

bool strategy::whetherToBeat(Cards &cs)
{
    if(cs.isEmpty()){
        return false;
    }
    Player* pendPlayer=m_player->getPendPlayer();
    if(m_player->role()==pendPlayer->role()){
        Cards left=m_cards;
        left.remove(cs);
        if(PlayHand(left).getHandType()!=PlayHand::Hand_Unknown){
            return true;
        }
        Card::CardPoint basePoint=PlayHand(cs).getCardPoint();
        if(basePoint == Card::Card_2 || basePoint == Card::Card_SJoker || basePoint == Card::Card_BJoker){
            return false;
        }
    }
    else{
        PlayHand myHand(cs);
        if((myHand.getHandType() == PlayHand::Hand_Triple_Single || myHand.getHandType() == PlayHand::Hand_Triple_Pair)
                && myHand.getCardPoint() == Card::Card_2)
        {
            return false;
        }

        if(myHand.getHandType() == PlayHand::Hand_Pair && myHand.getCardPoint() == Card::Card_2
                && pendPlayer->getCards().CardCount() >= 10 && m_player->getCards().CardCount() >= 5)
        {
            return false;
        }
    }
    return true;
}

Cards strategy::findSamePointCards(Card::CardPoint point, int count)
{
    if(count < 1 || count > 4)
    {
        return Cards();
    }
    if(point == Card::Card_SJoker || point == Card::Card_BJoker)
    {
        if(count > 1)
        {
            return Cards();
        }

        Card card;
        card.setPoint(point);
        card.setSuit(Card::Suit_begin);
        if(m_cards.Contains(card))
        {
            Cards cards;
            cards.add(card);
            return cards;
        }
        return Cards();
    }

    int findCount = 0;
    Cards findCards;
    for(int suit = Card::Suit_begin+1; suit < Card::Suit_end; suit++)
    {
        Card card;
        card.setPoint(point);
        card.setSuit((Card::CardSuit)suit);
        if(m_cards.Contains(card))
        {
            findCount ++;
            findCards.add(card);
            if(findCount == count)
            {
                return findCards;
            }
        }
    }
    return Cards();
}

QVector<Cards> strategy::findCardsByCount(int count)
{
    if(count < 1 || count > 4)
    {
        return QVector<Cards>();
    }

    QVector<Cards> cardsArray;
    for(Card::CardPoint point = Card::Card_3; point < Card::Card_end; point = (Card::CardPoint)(point+1))
    {
        if(m_cards.PointCount(point) == count)
        {
            Cards cs;
            cs << findSamePointCards(point, count);
            cardsArray << cs;
        }
    }
    return cardsArray;
}

Cards strategy::getRangeCards(Card::CardPoint begin, Card::CardPoint end)
{
    Cards rangeCards;
    for(Card::CardPoint point = begin; point < end; point = (Card::CardPoint)(point+1))
    {
        int count = m_cards.PointCount(point);
        Cards cs = findSamePointCards(point, count);
        rangeCards << cs;
    }
    return rangeCards;
}

QVector<Cards> strategy::findCardType(PlayHand hand, bool beat)
{
    PlayHand::HandType type = hand.getHandType();
    Card::CardPoint point = hand.getCardPoint();
    int extra = hand.getExtra();

    Card::CardPoint beginPoint = beat ? Card::CardPoint(point + 1) : Card::Card_3;

    switch(type)
    {
    case PlayHand::Hand_Single:
        return getCards(beginPoint, 1);
    case PlayHand::Hand_Pair:
        return getCards(beginPoint, 2);
    case PlayHand::Hand_Triple:
        return getCards(beginPoint, 3);
    case PlayHand::Hand_Triple_Single:
        return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Triple_Pair:
        return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Pair);
    case PlayHand::Hand_Plane:
        return getPlane(beginPoint);
    case PlayHand::Hand_Plane_Two_Single:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Plane_Two_Pair:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::Hand_Pair);
    case PlayHand::Hand_Seq_Pair:
    {
        CardInfo info;
        info.begin = beginPoint;
        info.end = Card::Card_Q;
        info.number = 2;
        info.base = 3;
        info.extra = extra;
        info.beat = beat;
        info.getSeq = &strategy::getBaseSeqPair;
        return getSepPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Seq_Single:
    {
        CardInfo info;
        info.begin = beginPoint;
        info.end = Card::Card_10;
        info.number = 1;
        info.base = 5;
        info.extra = extra;
        info.beat = beat;
        info.getSeq = &strategy::getBaseSeqSingle;
        return getSepPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Bomb:
        return getBomb(beginPoint);
    default:
        return QVector<Cards>();
    }
}

void strategy::pickSeqSingles(QVector<QVector<Cards>> &allSeqRecord, const QVector<Cards> &seqSingle, const Cards &cards)
{
    QVector<Cards> allSeq = strategy(m_player, cards).findCardType(PlayHand(PlayHand::Hand_Seq_Single, Card::Card_begin, 0), false);
    if(allSeq.isEmpty())
    {
        allSeqRecord << seqSingle;
    }
    else
    {
        Cards saveCards = cards;
        for(int i=0; i<allSeq.size(); ++i)
        {
            Cards aScheme = allSeq.at(i);
            Cards temp = saveCards;
            temp.remove(aScheme);

            QVector<Cards> seqArray = seqSingle;
            seqArray << aScheme;

            pickSeqSingles(allSeqRecord, seqArray, temp);
        }
    }
}

QVector<Cards> strategy::pickOptimalSeqSingles()
{
    QVector<QVector<Cards>> seqRecord;
    QVector<Cards> seqSingles;
    Cards save = m_cards;
    save.remove(findCardsByCount(4));
    save.remove(findCardsByCount(3));
    pickSeqSingles(seqRecord, seqSingles, save);
    if(seqRecord.isEmpty())
    {
        return QVector<Cards>();
    }

    QMap<int, int> seqMarks;
    for(int i=0; i<seqRecord.size(); ++i)
    {
        Cards backupCards = m_cards;
        QVector<Cards> seqArray = seqRecord[i];
        backupCards.remove(seqArray);

        QVector<Cards> singleArray = strategy(m_player, backupCards).findCardsByCount(1);

        CardList cardList;
        for(int j=0; j<singleArray.size(); ++j)
        {
            cardList << singleArray[j].toCardList();
        }
        int mark = 0;
        for(int j=0; j<cardList.size(); ++j)
        {
            mark += cardList[j].point() + 15;
        }
        seqMarks.insert(i, mark);
    }

    int value = 0;
    int comMark = 1000;
    auto it = seqMarks.constBegin();
    for(;it!=seqMarks.constEnd(); ++it)
    {
        if(it.value() < comMark)
        {
            comMark = it.value();
            value = it.key();
        }
    }

    return seqRecord[value];
}

QVector<Cards> strategy::getCards(Card::CardPoint point, int number)
{
    QVector<Cards> findCardsArray;
    for(Card::CardPoint pt=point; pt < Card::Card_end; pt = (Card::CardPoint)(pt + 1))
    {
        if(m_cards.PointCount(pt) == number)
        {
            Cards cs = findSamePointCards(pt, number);
            findCardsArray << cs;
        }
    }
    return findCardsArray;
}


QVector<Cards> strategy::getSepPairOrSeqSingle(CardInfo &info)
{
    QVector<Cards> findCardsArray;
    if(info.beat){
        for(Card::CardPoint point=info.begin;point<=info.end;point=(Card::CardPoint)(point+1)){
            bool found=false;
            Cards seqCards;
            for(int i=0;i<info.extra;i++)
            {
                Cards cards = findSamePointCards((Card::CardPoint)(point + i), info.number);
                if(cards.isEmpty() || (point + info.extra >= Card::Card_2))
                {
                    found = false;
                    seqCards.clearCard();
                    break;
                }
                else
                {
                    seqCards << cards;
                }
            }
            if(found)
            {
                findCardsArray << seqCards;
                return findCardsArray;
            }
        }
    }
    else
    {
        for(Card::CardPoint point = info.begin; point <= info.end; point = (Card::CardPoint)(point+1))
        {
            Cards baseSeq = (this->*info.getSeq)(point);
            if(baseSeq.isEmpty())
            {
                continue;
            }
            findCardsArray << baseSeq;

            int followed = info.base;
            Cards alreadyFollowedCards;

            while(true)
            {
                Card::CardPoint followedPoint = Card::CardPoint(point + followed);
                if(followedPoint >= Card::Card_2)
                {
                    break;
                }
                Cards follwedCards = findSamePointCards(followedPoint, info.number);
                if(follwedCards.isEmpty())
                {
                    break;
                }
                else
                {
                    alreadyFollowedCards << follwedCards;
                    Cards newSeq = baseSeq;
                    newSeq << alreadyFollowedCards;
                    findCardsArray << newSeq;
                    followed++;
                }
            }
        }
    }
    return findCardsArray;
}

Cards strategy::getBaseSeqSingle(Card::CardPoint point)
{
    Cards cards0 = findSamePointCards(point, 1);
    Cards cards1 = findSamePointCards((Card::CardPoint)(point+1), 1);
    Cards cards2 = findSamePointCards((Card::CardPoint)(point+2), 1);
    Cards cards3 = findSamePointCards((Card::CardPoint)(point+3), 1);
    Cards cards4 = findSamePointCards((Card::CardPoint)(point+4), 1);
    Cards baseSeq;
    if(!cards0.isEmpty() && !cards1.isEmpty() && !cards2.isEmpty() && !cards3.isEmpty() && !cards4.isEmpty())
    {
        baseSeq << cards0 << cards1 << cards2 << cards3 << cards4;
    }
    return baseSeq;
}

QVector<Cards> strategy::getBomb(Card::CardPoint begin)
{
    QVector<Cards> findcardsArray;
    for(Card::CardPoint point = begin; point < Card::Card_end; point = (Card::CardPoint)(point + 1))
    {
        Cards cs = findSamePointCards(point, 4);
        if(!cs.isEmpty())
        {
            findcardsArray << cs;
        }
    }
    return findcardsArray;
}



