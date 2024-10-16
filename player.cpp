#include "player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{

}

Player::Player(QString name, QObject *parent)
{
    m_name=name;
}

void Player::setName(QString name)
{
    m_name=name;
}

QString Player::name()
{
    return m_name;
}

void Player::setRole(Role role)
{
    m_role=role;
}

Player::Role Player::role()
{
    return m_role;
}

void Player::setSex(Sex sex)
{
    m_sex=sex;
}

Player::Sex Player::sex()
{
    return m_sex;
}

void Player::setDirection(Direction dirc)
{
    m_dirc=dirc;
}

Player::Direction Player::dirc()
{
    return m_dirc;
}

void Player::setType(Type type)
{
    m_type=type;
}

Player::Type Player::type()
{
    return m_type;
}

void Player::setScore(int score)
{
    m_score=score;
}

int Player::score()
{
    return m_score;
}

void Player::setWin(bool flag)
{
    m_isWin=flag;
}

bool Player::isWin()
{
    return m_isWin;
}

void Player::setPrevPlayer(Player *player)
{
    m_prev=player;
}

void Player::setNextPlayer(Player *player)
{
    m_next=player;
}

Player *Player::PrevPlayer()
{
    return m_prev;
}

Player *Player::NextPlayer()
{
    return m_next;
}

void Player::grabLordBet(int point)
{

}

void Player::StoreDispatchCard(Card &card)
{
    m_cards.add(card);
}

void Player::StoreDispatchCards(Cards &cards)
{
    m_cards.add(cards);
}

Cards Player::getCards()
{
    return m_cards;
}

void Player::clearCards()
{
    m_cards.clearCard();
}

void Player::playHand(Cards &cards)
{
    m_cards.remove(cards);
}

void Player::setPendingInfo(Player *player, Cards &cards)
{
    m_pendplayer=player;
    m_pendcards=cards;
}

Player *Player::getPendPlayer()
{
    return m_pendplayer;
}

Cards Player::getPendCards()
{
    return m_pendcards;
}

void Player::prepareCallLord()
{

}

void Player::preparePlayHand()
{

}
