#ifndef BRICK_H
#define BRICK_H

#include "rectentity.h"
#include "bonus.h"

class Brick : public RectEntity
{
public:
    Brick(float x, float y, float width, float height, sf::Color color, Arkanoid::Bricks type);
    void move();
    inline bool isAlive() const {return health_ > 0;}
    void hit();
    int getScoreIncrement();
    void activateBonus(std::vector<Bonus> &bonuses, sf::Texture texture);
    void draw(sf::RenderWindow &window) const override;
private:
    int health_{100};
    Arkanoid::Bricks type_;
    Arkanoid::Bonuses bonusType_;
    sf::Color bonusColor_;
};

#endif // BRICK_H
