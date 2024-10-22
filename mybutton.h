#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>

class myButton : public QPushButton
{
    Q_OBJECT
public:
    explicit myButton(QWidget *parent = nullptr);
    void setImage(QString normal,QString hover,QString pressed);

protected:
    //鼠标按下
    void MousePressEvent(QMouseEvent* ev);
    //鼠标释放
    void MouseRealseEvent(QMouseEvent* ev);
    //鼠标进入
    void enterEvent(QEvent* ev);
    //鼠标离开
    void leaveEvent(QEvent* ev);
    //绘图
    void paintEvent(QPaintEvent* ev);


signals:
private:
    QString m_normal;
    QString m_hover;
    QString m_pressed;
    QPixmap m_pixmap;

};

#endif // MYBUTTON_H
