#include "cards.h"
#include <QRandomGenerator>

Cards::Cards()
{

}

void Cards::add(const Card &card)
{
    m_cards.insert(card);
}

void Cards::add(const Cards &cards)
{
    //合并集合
    m_cards.unite(cards.m_cards);
}

Cards &Cards::operator <<(const Card &card)
{
    add(card);
    return *this;
}

Cards &Cards::operator <<(const Cards &cards)
{
    add(cards);
    return *this;
}

void Cards::remove(const Card &card)
{
    m_cards.remove(card);
}

void Cards::remove(const Cards &cards)
{
    m_cards.subtract(cards.m_cards);
}

void Cards::remove(const QVector<Cards> &cards)
{
    for(int i=0;i<cards.size();i++){
        remove(cards.at(i));
    }
}

int Cards::CardCount()
{
    return m_cards.size();
}

bool Cards::isEmpty()
{
    return m_cards.empty();
}

void Cards::clearCard()
{
    m_cards.clear();
}

Card::CardPoint Cards::maxPoint()
{
    Card::CardPoint max=Card::Card_begin;
    for(auto it=m_cards.begin();it!=m_cards.end();it++){
        if(it->point() > max){
            max=it->point();
        }
    }
    return max;
}

Card::CardPoint Cards::minPoint()
{
    Card::CardPoint min=Card::Card_end;
    for(auto it=m_cards.begin();it!=m_cards.end();it++){
        if(it->point() < min){
            min=it->point();
        }
    }
    return min;
}

int Cards::PointCount(Card::CardPoint point)
{
    int count=0;
    for(auto it=m_cards.begin();it!=m_cards.end();it++){
        if(it->point()==point){
            count++;
        }
    }
    return count;
}

bool Cards::Contains(const Card &card)
{
    return m_cards.contains(card);
}

bool Cards::Contains(const Cards &cards)
{
    return m_cards.contains(cards.m_cards);
}

Card Cards::takeRandomcard()
{
    //生成随机数
    int num=QRandomGenerator::global()->bounded(m_cards.size());
    //只读迭代器
    QSet<Card>::const_iterator it=m_cards.constBegin();
    for(int i=0;i<num;i++,it++);
    Card card=*it;
    m_cards.erase(it);
    return card;
}

CardList Cards::toCardList(SortType type)
{
    CardList list;
    for(auto it=m_cards.begin();it!=m_cards.end();it++){
        list<<*it;
    }
    if(type==Asc){
        //qsort高版本舍弃
        std::sort(list.begin(),list.end(),lessSort);
    }
    else if(type==Desc){
        std::sort(list.begin(),list.end(),greaterSort);
    }
}


