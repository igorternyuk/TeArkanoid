#ifndef BONUS_H
#define BONUS_H

#include "rectentity.h"

class Bonus : public Entity
{
public:
    Bonus(float x, float y, float width, float height, float vx, float vy,
          Arkanoid::Bonuses type, sf::Texture texture);
    inline Arkanoid::Bonuses getType() const {return type_; }
    void move();
    inline float x() const {return position_.x; }
    inline float y() const {return position_.y; }
    float top() const;
    float bottom() const;
    float left() const;
    float right() const;
    void draw(sf::RenderWindow &window) const;
    void drop();
    void accelerateVertically();
    inline bool isActive() const { return isActive_; }
    inline bool isTaken() const { return isTaken_; }
    inline void activate() {isActive_ = true; }
    inline void desactivate() {isActive_ = false; }
    void take() {isTaken_ = true; }
private:
    sf::Vector2f position_;
    sf::Vector2f velocity_;
    sf::Vector2f size_;
    Arkanoid::Bonuses type_;
    sf::Texture texture_;
    mutable sf::Vector2i source_;
    bool isActive_{true};
    bool isTaken_{false};
};

#endif // BONUS_H
