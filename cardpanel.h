#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>
#include "card.h"
#include "player.h"

class CardPanel : public QWidget
{
    Q_OBJECT
public:
    explicit CardPanel(QWidget *parent = nullptr);

    //设置图片函数
    void setImage(QPixmap &front,QPixmap &back);
    QPixmap Image();

    //扑克牌显示哪一面
    //true为正面，false为负面
    void setFrontSide(bool flag);
    bool isFrontSide();

    //标记扑克牌是否被选中
    void setSelected(bool flag);
    bool isSelected();

    //显示出正面的扑克牌的点数和花色
    void setCard(Card &card);
    Card card();

    //扑克牌的所有者
    void setOwner(Player *player);
    Player* Owner();
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
private:
    QPixmap m_front;
    QPixmap m_back;
    bool m_isFront;
    bool m_isSelected;
    Card m_card;
    Player* m_owner;
signals:

};

#endif // CARDPANEL_H
