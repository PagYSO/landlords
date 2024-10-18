#include "scorepanel.h"
#include "ui_scorepanel.h"

ScorePanel::ScorePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScorePanel)
{
    ui->setupUi(this);
}

ScorePanel::~ScorePanel()
{
    delete ui;
}

void ScorePanel::setScore(int left, int right, int user)
{
    ui->leftlabel->setText(QString::number(left));
    ui->rightlabel->setText(QString::number(right));
    ui->mylabel->setText(QString::number(user));
}
