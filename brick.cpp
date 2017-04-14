#include "brick.h"

Brick::Brick(float x, float y, float width, float height,
             sf::Color color, Arkanoid::Bricks type) :
    RectEntity(x, y, width, height, color), type_(type)
{
    int randNum = rand() % 1000;
    if(randNum < 400)
    {
        bonusType_ = Arkanoid::Bonuses::EMPTY;
        bonusColor_ = sf::Color::White;
    }
    else if(randNum < 600)
    {
        bonusType_ = Arkanoid::Bonuses::PLAYER;
        bonusColor_ = sf::Color(128, 128, 128);
    }
    else if(randNum < 700)
    {
        bonusType_ = Arkanoid::Bonuses::EXPAND;
        bonusColor_ = sf::Color(0, 0, 255);
    }
    else if(randNum < 800)
    {
        bonusType_ = Arkanoid::Bonuses::SLOW;
        bonusColor_ = sf::Color(235, 19, 150);
    }
    else if(randNum < 900)
    {
        bonusType_ = Arkanoid::Bonuses::CATCH;
        bonusColor_ = sf::Color(0, 255, 0);
    }
    else if(randNum < 950)
    {
        bonusType_ = Arkanoid::Bonuses::LASER;
        bonusColor_ = sf::Color(255, 0, 0);
    }
    else
    {
        bonusType_ = Arkanoid::Bonuses::DIVIDE;
        bonusColor_ = sf::Color::Cyan;
    }
    /*
    BLUE (E — от англ. expand — «расширять») — увеличивает платформу в 2 раза.
    CYAN (D — от англ. divide — «делить») — из шарика вылетают ещё 2 шарика.
    RED (L — от англ. laser — «лазер») — устанавливает на платформу лазерную установку, которая может разрушать кирпичи лазером.
    PINK (S — от англ. slow — «медленный») — замедляет шар.
    GREEN (C — от англ. catch — «ловить») — шарик теперь прилипает к платформе.
    GRAY (P — от англ. player — «игрок») — дополнительная жизнь.
    */
}

void Brick::move()
{}

void Brick::hit()
{
    health_ -= static_cast<int>(type_);
}

int Brick::getScoreIncrement()
{
    int result = 0;
    //enum class Bricks {YELLOW = 100, BLUE = 50, ORANGE = 34, GREEN = 25, GRAY = 20};
    switch(type_)
    {
        case Arkanoid::Bricks::YELLOW :
            result = 10;
            break;
        case Arkanoid::Bricks::BLUE :
            result = 20;
            break;
        case Arkanoid::Bricks::ORANGE :
            result = 30;
            break;
        case Arkanoid::Bricks::GREEN :
            result = 40;
            break;
        case Arkanoid::Bricks::GRAY :
            result = 50;
            break;
        default :
            result = 0;
        break;
    }
    return result;
}

void Brick::activateBonus(std::vector<Bonus> &bonuses, sf::Texture texture)
{
    if(bonusType_ != Arkanoid::Bonuses::EMPTY)
    {
        bonuses.emplace_back(x(), bottom(), Arkanoid::bonusWidth, Arkanoid::bonusHeight,
                             0.f, Arkanoid::bonusVelocity, bonusType_, texture);
    }
}

void Brick::draw(sf::RenderWindow &window) const
{
    if(isAlive()) RectEntity::draw(window);
}
