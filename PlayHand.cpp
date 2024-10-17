#include "PlayHand.h"

PlayHand::PlayHand()
{

}

PlayHand::PlayHand(Cards &cards)
{
    classify(cards);
    judgeCardType();
}

PlayHand::PlayHand(HandType type, Card::CardPoint pt, int extra)
{
    m_type=type;
    m_pt=pt;
    m_extra=extra;
}

PlayHand::HandType PlayHand::getHandType()
{
    return m_type;
}

Card::CardPoint PlayHand::getCardPoint()
{
    return m_pt;
}

int PlayHand::getExtra()
{
    return m_extra;
}

bool PlayHand::canBeat(const PlayHand &other)
{
    if(m_type==Hand_Unknown){
        return false;
    }
    if(other.m_type==Hand_Pass){
        return true;
    }
    if(m_type == Hand_Bomb_Jokers)
    {
        return true;
    }
    if(m_type == Hand_Bomb && other.m_type >= Hand_Single && other.m_type <= Hand_Seq_Single)
    {
        return true;
    }
    if(m_type == other.m_type)
    {
        if(m_type == Hand_Seq_Pair || m_type == Hand_Seq_Single)
        {
            return m_pt > other.m_pt && m_extra == other.m_extra;
        }
        else
        {
            return m_pt > other.m_pt;
        }
    }
    return false;

}




void PlayHand::classify(Cards &cards)
{
    CardList list=cards.toCardList();
    int cardRecord[Card::Card_end];
    memset(cardRecord,0,sizeof(int)*Card::Card_end);

    for(int i=0;i<list.size();i++){
        Card c=list.at(i);
        cardRecord[c.point()]++;
    }

    m_oneCard.clear();
    m_twoCard.clear();
    m_threeCard.clear();
    m_fourCard.clear();

    for(int i=0;i<Card::Card_end;i++){
        if(cardRecord[i]==1){
            m_oneCard.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i]==2){
            m_twoCard.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i]==3){
            m_threeCard.push_back((Card::CardPoint)i);
        }
        else if(cardRecord[i]==2){
            m_fourCard.push_back((Card::CardPoint)i);
        }
    }
}

void PlayHand::judgeCardType()
{
    m_type=Hand_Unknown;
    m_pt=Card::Card_begin;
    m_extra=0;
    if(isPass()){
        m_type=Hand_Pass;
    }
}



bool PlayHand::isPass()
{
    if(m_oneCard.size()==0 && m_twoCard.empty() && m_threeCard.isEmpty() && m_fourCard.isEmpty()){
        return true;
    }
}

bool PlayHand::isSingle()
{
    if(m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPair()
{
    if(m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTriple()
{
    if(m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.size() == 1 && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTripleSingle()
{
    if(m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.size() == 1 && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isTriplePair()
{
    if(m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.size() == 1 && m_fourCard.isEmpty())
    {
        return true;
    }
    return false;
}

bool PlayHand::isPlane()
{
    if(m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.size()==2 && m_fourCard.isEmpty()){
        //排序后确保连贯，因为飞机必须是333444等
        std::sort(m_threeCard.begin(),m_threeCard.end());
        if(m_threeCard[1]-m_threeCard[0]==1 && m_threeCard[1]<Card::Card_2){
            return true;
        }
    }
}

bool PlayHand::isPlaneTwoSingle()
{
    if(m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.size() == 2 && m_fourCard.isEmpty())
    {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2 &&
                m_oneCard[0] != Card::Card_SJoker && m_oneCard[1] != Card::Card_BJoker)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoPair()
{
    if(m_oneCard.isEmpty() && m_twoCard.size() == 2 && m_threeCard.size() == 2 && m_fourCard.isEmpty())
    {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        if(m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqPair()
{
    if(m_oneCard.isEmpty() && m_twoCard.size() >= 3 && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_twoCard.begin(), m_twoCard.end());
        if(m_twoCard.last() - m_twoCard.first() == (m_twoCard.size() -1) &&
                m_twoCard.first() >= Card::Card_3 && m_twoCard.last() < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqSingle()
{
    if(m_oneCard.size() >= 5 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard.last() - m_oneCard.first() == (m_oneCard.size() -1) &&
                m_oneCard.first() >= Card::Card_3 && m_oneCard.last() < Card::Card_2)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBomb()
{
    if(m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
         return true;
    }
    return false;
}

bool PlayHand::isBombPair()
{
    if(m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
         return true;
    }
    return false;
}

bool PlayHand::isBombTwoSingle()
{
    if(m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1)
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard.first() != Card::Card_SJoker && m_oneCard.last() != Card::Card_BJoker)
        {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokers()
{
    if(m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty())
    {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if(m_oneCard.first() == Card::Card_SJoker && m_oneCard.last() == Card::Card_BJoker)
        {
            return true;
        }
    }
    return false;
}








