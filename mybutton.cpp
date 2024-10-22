#include "mybutton.h"

#include <QMouseEvent>
#include <QPainter>

myButton::myButton(QWidget *parent)
    : QPushButton{parent}
{

}

void myButton::setImage(QString normal, QString hover, QString pressed)
{
    m_normal = normal;
    m_hover = hover;
    m_pressed = pressed;
    m_pixmap.load(m_normal);
    update();
}

void myButton::MousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_pixmap.load(m_pressed);
        update();
    }
    QPushButton::mousePressEvent(ev);
}

void myButton::MouseRealseEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        m_pixmap.load(m_normal);
        update();
    }
    QPushButton::mouseReleaseEvent(ev);
}

void myButton::enterEvent(QEvent *ev)
{
    Q_UNUSED(ev)
    m_pixmap.load(m_hover);
    update();
}

void myButton::leaveEvent(QEvent *ev)
{
    Q_UNUSED(ev)
    m_pixmap.load(m_normal);
    update();
}

void myButton::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev)
    QPainter p(this);
    p.drawPixmap(rect(), m_pixmap);
}
