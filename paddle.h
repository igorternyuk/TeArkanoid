#ifndef PADDLE_H
#define PADDLE_H

#include "rectentity.h"
#include "lasergun.h"

class Paddle : public RectEntity
{
public:
    explicit Paddle(float x, float y, float velocity, float width, float height, sf::Color color);
    void move() override;
    Arkanoid::Direction getDirection() const {return dir_; }
    bool isGunActive() const { return gun_.isActive(); }
    bool isGunRayActive() const { return gun_.isShooting(); }
    void expand();
    void narrow();
    void resetPosition();
    void activateLaserGun();
    void desactivateLaserGun();
    void fire();
    void ceaseFire();
    void draw(sf::RenderWindow &window) const override;
private:
    Arkanoid::Direction dir_{Arkanoid::Direction::STOP};
    float velocity_;
    LaserGun gun_;
    void updateGunPosition();
};

#endif // PADDLE_H
