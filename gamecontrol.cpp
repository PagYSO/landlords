#include "gamecontrol.h"
#include "QRandomGenerator"

GameControl::GameControl(QObject *parent)
    : QObject{parent}
{

}

void GameControl::PlayerInit()
{
    //玩家初始化
    m_robotLeft=new Robot("机器人A",this);
    m_robotRight=new Robot("机器人B",this);
    m_userplayer=new UserPlayer("自己",this);

    //玩家头像显示方位
    m_robotLeft->setDirection(Player::left);
    m_robotRight->setDirection(Player::right);
    m_userplayer->setDirection(Player::right);

    //性别
    Player::Sex sex;
    sex=(Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotLeft->setSex(sex);
    sex=(Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotRight->setSex(sex);
    sex=(Player::Sex)QRandomGenerator::global()->bounded(2);
    m_userplayer->setSex(sex);

    //出牌顺序
    //User
    m_userplayer->setPrevPlayer(m_robotLeft);
    m_userplayer->setNextPlayer(m_robotRight);
    //左侧机器人
    m_robotLeft->setPrevPlayer(m_robotRight);
    m_robotLeft->setNextPlayer(m_userplayer);
    //右侧机器人
    m_robotRight->setPrevPlayer(m_userplayer);
    m_robotRight->setNextPlayer(m_robotLeft);

    //指定当前玩家
    m_curplayer=m_userplayer;
}

Robot *GameControl::getLeftRobot()
{
    return m_robotLeft;
}

Robot *GameControl::getRightRobot()
{
    return m_robotRight;
}

UserPlayer *GameControl::getUserPlayer()
{
    return m_userplayer;
}

void GameControl::setCurrentPlayer(Player *player)
{
    m_curplayer=player;
}

Player *GameControl::getCurrentPlayer()
{
    return m_curplayer;
}

Player *GameControl::getPendPlayer()
{
    return m_pendplayer;
}

Cards GameControl::getPendCards()
{
    return m_pendCards;
}

void GameControl::initallCards()
{
    m_allCards.clearCard();
    for(int s=Card::Suit_begin+1;s<Card::Suit_end;s++){
        for(int p=Card::Card_begin+1;p<Card::Card_SJoker;p++){
            Card c((Card::CardPoint)p,(Card::CardSuit)s);
            m_allCards.add(c);
        }
    }
    m_allCards.add(Card(Card::Card_SJoker,Card::Suit_begin));
    m_allCards.add(Card(Card::Card_BJoker,Card::Suit_begin));
}

Card GameControl::pickOne()
{
    return m_allCards.takeRandomcard();
}

Cards GameControl::lastThree()
{
    return m_allCards;
}

void GameControl::reset()
{
    initallCards();
    //清空所有牌
    m_userplayer->clearCards();
    m_robotLeft->clearCards();
    m_robotRight->clearCards();
    //初始化出牌玩家和出的牌
    m_userplayer=nullptr;
    m_pendCards.clearCard();
}

void GameControl::startCallLord()
{
    m_curplayer->prepareCallLord();
}

void GameControl::becomLord(Player* player)
{
    player->setRole(Player::lord);
    player->PrevPlayer()->setRole(Player::farmer);
    player->NextPlayer()->setRole(Player::farmer);

    m_curplayer=player;
    player->StoreDispatchCards(m_allCards);

    m_curplayer->preparePlayHand();
}

void GameControl::clearScore()
{
    m_userplayer->setScore(0);
    m_robotLeft->setScore(0);
    m_robotRight->setScore(0);
}
