#include "cardpanel.h"
#include <QPainter>

CardPanel::CardPanel(QWidget *parent)
    : QWidget{parent}
{
    m_isFront=true;
}

void CardPanel::setImage(QPixmap &front, QPixmap &back)
{
    m_front=front;
    m_back=back;

    //制定图片大小
    setFixedSize(m_front.size());

    update();
}

QPixmap CardPanel::Image()
{
    return m_front;
}

void CardPanel::setFrontSide(bool flag)
{
    m_isFront=flag;
}

bool CardPanel::isFrontSide()
{
    return m_isFront;
}

void CardPanel::setSelected(bool flag)
{
    m_isSelected=true;
}

bool CardPanel::isSelected()
{
    return m_isSelected;
}

void CardPanel::setCard(Card &card)
{
    m_card=card;
}

Card CardPanel::card()
{
    return m_card;
}

void CardPanel::setOwner(Player *player)
{
    m_owner=player;
}

Player *CardPanel::Owner()
{
    return m_owner;
}


void CardPanel::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    if(m_isFront){
        p.drawPixmap(rect(),m_front);
    }
    else{
        p.drawPixmap(rect(),m_back);
    }
}

void CardPanel::mousePressEvent(QMouseEvent *event)
{

}
