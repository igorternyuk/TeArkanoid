#include "paddle.h"

Paddle::Paddle(float x, float y, float velocity, float width, float height, sf::Color color) :
    RectEntity(x, y, width, height, color), velocity_(velocity),
    gun_{x, y - Arkanoid::laserGunHeight / 2 - height / 2, Arkanoid::laserGunWidth, Arkanoid::laserGunHeight,
         sf::Color::Green, sf::Color::Red}
{}

void Paddle::move()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && left() > Arkanoid::fieldLeft)
        dir_ = Arkanoid::Direction::LEFT;
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && right() < Arkanoid::fieldRight)
        dir_ = Arkanoid::Direction::RIGHT;
    else
        dir_ = Arkanoid::Direction::STOP;
    this->moveOrtho(dir_, velocity_);
    gun_.move(dir_, velocity_);
}

void Paddle::expand()
{
    float newWidth = rect_.getSize().x + Arkanoid::paddleWidthIncrement;
    if(newWidth <= 0.8 * Arkanoid::fieldWidth)
    {
        rect_.setOrigin({-rect_.getSize().x / 2, -rect_.getSize().y / 2});
        //Переставляем начало координат в левый верхний угол
        rect_.setSize({newWidth, rect_.getSize().y});
        resetPosition();
    }
}

void Paddle::narrow()
{
   rect_.setOrigin({-rect_.getSize().x / 2, -rect_.getSize().y / 2});
   rect_.setSize({Arkanoid::paddleWidth, rect_.getSize().y});
   rect_.setOrigin({rect_.getSize().x / 2, rect_.getSize().y / 2});
   updateGunPosition();
}

void Paddle::resetPosition()
{
    rect_.setOrigin({rect_.getSize().x / 2, rect_.getSize().y / 2});
    rect_.setPosition({Arkanoid::fieldWidth / 2, Arkanoid::paddleTop});
    updateGunPosition();
}

void Paddle::activateLaserGun()
{
    gun_.activate();
}

void Paddle::desactivateLaserGun()
{
    gun_.desactivate();
}

void Paddle::fire()
{
    gun_.activateLaserRay();
}

void Paddle::ceaseFire()
{
    gun_.desactivateLaserRay();
}

void Paddle::draw(sf::RenderWindow &window) const
{
    RectEntity::draw(window);
    gun_.draw(window);
}

void Paddle::updateGunPosition()
{
    gun_.setPosition(x(), top() - Arkanoid::laserGunHeight / 2);
}
