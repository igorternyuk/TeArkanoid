#ifndef RECTENTITY_H
#define RECTENTITY_H

#include "entity.h"
#include "constants.h"

class RectEntity : public Entity
{
public:
    explicit RectEntity(float x, float y, float width, float height, sf::Color color);
    virtual void move() = 0;
    void moveOrtho(Arkanoid::Direction dir, float velocity);
    inline float x() const {return rect_.getPosition().x; }
    inline float y() const {return rect_.getPosition().y; }
    float top() const;
    float bottom() const;
    float left() const;
    float right() const;
    virtual void draw(sf::RenderWindow &window) const;
protected:
    sf::RectangleShape rect_;
};

#endif // RECTENTITY_H
