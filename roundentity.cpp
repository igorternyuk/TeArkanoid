#include "roundentity.h"

RoundEntity::RoundEntity(float x, float y, float vx, float vy, float radius, sf::Color color) :
    velocity_{vx, vy}
{
    circle_.setPosition({x, y});
    circle_.setRadius(radius);
    circle_.setFillColor(color);
    circle_.setOrigin(radius, radius);
}

void RoundEntity::move()
{
    circle_.move({vx(), vy()});
}

float RoundEntity::top() const
{
    return y() - circle_.getRadius();
}

float RoundEntity::bottom() const
{
    return y() + circle_.getRadius();
}

float RoundEntity::left() const
{
    return x() - circle_.getRadius();
}

float RoundEntity::right() const
{
    return x() + circle_.getRadius();
}

void RoundEntity::draw(sf::RenderWindow &window) const
{
    window.draw(circle_);
}
