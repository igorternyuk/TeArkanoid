#ifndef ROUNDENTITY_H
#define ROUNDENTITY_H

#include "entity.h"

class RoundEntity : public Entity
{
public:
    explicit RoundEntity(float x, float y, float vx, float vy, float radius, sf::Color color);
    virtual void move();
    inline float x() const {return circle_.getPosition().x; }
    inline float y() const {return circle_.getPosition().y; }
    float top() const;
    float bottom() const;
    float left() const;
    float right() const;
    virtual void draw(sf::RenderWindow &window) const;
    inline float radius() const {return circle_.getRadius(); }
    inline float vx() const {return velocity_.x; }
    inline float vy() const {return velocity_.y; }    
protected:
    sf::CircleShape circle_;
    sf::Vector2f velocity_;
};

#endif // ROUNDENTITY_H
