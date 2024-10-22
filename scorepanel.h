#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class ScorePanel;
}

class ScorePanel : public QWidget
{
    Q_OBJECT

public:
    enum FontColor{Black, White, Red, Blue, Green};
    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();

    //设置玩家的分
    void setScore(int left,int right,int user);
    void setMyFontColor(FontColor color);
    void setMyFontSize(int point);

private:
    Ui::ScorePanel *ui;
    QVector<QLabel*> m_list;
};

#endif // SCOREPANEL_H
