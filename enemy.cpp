#include "enemy.h"

Enemy::Enemy(float x, float y, float velocity, float width, float height, sf::Color color) :
    RectEntity(x, y, width, height, color), velocity_(velocity)
{}

void Enemy::move()
{
    if(right() > Arkanoid::enemyRightLimit)
        dir_ = Arkanoid::Direction::LEFT;
    else if(left() < Arkanoid::enemyLeftLimit)
        dir_ = Arkanoid::Direction::RIGHT;
    moveOrtho(dir_, velocity_);
    //std::cout << "Enemy : x = " << x() << " y = " << y() << std::endl;
}

void Enemy::draw(sf::RenderWindow &window) const
{
    if(right() > Arkanoid::fieldLeft && right() < Arkanoid::fieldRight + 10)
        RectEntity::draw(window);
}
