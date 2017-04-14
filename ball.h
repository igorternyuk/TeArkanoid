#ifndef BALL_H
#define BALL_H

#include "roundentity.h"

class Ball : public RoundEntity
{
public:
    explicit Ball(float x, float y, float vx, float vy, float radius, sf::Color color);
    //Интерфейс нашего шарика
    void move() override;
    void reset(int level);
    void reflectHorizontally();
    void reflectVertically();
    void reflectTopLeft();
    void reflectTopRight();
    void changeDirectionAngle(float angle_grad);
    void decelerate();
};

#endif // BALL_H
