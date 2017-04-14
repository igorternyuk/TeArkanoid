#include "game.h"

Game::Game() :
    paddle_{Arkanoid::fieldWidth / 2, Arkanoid::paddleTop, Arkanoid::paddleVelocity,
            Arkanoid::paddleWidth, Arkanoid::paddleHeight, sf::Color(114, 14, 195)}
{
    //Создаем первый мячик
    Ball ball{Arkanoid::fieldWidth / 2, Arkanoid::fieldHeight / 2, Arkanoid::ballVelocityX,
              Arkanoid::ballVelocityY, Arkanoid::ballRadius, sf::Color(127, 0, 0)};
    balls_.push_back(ball);
    //Создаем вектор кирпичей
    createBricks();
    createEnemies();
    if(!font_.loadFromFile("DroidSansMono.ttf"))
    {
        std::cout << "Could not load font!" << std::endl;
    }
    if(!capsulesTexture_.loadFromFile("capsules.png"))
    {
        std::cout << "Could not load capsules.png" << std::endl;
    }
    createButtons();
    loadSounds();
}

void Game::go()
{
    sf::RenderWindow window{{Arkanoid::windowWidth, Arkanoid::windowHeight}, "TeArkanoid", sf::Style::Close};
    window.setPosition({Arkanoid::windowLeft, Arkanoid::windowTop});
    window.setFramerateLimit(60);

    //Game loop
    backgroundMusic_.play();
    backgroundMusic_.setVolume(50);
    while(window.isOpen())
    {
        static int goalTicker = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::N))
            prepareNewGame(1);
        sf::Event events;
        while(window.pollEvent(events))
        {
            if(events.type == sf::Event::Closed)
                window.close();
            if(events.type == sf::Event::KeyReleased)
            {
                if(events.key.code == sf::Keyboard::Space)
                    pauseGame();
                else if(events.key.code == sf::Keyboard::F)
                    paddle_.ceaseFire();
            }
            if(events.type == sf::Event::MouseButtonReleased)
            {
                if(events.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    int bx1 = newGameButton_.getPosition().x, by1 = newGameButton_.getPosition().y;
                    int br = newGameButton_.getRadius();
                    if(pow(mousePosition.x - bx1, 2) + pow(mousePosition.y - by1, 2) <= br * br)
                    {
                        prepareNewGame(1);
                    }

                    int bx2 = pauseButton_.getPosition().x, by2 = pauseButton_.getPosition().y;
                    int br2 = pauseButton_.getRadius();
                    if(pow(mousePosition.x - bx2, 2) + pow(mousePosition.y - by2, 2) <= br2 * br2)
                       pauseGame();
                    int bx3 = exitButton_.getPosition().x, by3 = exitButton_.getPosition().y;
                    int br3 = exitButton_.getRadius();
                    if(pow(mousePosition.x - bx3, 2) + pow(mousePosition.y - by3, 2) <= br3 * br3)
                    {
                        window.close();
                    }
                }
             }
        }
        if(gameState_ == GameState::PLAY)
        {
            if(paddle_.isGunActive() && sf::Keyboard::isKeyPressed(sf::Keyboard::F))
            {
                paddle_.fire();
            }
            //Регуляция громкости фоновой музыки
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
            {
                if(volume_ < 100)
                    ++volume_;
               // std::cout << "Volume = " << volume_ << std::endl;
                backgroundMusic_.setVolume(volume_);
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
            {
                if(volume_ > 0)
                    --volume_;
               // std::cout << "Volume = " << volume_ << std::endl;
                backgroundMusic_.setVolume(volume_);
            }
            //Двигаем ракетку
            paddle_.move();
            //Обрабатываем коллизии кирпичей с лазерным лучом
            for(auto &brick : bricks_)
            {
                if(paddle_.isGunRayActive() && sound_laser_.getStatus() == sf::Sound::Status::Stopped)
                    sound_laser_.play();
                resolveCollision(brick, paddle_);
            }
            //Обрабатываем коллизии мячиков с ракеткой
            for(auto &ball : balls_)
            {
                if(!isIntersecting(ball, paddle_)) continue; // Линию по ходу можно убрать
                resolveCollision(ball, paddle_);
                if(sound_paddleBall_.getStatus() == sf::Sound::Status::Stopped)
                    sound_paddleBall_.play();
            }
            for(auto &ball : balls_)
            {
                //Обрабатываем коллизии мячиков с кирпичами
                for(auto &brick : bricks_)
                {
                    if(!isIntersecting(ball, brick))
                        continue;
                    resolveCollision(ball, brick);
                    brick.hit();
                    if(sound_ballBrick_.getStatus() == sf::Sound::Status::Stopped)
                        sound_ballBrick_.play();
                    if(!brick.isAlive())
                    {
                        score_ += brick.getScoreIncrement();
                        brick.activateBonus(bonuses_, capsulesTexture_);
                        if(sound_collectPoints_.getStatus() == sf::Sound::Status::Stopped)
                            sound_collectPoints_.play();
                    }
                }
                //Обрабатываем коллизии мячиков с врагами
                for(auto &enemy : enemies_)
                {
                    //Если враг находится на поле и мячик летит вверх то обабатываем их коллизию
                    if(isOnTheField(enemy) && ball.vy() < 0)
                    {
                        if(!isIntersecting(ball, enemy)) continue;
                        resolveCollision(ball, enemy);
                        if(sound_BallEnemy_.getStatus() == sf::Sound::Status::Stopped)
                            sound_BallEnemy_.play();
                        //break;
                    }
                }
                //Двигаем мячик
                ball.move();
                if(ball.top() > Arkanoid::redLineLimit)
                {
                    --numLives_;
                    gameState_ = GameState::GOAL;
                    if(sound_Goal_.getStatus() == sf::Sound::Status::Stopped)
                        sound_Goal_.play();
                    ball.reset(level_);
                }
            }
            //Двигаем врагов - Тут нада проработать чтобы враги появлялись как красное НЛО в
            for(auto &enemy : enemies_)
            {
                enemy.move();
            }
            //Двигаем бонусы
            for(auto &bonus : bonuses_)
            {
                bonus.drop();
                if(bonus.top() > Arkanoid::redLineLimit)
                    bonus.desactivate();
                if(!isIntersecting(bonus, paddle_)) continue;
                resolveCollision(bonus, paddle_);
                if(sound_collectBonus_.getStatus() == sf::Sound::Status::Stopped)
                    sound_collectBonus_.play();
            }
            //Подчищаем вектор кирпичей
            bricks_.erase(std::remove_if(bricks_.begin(), bricks_.end(), [](Brick brick){return !brick.isAlive();}), bricks_.end());
            if(bricks_.size() == 0)
            {
                //Если кирпичей не осталось то переходим на следующий уровень
                if(++level_ > Arkanoid::maxLevel && numLives_ > 0)
                    gameState_ = GameState::VICTORY;
                else
                {
                    if(!sound_nextLevel_.getStatus() == sf::Sound::Status::Stopped)
                        sound_nextLevel_.play();
                    prepareNewGame(level_);
                }
            }
            //Подчищаем вектор бонусов
            bonuses_.erase(std::remove_if(bonuses_.begin(), bonuses_.end(), [](Bonus bonus){return bonus.isTaken() || !bonus.isActive();}), bonuses_.end());
            if(numLives_ <= 0)
                gameState_ = GameState::GAME_OVER;

        }
        window.clear(sf::Color::Black);
        drawField(window);
        drawGameInfo(window);
        drawCharacters(window);
        drawButtons(window);
        //Проверяем состояние игры
        switch(gameState_)
        {
            case GameState::GOAL :
                if(++goalTicker < Arkanoid::goalDelay)
                    drawMessage(window, Arkanoid::GOAL_MESSAGE, font_, 100, sf::Color::Blue, 130, 200);
                else
                {
                    goalTicker = 0;
                    gameState_ = GameState::PLAY;
                }
                break;
            case GameState::VICTORY :
                drawMessage(window, Arkanoid::WIN_MESSAGE, font_, 50, sf::Color::Red, 40, 200);
                if(sound_Victory_.getStatus() == sf::Sound::Status::Stopped)
                    sound_Victory_.play();
                break;
            case GameState::GAME_OVER :
                drawMessage(window, Arkanoid::GAME_OVER_MESSAGE, font_, 80, sf::Color::Yellow, 110, 200);
                backgroundMusic_.stop();
                if(sound_Defeat_.getStatus() == sf::Sound::Status::Stopped)
                    sound_Defeat_.play();
                break;
            case GameState::PAUSE :
                drawMessage(window, Arkanoid::PAUSE_MESSAGE, font_, 80, sf::Color(25, 98, 37), 40, 200);
                break;
            default :
            break;
        }
        window.display();
    }
}

void Game::resolveCollision(Ball &ball, Paddle &paddle)
{
    if(!isIntersecting(ball, paddle)) return;
    float overlapLeft{ball.right() - paddle.left()};
    float overlapRight{ball.left() - paddle.right()};
    double minOverlapX{std::min(fabs(overlapLeft), fabs(overlapRight))};

    if(minOverlapX < fabs(ball.radius()))
    {
       //std::cout << "Меняем угол движения мячика случайным образом" << std::endl;
       int randAngle = rand() % 50 + 40;
       //std::cout << "Угол = " << randAngle << std::endl;
       ball.changeDirectionAngle(randAngle);
    }

    switch(paddle.getDirection())
    {
        case Arkanoid::Direction::LEFT :
            ball.reflectTopLeft();
            break;
        case Arkanoid::Direction::RIGHT :
            ball.reflectTopRight();
            break;
        case Arkanoid::Direction::STOP :
            ball.reflectVertically();
            break;
        default :
            break;
    }
}

void Game::resolveCollision(Bonus &bonus, Paddle &paddle)
{
    if(!isIntersecting(bonus, paddle)) return;
    switch(bonus.getType())
    {
        case Arkanoid::Bonuses::PLAYER :
            //std::cout << "Поймали бонус, дающий дополнительную жизнь" << std::endl;
            ++numLives_;
            break;
        case Arkanoid::Bonuses::EXPAND :
            //std::cout << "Поймали бонус расширяющий ракетку" << std::endl;
            paddle.expand();
            break;
        case Arkanoid::Bonuses::SLOW :
            //std::cout << "Поймали бонус замедляющий шарики" << std::endl;
            for(auto &ball :balls_)
                ball.decelerate();
            break;
        case Arkanoid::Bonuses::CATCH :
            //std::cout << "Поймали бонус отнимающий жизнь" << std::endl;
            --numLives_;
            break;
        case Arkanoid::Bonuses::DIVIDE :
            //std::cout << "Поймали бонус, из шарика вылетает еще 1 " << std::endl;
            balls_.emplace_back(balls_[0].x(), balls_[0].y(), -Arkanoid::ballVelocityX,
                   -Arkanoid::ballVelocityY, Arkanoid::ballRadius, sf::Color(127, 0, 0));
            break;
        case Arkanoid::Bonuses::LASER :
            //std::cout << "Поймали бонус, дающий лазерную пушку игроку" << std::endl;
            paddle_.activateLaserGun();
            break;
        default:
        break;
    }
    bonus.desactivate();
}

void Game::resolveCollision(Brick &brick, Paddle &paddle)
{
    if(!paddle.isGunRayActive()) return;
    if(paddle.x() >= brick.left() && paddle.x() <= brick.right())
        brick.hit();
}

void Game::prepareNewGame(int level)
{
    gameState_ = GameState::PLAY;
    backgroundMusic_.play();
    paddle_.narrow(); // Сужаем ракетку обратно
    paddle_.desactivateLaserGun();
    bonuses_.clear();
    bricks_.clear();
    createBricks();
    if(level == 1)
    {
        level_ = 1;
        score_ = 0;
        numLives_ = Arkanoid::playerLivesNumber;
    }
    auto it = balls_.begin();
    ++it;
    balls_.erase(it, balls_.end());
    balls_[0].reset(level);
    //std::cout << "Текущий уровень - " << level_ << std::endl;
    //std::cout << "Скорость мячика " << sqrt(pow(balls_[0].vx(), 2) + pow(balls_[0].vy(), 2)) << std::endl;
}

void Game::pauseGame()
{
    if(gameState_ != GameState::GAME_OVER && gameState_ != GameState::VICTORY)
    {
        if(gameState_ == GameState::PLAY)
        {
           gameState_ = GameState::PAUSE;
           backgroundMusic_.pause();
        }
        else
        {
           gameState_ = GameState::PLAY;
           backgroundMusic_.play();
        }
    }
}

void Game::createBricks()
{
    bricks_.clear();
    srand(time(0));
    for(int x{0}; x < Arkanoid::numBricksX; ++x)
    {
        for(int y{0}; y < Arkanoid::numBricksY; ++y)
        {
            Arkanoid::Bricks currType;
            sf::Color currColor;
            int randNum = rand() % 1000;
            if(randNum < 500)
            {
                currType = Arkanoid::Bricks::YELLOW;
                currColor = sf::Color::Yellow;
            }
            else if(randNum < 700)
            {
                currType = Arkanoid::Bricks::BLUE;
                currColor = sf::Color::Blue;
            }
            else if(randNum < 800)
            {
                currType = Arkanoid::Bricks::ORANGE;
                currColor = sf::Color(255, 26, 0);
            }
            else if(randNum < 950)
            {
                currType = Arkanoid::Bricks::GREEN;
                currColor = sf::Color(32, 164, 21);
            }
            else
            {
                currType = Arkanoid::Bricks::GRAY;
                currColor = sf::Color(188, 190, 162);
            }
            Brick brick(70 + x * (Arkanoid::brickWidth + 3),
                        Arkanoid::bricksFirstRowTop + y * (Arkanoid::brickHeight + 2),
                        Arkanoid::brickWidth, Arkanoid::brickHeight, currColor, currType);
            bricks_.push_back(brick);
        }
    }
}

void Game::createButtons()
{
    newGameButtonTexture_.loadFromFile("newGameButtonR.png");
    newGameButton_.setPosition({700, 330});
    newGameButton_.setRadius(40);
    newGameButton_.setOrigin(40, 40);
    newGameButton_.setFillColor(sf::Color::Green);
    newGameButton_.setTexture(&newGameButtonTexture_);

    pauseButtonTexture_.loadFromFile("pauseButtonR.png");
    pauseButton_.setPosition({700, 430});
    pauseButton_.setRadius(40);
    pauseButton_.setOrigin(40, 40);
    pauseButton_.setFillColor(sf::Color(0, 148, 255));
    pauseButton_.setTexture(&pauseButtonTexture_);

    exitButtonTexture_.loadFromFile("exitButtonR.png");
    exitButton_.setPosition({700, 530});
    exitButton_.setRadius(40);
    exitButton_.setOrigin(40, 40);
    exitButton_.setFillColor(sf::Color::Red);
    exitButton_.setTexture(&exitButtonTexture_);
}

void Game::createEnemies()
{
    Enemy redEnemy(-100, 250, Arkanoid::enemyVelocity, Arkanoid::enemyWidth, Arkanoid::enemyHeight, sf::Color::Red);
    Enemy greenEnemy(-500, 380, Arkanoid::enemyVelocity, Arkanoid::enemyWidth, Arkanoid::enemyHeight, sf::Color::Green);
    enemies_.push_back(redEnemy);
    enemies_.push_back(greenEnemy);
}

void Game::loadSounds()
{
    if(!ballBrick_.loadFromFile("Sounds/ball_brick.wav"))
        std::cout << "Could not load Sounds/ball_brick.wav" << std::endl;
    else
    {
        sound_ballBrick_.setBuffer(ballBrick_);
        sound_ballBrick_.setLoop(false);
    }
    if(!paddleBall_.loadFromFile("Sounds/paddle_ball.wav"))
        std::cout << "Could not load Sounds/paddle_ball.wav" << std::endl;
    else
    {
        sound_paddleBall_.setBuffer(paddleBall_);
        sound_paddleBall_.setLoop(false);
    }
    if(!ballEnemy_.loadFromFile("Sounds/ball_enemy.wav"))
        std::cout << "Could not load Sounds/ball_enemy.wav" << std::endl;
    else
    {
        sound_BallEnemy_.setBuffer(ballEnemy_);
        sound_BallEnemy_.setLoop(false);
    }
    if(!collectBonus_.loadFromFile("Sounds/collect_bonus.wav"))
        std::cout << "Could not load Sounds/collect_bonus.wav" << std::endl;
    else
    {
        sound_collectBonus_.setBuffer(collectBonus_);
        sound_collectBonus_.setLoop(false);
    }
    if(!collectPoints_.loadFromFile("Sounds/collect_points.wav"))
        std::cout << "Could not load Sounds/collect_points.wav" << std::endl;
    {
        sound_collectPoints_.setBuffer(collectPoints_);
        sound_collectPoints_.setLoop(false);
    }
    //sf::SoundBuffer laser_;
    //sf::Sound sound_laser_;
    if(!laser_.loadFromFile("Sounds/laser.wav"))
        std::cout << "Could not load Sounds/laser.wav" << std::endl;
    else
    {
        sound_laser_.setBuffer(laser_);
        sound_laser_.setLoop(false);
    }
    if(!goal_.loadFromFile("Sounds/goal.wav"))
        std::cout << "Could not load Sounds/goal.wav" << std::endl;
    else
    {
        sound_Goal_.setBuffer(goal_);
        sound_Goal_.setLoop(false);
    }
    if(!nextLevel_.loadFromFile("Sounds/next_level.wav"))
        std::cout << "Could not load Sounds/next_level.wav" << std::endl;
    else
    {
        sound_nextLevel_.setBuffer(nextLevel_);
        sound_nextLevel_.setLoop(false);
    }
    if(!defeat_.loadFromFile("Sounds/defeat.wav"))
        std::cout << "Could not load Sounds/defeat.wav" << std::endl;
    else
    {
        sound_Defeat_.setBuffer(defeat_);
        sound_Defeat_.setLoop(false);
    }
    if(!victory_.loadFromFile("Sounds/victory.wav"))
        std::cout << "Could not load Sounds/victory.wav" << std::endl;
    else
    {
        sound_Victory_.setBuffer(victory_);
        sound_Victory_.setLoop(false);
    }
    backgroundMusic_.openFromFile("Sounds/background.ogg");
    backgroundMusic_.setLoop(true);
}

void Game::drawField(sf::RenderWindow &window) const
{
    sf::RectangleShape rect;
    rect.setPosition(Arkanoid::fieldLeft, Arkanoid::fieldTop);
    rect.setSize({Arkanoid::fieldWidth, Arkanoid::fieldHeight});
    rect.setFillColor(sf::Color(15, 195, 90));
    rect.setOutlineThickness(10);
    rect.setOutlineColor(sf::Color(127, 51, 0));
    sf::RectangleShape redLine;
    redLine.setSize({Arkanoid::fieldWidth, 5});
    redLine.setPosition({Arkanoid::fieldLeft, 560});
    redLine.setFillColor(sf::Color::Red);
    window.draw(rect);
    window.draw(redLine);
}

void Game::drawGameInfo(sf::RenderWindow &window) const
{
    //Level
    sf::Text levelText(sf::String("LEVEL"), font_, 40);
    levelText.setColor(sf::Color(37, 187, 40));
    levelText.setPosition(Arkanoid::fieldRight + 30, Arkanoid::fieldTop + 5);
    levelText.setStyle(sf::Text::Bold | sf::Text::Italic);
    std::stringstream ss;
    ss << level_;
    sf::Text level(sf::String(ss.str()), font_, 50);
    level.setColor(sf::Color::Red);
    level.setPosition(Arkanoid::fieldRight + 70, Arkanoid::fieldTop + 48);
    level.setStyle(sf::Text::Bold);
    //Score
    sf::Text scoreText(sf::String("SCORE"), font_, 40);
    scoreText.setColor(sf::Color(37, 73, 187));
    scoreText.setPosition(Arkanoid::fieldRight + 30, Arkanoid::fieldTop + 105);
    scoreText.setStyle(sf::Text::Bold | sf::Text::Italic);
    std::stringstream ss2;
    ss2 << score_;
    sf::Text score(sf::String(ss2.str()), font_, 30);
    score.setColor(sf::Color::Green);
    score.setPosition(Arkanoid::fieldRight + 30, Arkanoid::fieldTop + 145);
    score.setStyle(sf::Text::Bold);
    //Lives
    sf::Text livesText(sf::String("LIVES"), font_, 40);
    livesText.setColor(sf::Color::Yellow);
    livesText.setPosition(Arkanoid::fieldRight + 30, Arkanoid::fieldTop + 185);
    livesText.setStyle(sf::Text::Bold | sf::Text::Italic);
    std::stringstream ss3;
    ss3 << numLives_;
    sf::Text numLives(sf::String(ss3.str()), font_, 40);
    numLives.setColor(sf::Color::Red);
    numLives.setPosition(Arkanoid::fieldRight + 80, Arkanoid::fieldTop + 225);
    numLives.setStyle(sf::Text::Bold);
    window.draw(levelText);
    window.draw(level);
    window.draw(scoreText);
    window.draw(score);
    window.draw(livesText);
    window.draw(numLives);
}

void Game::drawCharacters(sf::RenderWindow &window) const
{

    //Прорисовываем мячики
    for(auto &ball: balls_)
         ball.draw(window);
    //Прорисовываем кирпичи
    for(auto &brick : bricks_)
        if(brick.isAlive())
            brick.draw(window);
    for(auto &bonus: bonuses_)
        if(bonus.isActive())
            bonus.draw(window);
    //Прорисовываем врагов
    for(auto &enemy : enemies_)
        enemy.draw(window);
    //С врагами потом разберемся
    //Прорисовываем ракетку
    paddle_.draw(window);
}

void Game::drawButtons(sf::RenderWindow &window) const
{
    window.draw(newGameButton_);
    window.draw(pauseButton_);
    window.draw(exitButton_);
}

void Game::drawMessage(sf::RenderWindow &window, const std::string &message,
                       const sf::Font &font, int fontSize, sf::Color color,
                       float x, float y) const
{
    sf::Text text(sf::String(message), font, fontSize);
    text.setPosition({x, y});
    text.setColor(color);
    text.setStyle(sf::Text::Bold);
    window.draw(text);
}
