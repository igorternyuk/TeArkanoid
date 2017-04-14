#include "bonus.h"

Bonus::Bonus(float x, float y, float width, float height, float vx, float vy, Arkanoid::Bonuses type, sf::Texture texture) :
       position_{x, y}, velocity_{vx, vy}, size_{width, height}, type_(type), texture_(texture)
{
    source_.x = 0;
    source_.y = static_cast<int>(type_);
    texture_.loadFromFile("capsules.png");
}

void Bonus::move()
{
    position_.x += velocity_.x;
    position_.y += velocity_.y;
}

float Bonus::top() const
{
    return position_.y - size_.y / 2;
}

float Bonus::bottom() const
{
    return position_.y + size_.y / 2;
}

float Bonus::left() const
{
    return position_.x - size_.x / 2;
}

float Bonus::right() const
{
    return position_.x + size_.x / 2;
}

void Bonus::draw(sf::RenderWindow &window) const
{
    static unsigned short int tick = 0;
    ++tick;
    if(tick > 15)
    {
        tick = 0;
        ++source_.x;
        if(source_.x > 1)
            source_.x = 0;
    }
    if(isActive_ && !isTaken_)
    {
        sf::Sprite sprite;
        sprite.setTexture(texture_);
        sprite.setPosition(position_);
        sprite.setTextureRect(sf::IntRect(source_.x * Arkanoid::bonusWidth,
        source_.y * Arkanoid::bonusHeight, Arkanoid::bonusWidth, Arkanoid::bonusHeight));
        window.draw(sprite);
    }
}

void Bonus::accelerateVertically()
{
    velocity_.y += Arkanoid::gravity;
}

void Bonus::drop()
{
    static unsigned short int tick = 0;
    ++tick;
    if(y() <= Arkanoid::fieldHeight)
    {
        if(tick > 30)
        {
            accelerateVertically();
            tick = 0;
        }
        position_.y += velocity_.y;
    }
}
