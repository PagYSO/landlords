#include "robot.h"

Robot::Robot(QObject *parent)
    : Player{parent}
{
    m_type=Player::Robot;
}
