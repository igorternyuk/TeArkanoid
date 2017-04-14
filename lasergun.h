#ifndef LASERGUN_H
#define LASERGUN_H

#include "utils.h"
#include "constants.h"

class LaserGun
{
public:
    LaserGun(float x, float y, float width, float height, sf::Color gunColor, sf::Color rayColor);
    void move(Arkanoid::Direction dir, float speed);
    float x() const {return ray_.getPosition().x; }
    float y() const {return ray_.getPosition().y; }
    bool isActive() const {return isActive_; }
    bool isShooting() const {return isShooting_; }
    void draw(sf::RenderWindow &window) const;
    void activate() {isActive_ = true; }
    void desactivate() {isActive_ = false; isShooting_ = false; }
    void activateLaserRay() { isShooting_ = isActive_ ? true : false; }
    void desactivateLaserRay() { isShooting_ = false; }
    void setPosition(float x, float y);
private:
    sf::RectangleShape gun_;
    sf::RectangleShape ray_;
    sf::Color gunColor_;
    sf::Color rayColor_;
    bool isActive_{false};
    bool isShooting_{false};
};

#endif // LASERGUN_H
