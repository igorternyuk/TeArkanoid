#include "ball.h"

Ball::Ball(float x, float y, float vx, float vy, float radius, sf::Color color) :
    RoundEntity(x, y, vx, vy, radius, color)
{}

void Ball::move()
{
    if(left() <= Arkanoid::fieldLeft || right() >= Arkanoid::fieldWidth + 10)
        this->reflectHorizontally();
    if(top() <= Arkanoid::fieldTop || bottom() >= Arkanoid::fieldHeight)
        this->reflectVertically();
    RoundEntity::move(); // Вызов метод базового класса из класса потомка
}

void Ball::reset(int level)
{
    circle_.setPosition({Arkanoid::fieldWidth / 2, Arkanoid::fieldHeight / 2});
    velocity_.x = Arkanoid::ballVelocityX;
    //std::cout << "vx = " << velocity_.x;
    velocity_.y = Arkanoid::ballVelocityY - 0.5 * level - 1; // 0.5 - т.к. слишком резво растет скорость мячика при переходе от уровня  к уровню
    //std::cout << "vy = " << velocity_.y << std::endl;
}

void Ball::reflectHorizontally()
{
    velocity_.x = -velocity_.x;
}

void Ball::reflectVertically()
{
    velocity_.y = -velocity_.y;
}

void Ball::reflectTopLeft()
{
    velocity_.x = -fabs(velocity_.x);
    velocity_.y = -fabs(velocity_.y);
}

void Ball::reflectTopRight()
{
    velocity_.x = +fabs(velocity_.x);
    velocity_.y = -fabs(velocity_.y);
}

void Ball::changeDirectionAngle(float angle_grad)
{
    float velocity_abs = sqrt(pow(velocity_.x, 2) + pow(velocity_.y, 2));
    velocity_.x = velocity_abs * cos(angle_grad * M_PI / 180);
    velocity_.y = velocity_abs * sin(angle_grad * M_PI / 180);
}

void Ball::decelerate()
{
    if(velocity_.x > Arkanoid::mimBallVelocity) --velocity_.x;
    if(velocity_.y > Arkanoid::mimBallVelocity) --velocity_.y;
}
