#include "lasergun.h"

LaserGun::LaserGun(float x, float y, float width, float height, sf::Color gunColor,
                   sf::Color rayColor)
{

    gun_.setSize({width, height});
    gun_.setOrigin(width / 2, height / 2);
    gun_.setPosition({x, y});
    gun_.setFillColor(gunColor);
    ray_.setSize({width / 2, gun_.getPosition().y - gun_.getSize().y / 2 - 10});
    ray_.setOrigin(ray_.getSize().x / 2, ray_.getSize().y);
    ray_.setPosition({gun_.getPosition().x, gun_.getPosition().y - gun_.getSize().y / 2});
    ray_.setFillColor(rayColor);
}

void LaserGun::move(Arkanoid::Direction dir, float speed)
{
    switch(dir)
    {
        case Arkanoid::Direction::UP :
            gun_.move({0, -speed});
            ray_.move({0, -speed});
            break;
        case Arkanoid::Direction::DOWN :
            gun_.move({0, +speed});
            ray_.move({0, +speed});
            break;
        case Arkanoid::Direction::LEFT :
            gun_.move({-speed, 0});
            ray_.move({-speed, 0});
            break;
        case Arkanoid::Direction::RIGHT :
            gun_.move({+speed, 0});
            ray_.move({+speed, 0});
            break;
        default :
            break;
    }
}

void LaserGun::draw(sf::RenderWindow &window) const
{
    if(isActive_)
    {
        window.draw(gun_);
        if(isShooting_)
            window.draw(ray_);
    }
}

void LaserGun::setPosition(float x, float y)
{
    gun_.setPosition({x, y});
    ray_.setPosition({x, gun_.getPosition().y - gun_.getSize().y / 2});
}
