#ifndef GAME_H
#define GAME_H

#include "utils.h"
#include "constants.h"
#include "paddle.h"
#include "lasergun.h"
#include "ball.h"
#include "brick.h"
#include "bonus.h"
#include "enemy.h"

class Game
{
public:
    Game();
    void go();
private:
    int numLives_{Arkanoid::playerLivesNumber};
    int score_{0};
    int level_{1};
    float volume_{50.f};
    enum class GameState {PLAY, GOAL, PAUSE, GAME_OVER, VICTORY};
    GameState gameState_{GameState::PLAY};
    Paddle paddle_;
    std::vector<Ball> balls_;
    std::vector<Brick> bricks_;
    std::vector<Bonus> bonuses_;
    std::vector<Enemy> enemies_;
    sf::Font font_;
    sf::Texture capsulesTexture_;
    //Buttons
    sf::Texture newGameButtonTexture_;
    sf::Texture pauseButtonTexture_;
    sf::Texture exitButtonTexture_;
    sf::CircleShape newGameButton_;
    sf::CircleShape pauseButton_;
    sf::CircleShape exitButton_;
    //Sounds
    sf::SoundBuffer ballBrick_;
    sf::Sound sound_ballBrick_;
    sf::SoundBuffer paddleBall_;
    sf::Sound sound_paddleBall_;
    sf::SoundBuffer ballEnemy_;
    sf::Sound sound_BallEnemy_;
    sf::SoundBuffer collectBonus_;
    sf::Sound sound_collectBonus_;
    sf::SoundBuffer collectPoints_;
    sf::Sound sound_collectPoints_;
    sf::SoundBuffer laser_;
    sf::Sound sound_laser_;
    sf::SoundBuffer goal_;
    sf::Sound sound_Goal_;
    sf::SoundBuffer nextLevel_;
    sf::Sound sound_nextLevel_;
    sf::SoundBuffer defeat_;
    sf::Sound sound_Defeat_;
    sf::SoundBuffer victory_;
    sf::Sound sound_Victory_;
    //Background
    sf::Music backgroundMusic_;
    template < class T1, class T2 > bool isIntersecting(T1 &mA, T2 &mB);
    template < class T1, class T2 > void resolveCollision(T1 &mA, T2 &mB);
    template < class T> bool isOnTheField(T &entity);
    void resolveCollision(Ball &ball, Paddle &paddle);
    void resolveCollision(Bonus &bonus, Paddle &paddle);
    void resolveCollision(Brick &brick, Paddle &paddle);
    void prepareNewGame(int level);
    void pauseGame();
    void createBricks();
    void createButtons();
    void createEnemies();
    void loadSounds();
    void drawField(sf::RenderWindow &window) const;
    void drawGameInfo(sf::RenderWindow &window) const;
    void drawCharacters(sf::RenderWindow &window) const;
    void drawButtons(sf::RenderWindow &window) const;
    void drawMessage(sf::RenderWindow &window, const std::string &message, const sf::Font &font,
                     int fontSize, sf::Color color, float x, float y) const;
};


template < class T1, class T2 >
bool Game::isIntersecting(T1 &objectA, T2 &objectB)
{
    return !(objectA.right() < objectB.left() || objectA.left() > objectB.right() ||
             objectA.top() > objectB.bottom() || objectA.bottom() < objectB.top());
}

template < class T1, class T2 >
void Game::resolveCollision(T1 &moveable, T2 &fixed)
{
    if(!isIntersecting(moveable, fixed)) return;
    //Вычисление с какой стороны мячик столкнулся с кирпичом
    float overlapLeft{moveable.right() - fixed.left()};
    float overlapRight{moveable.left() - fixed.right()};
    float overlapTop{moveable.bottom() - fixed.top()};
    float overlapBottom{moveable.top() - fixed.bottom()};

    double minOverlapX{std::min(fabs(overlapLeft), fabs(overlapRight))};
    double minOverlapY{std::min(fabs(overlapTop), fabs(overlapBottom))};

    if(minOverlapX < minOverlapY)
    {
        //Если мячик ударился об уголок ракетки меняем его направление случайным образом
        if(minOverlapY < fabs(moveable.top() - moveable.bottom()) / 2)
        {
           int randAngle = rand() % 50 + 40;
           moveable.changeDirectionAngle(randAngle);
        }
        moveable.reflectHorizontally();
        moveable.move();
    }
    else if(minOverlapX > minOverlapY)
    {
        if(minOverlapX < fabs(moveable.left() - moveable.right()) / 2)
        {
           int randAngle = rand() % 50 + 40;
           moveable.changeDirectionAngle(randAngle);
        }
        moveable.reflectVertically();
        moveable.move();
    }
}

template < class T>
bool Game::isOnTheField(T &entity)
{
    return entity.right() > Arkanoid::fieldLeft && entity.right() < Arkanoid::fieldRight + 10 &&
           entity.bottom() > Arkanoid::fieldTop && entity.top() < Arkanoid::fieldBottom + 10;

}

#endif // GAME_H
