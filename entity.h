#ifndef ENTITY_H
#define ENTITY_H

#include "utils.h"
#include "constants.h"

class Entity
{
public:
    explicit Entity();
    virtual void move() = 0;
    virtual float x() const = 0;
    virtual float y() const = 0;
    virtual float top() const = 0;
    virtual float bottom() const = 0;
    virtual float left() const = 0;
    virtual float right() const = 0;
    virtual void draw(sf::RenderWindow &window) const = 0;
};

#endif // ENTITY_H
