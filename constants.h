#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
namespace Arkanoid
{
const std::string GOAL_MESSAGE{"GOAL!!!"};
const std::string WIN_MESSAGE{"CONGRATULATIONS!!!\n    YOU WON!!!"};
const std::string PAUSE_MESSAGE{"GAME PAUSED"};
const std::string GAME_OVER_MESSAGE{"GAME OVER"};
constexpr int windowWidth{800}, windowHeight{600};
constexpr int windowLeft{283}, windowTop{50};
constexpr int fieldWidth{600}, fieldHeight{580};
constexpr int fieldLeft{10}, fieldTop{10}, fieldRight{610}, fieldBottom{590};
constexpr int windowStartPosLeft{150}, windowStartPosY{150};
constexpr float gravity{1.f};
constexpr int playerLivesNumber{12};
constexpr float ballRadius{10}, ballVelocityX{-1.f}, ballVelocityY{-4.f}, mimBallVelocity{1.f};
constexpr float paddleWidth{100}, paddleHeight{20}, paddleTop{windowHeight - 50}, paddleVelocity{6.f}, paddleWidthIncrement{20};
constexpr float brickWidth{45}, brickHeight{20}, bricksFirstRowTop{70};
constexpr int numBricksX{11}, numBricksY{6};
constexpr float bonusVelocity{1.f}, bonusRadius{12.f}, bonusWidth{43}, bonusHeight{20};
constexpr float enemyVelocity{3.f}, enemyLeftLimit{-700}, enemyRightLimit{700};
constexpr float enemyWidth{120}, enemyHeight{20};
constexpr int redLineLimit{550};
constexpr float laserGunWidth{10}, laserGunHeight{50};
constexpr int goalDelay{50};
constexpr int maxLevel{12};

enum class Direction {STOP, LEFT, RIGHT, UP, DOWN};
enum class Bricks {YELLOW = 100, BLUE = 50, ORANGE = 34, GREEN = 25, GRAY = 20};
enum class Bonuses {EMPTY = -1, EXPAND = 0, DIVIDE = 1, LASER = 2, SLOW = 3, CATCH = 4, PLAYER = 5};
}

/*
BLUE (E — от англ. expand — «расширять») — увеличивает платформу в 2 раза.
CYAN (D — от англ. divide — «делить») — из шарика вылетают ещё 2 шарика.
RED (L — от англ. laser — «лазер») — устанавливает на платформу лазерную установку, которая может разрушать кирпичи лазером.
PINK (S — от англ. slow — «медленный») — замедляет шар.
GREEN (C — от англ. catch — «ловить») — шарик теперь прилипает к платформе.
GRAY (P — от англ. player — «игрок») — дополнительная жизнь.
*/
#endif // CONSTANTS_H
