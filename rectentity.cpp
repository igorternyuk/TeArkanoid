#include "rectentity.h"

RectEntity::RectEntity(float x, float y, float width, float height, sf::Color color)
{
    rect_.setPosition({x, y});
    rect_.setSize({width, height});
    rect_.setOrigin(width / 2.f, height / 2.f);
    rect_.setFillColor(color);
}

float RectEntity::top() const
{
    return y() - rect_.getSize().y / 2.f;
}

float RectEntity::bottom() const
{
    return y() + rect_.getSize().y / 2.f;
}

float RectEntity::left() const
{
    return x() - rect_.getSize().x / 2.f;
}

float RectEntity::right() const
{
    return x() + rect_.getSize().x / 2.f;
}

void RectEntity::draw(sf::RenderWindow &window) const
{
    window.draw(rect_);
}

void RectEntity::moveOrtho(Arkanoid::Direction dir, float velocity)
{
    switch(dir)
    {
        case Arkanoid::Direction::STOP :
            return;
        case Arkanoid::Direction::UP :
            rect_.move({0.f, -velocity});
            break;
        case Arkanoid::Direction::DOWN :
            rect_.move({0.f, +velocity});
            break;
        case Arkanoid::Direction::LEFT :
            rect_.move({-velocity, 0.f});
            break;
        case Arkanoid::Direction::RIGHT :
            rect_.move({+velocity, 0.f});
            break;
    }
}
