#ifndef ENEMY_H
#define ENEMY_H

#include "rectentity.h"

class Enemy : public RectEntity
{
public:
    Enemy(float x, float y, float velocity, float width, float height, sf::Color color);
    void move() override;
    void draw(sf::RenderWindow &window) const override;
private:
    float velocity_;
    Arkanoid::Direction dir_ = Arkanoid::Direction::RIGHT;
};

#endif // ENEMY_H
