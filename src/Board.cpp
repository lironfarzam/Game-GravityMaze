#include "Board.h"

#include <string>
#include <CollisionListener.h>

#include "DebugDraw.h"


const char* LEVELS_FILE = "levels.txt";

/*
* param: the window game passed by the controller
*/
Board::Board(sf::RenderWindow* w): m_platforms(0)
{
    m_window = w;
    // b2d collision detection
    Resources::instance().getImage(BG_IMAGE).setRepeated(true);
    m_background.setTexture(Resources::instance().getImage(BG_IMAGE));
    // rotate around center
    m_background.setOrigin(m_background.getTextureRect().width / 2, m_background.getTextureRect().height / 2);
    m_background.setPosition(0, 0);
    m_world = NULL; // will be created when loading the map
    srand(time(0)); // init random
}

void Board::loadMaps() 
{
    m_levels.loadMapsFromFile(LEVELS_FILE);
    setCurrentLevel(0);
    loadMapObjects();
}

/*
* loop over all objects  and draw them.
* at the end print game stats at the bottom of window
*/
void Board::drawGameWorld() const
{
    m_window->draw(m_background);

    for (auto& obj : m_gameObjects) {
        obj->draw(*m_window);
    }


    // uncomment to enable box2d debugging
    //auto w = m_world.get();
    //if (w) w->SetDebugDraw(m_debugDraw);
    //w->DebugDraw();

    printStats();
}

// dir is a keycode
void Board::rotateWorld(sf::Keyboard::Key key) {

    // rotate when player is on platform
    if (m_player->getCollisionWith() != NULL) {

        std::shared_ptr<Platform> plat_on_it;
        for (auto& plat : m_platforms) {
            if (plat.get() == m_player->getCollisionWith())
                plat_on_it = plat;
        }

        if (plat_on_it->getRotatable() ) {
            Resources::instance().playSound(ROTATE_SOUND, NO_LOOP, 100);

            setWorldRotating(true);
            // rotate door only if it is not on player's platform
            if (m_door->getPlatform().get() != plat_on_it.get()) {
                if (key == sf::Keyboard::A)
                    m_door->rotateLeft();
                else
                    if (key == sf::Keyboard::S)
                        m_door->rotateRight();
            }

            for (auto& plat : m_platforms) {
                // rotate all except the platform on which the player is
                if (plat.get() != m_player->getCollisionWith()) {
                    auto p = m_player->getPhysicalPosition();

                    if (key == sf::Keyboard::A)
                        plat->rotateLeft(); // set target angle
                    else
                        if (key == sf::Keyboard::S)
                            plat->rotateRight();
                }
            }

            for (auto& met : m_meteors) {
                if (key == sf::Keyboard::A)
                    met->rotateLeft(); // set target angle
                else
                    if (key == sf::Keyboard::S)
                        met->rotateRight();
            }
            for (auto& met : m_springboards) {
                // rotate all except the platform on which the spring is
                if (met->getPlatform().get() != plat_on_it.get()) {
                    if (key == sf::Keyboard::A)
                        met->rotateLeft();
                    else
                        if (key == sf::Keyboard::S)
                            met->rotateRight();
                }
            }
            for (auto laser : m_lasers)
            {
                if (key == sf::Keyboard::A)
                    laser->rotateLeft(); // set target angle
                else
                    if (key == sf::Keyboard::S)
                        laser->rotateRight();
            }
        }

    }
}

void Board::execGameCommand(sf::Keyboard::Key key)
{
    // moving
    if (key != sf::Keyboard::A && key != sf::Keyboard::S)
        m_player->execAction(key);
    else 
    if (!getWorldRotating())
        rotateWorld(key);
}

float Board::getWindowHeight() const
{
        return WIN_HEIGHT;
}

float Board::getWindowWidth() const
{
        return WIN_WIDTH;
}


void Board::resetGame() {

    m_gameIsOver = false;

    // delete world before releasing smart pointer
    if (m_world != NULL) {
        m_world->Step(0,0,0);
        delete m_world.release();
    }

    m_world = std::make_unique<b2World>(b2Vec2(0.f, 9.8f)); // box2d world gravity 9.8f

    /* FOR DEBUGGING */
    m_debugDraw = new DebugDraw(*m_window);
    //DebugDraw d(*m_window);
    uint32 flags = b2Draw::e_shapeBit;
    m_debugDraw->SetFlags(flags);
    /***************/

    m_world->SetContactListener(&m_collisionListener);
    m_platforms.clear();
    m_gameObjects.clear();
    m_springboards.clear();
    m_lasers.clear();
    m_player = NULL;
    m_door = NULL;
}


void Board::loadMapObjects()
{
    resetGame();

    GameMap curmap = m_levels.getCurrentMap();

    // use factory 
    auto obj = GameObjectFactory::create("Player", *m_world, m_levels.getCurrentMap().getPlayer()[0], m_levels.getCurrentMap().getPlayer()[1], 1, 1);
    m_player = std::dynamic_pointer_cast<Player>(obj);
    m_gameObjects.emplace_back(m_player);

    obj = GameObjectFactory::create("Door", *m_world, m_levels.getCurrentMap().getDoor()[0], m_levels.getCurrentMap().getDoor()[1], 1, 1);
    m_door = std::dynamic_pointer_cast<Door>(obj);
    m_door->setOrientation(m_levels.getCurrentMap().getDoor()[2]);
    m_gameObjects.emplace_back(m_door);

    for (auto& tile : curmap.getPlatforms()) {

        // b2World& world, float x, float y, sf::Texture& texture, int left, int right, can rotate, int orientation,  connect platform
        //                       0      1                               2         3        4             5             6          
        //tile[i]= int x, int y, int left, int right, int rotate, int orientation,int conectPlatform
        int h = 1;
        int w = 1;
        if (tile[5] == 0 || tile[5] == 2) // direction 0 == up   1 == right  2 = down  3 = left
            w = tile[2] + tile[3];
        else
            h = tile[2] + tile[3];

        m_platforms.emplace_back(std::dynamic_pointer_cast<Platform>(GameObjectFactory::create("Platform", *m_world, tile[0], tile[1], w, h)));
        m_gameObjects.emplace_back(m_platforms.back());

        m_platforms.back()->setLeftTiles(tile[2]);
        m_platforms.back()->setRightTiles(tile[3]);
        m_platforms.back()->setRotatable(tile[4]);
        m_platforms.back()->setOrientation(tile[5]);

        
        bool door_on_platform = false;

        if (tile[6] !=  0 ) {
            if (tile[5] == 0) {
                m_springboards.emplace_back(std::dynamic_pointer_cast<Springboard>(GameObjectFactory::create("Springboard",
                    *m_world,
                    tile[0] + tile[6],
                    tile[1] - 1,
                    1, 1)));
                m_springboards.back()->setOrientation(tile[5]);
                m_springboards.back()->setPlatform(m_platforms.back());
                m_gameObjects.push_back(m_springboards.back());
            }
            else {
            }

        }

        switch (m_door->getOrientation()) {
        case 0:
            // TOP
            door_on_platform = 
                m_levels.getCurrentMap().getDoor()[1] + 1 == tile[1]
                && m_levels.getCurrentMap().getDoor()[0] >= (tile[0] - tile[2]) 
                && m_levels.getCurrentMap().getDoor()[0] <= (tile[0] + tile[3]);
            break;
        case 1: // RIGHT
            door_on_platform =
                // check X
                m_levels.getCurrentMap().getDoor()[0] - 1 == tile[0]
                // check Y coord
                && m_levels.getCurrentMap().getDoor()[1] >= (tile[1] - tile[2]) 
                && m_levels.getCurrentMap().getDoor()[1] <= (tile[1] + tile[3]);
            break;
        case 2: // DOWN
            door_on_platform =
                m_levels.getCurrentMap().getDoor()[1] - 1 == tile[1]
                && m_levels.getCurrentMap().getDoor()[0] >= (tile[0] - tile[2]) 
                && m_levels.getCurrentMap().getDoor()[0] <= (tile[0] + tile[3]);
            break;
        case 3: // LEFT
            door_on_platform =
                m_levels.getCurrentMap().getDoor()[0] + 1 == tile[0]
                && m_levels.getCurrentMap().getDoor()[1] >= (tile[1] - tile[2]) 
                && m_levels.getCurrentMap().getDoor()[1] <= (tile[1] + tile[3]);
            break;
        }

        if (door_on_platform){
            // link platform to door
            m_door->setPlatform(m_platforms.back());
            m_xoffset = m_platforms.back()->getPosition().x;
            m_yoffset = m_platforms.back()->getPosition().y;
        }
    }

    for (auto& las : curmap.getlasers()) {

        // b2World& world, float x, float y, sf::Texture& texture, int left, int right, can rotate, int orientation,  connect platform
        //             0      1          2         3        4                    
        //las[i]= int x, int y, int orientation, int right, int speed
        int h = 1;
        int w = 1;

        m_lasers.emplace_back(std::dynamic_pointer_cast<Laser>(GameObjectFactory::create("Laser", *m_world, las[0], las[1], w, h)));
        m_gameObjects.emplace_back(m_lasers.back());

        m_lasers.back()->setOrientation(las[2]);
        m_lasers.back()->setDirctionToShoot(las[3]);
    }

    // create some meteors
    for (int i = 1; i < 5; i++) {

        m_meteors.emplace_back(std::dynamic_pointer_cast<Meteor>(GameObjectFactory::create("Meteor", *m_world, i * 2, 0, 1, 1)));
        int vx = randRange(-5, 5);
        int vy = randRange(1, 5);
        m_meteors.back()->getPhysicalBody()->SetLinearVelocity(b2Vec2(vx, vy));
        m_meteors.back()->getPhysicalBody()->SetTransform(b2Vec2(vy * 10, 0), atan2(vx, vy) + 3*M_PI/4);
        m_gameObjects.push_back(m_meteors.back());
    }
    
    resetGameClock();
}

void Board::activCodeLiron()
{
    liron_code = !liron_code;
}

void Board::rotGravityTo(int i)
{
    if (liron_code)
        m_gravity_int = ((i)+5) % 5;
    m_world->SetGravity(m_gravity_arr[m_gravity_int]);
}

sf::Keyboard::Key Board::getGravityIndex(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Num8 || key == sf::Keyboard::Numpad8)
        rotGravityTo(3);
    else if (key == sf::Keyboard::Num6 || key == sf::Keyboard::Numpad6)
        rotGravityTo(2);
    else if (key == sf::Keyboard::Num4 || key == sf::Keyboard::Numpad4)
        rotGravityTo(4);
    else if (key == sf::Keyboard::Num2 || key == sf::Keyboard::Numpad2)
        rotGravityTo(1);
    else if (key == sf::Keyboard::Num0 || key == sf::Keyboard::Numpad0)
        rotGravityTo(0);
    return key;
}


void Board::resetGameClock()
{
    m_gameClock.restart();
}

int Board::getGameClock()
{
    return (int)m_gameClock.getElapsedTime().asSeconds();
}

/*
* check if currrent game clock is over
*/
bool Board::timeIsOver() const {
    if (m_levels.getCurrentMap().getTimerValue() > 0) {
        if (m_gameClock.getElapsedTime().asSeconds() > m_levels.getCurrentMap().getTimerValue())
            return true;
    }
    return false;
}

int Board::getCurrentLevel() const
{
    return m_levels.getCurrentLevel();
}

void Board::setCurrentLevel(int lev)
{
    m_levels.setCurrentLevel(lev);
}

/*
* load the next map available
* returns false if no more maps, else true
*/
bool Board::loadNextMap() {

    if (m_levels.loadNextMap()) {
        loadMapObjects();
        return true;
    }
    else
        return false; // no more maps game is over
}


/*
* check if level is complete and load next map if any
*/
void Board::checkBoardState() {

    if (timeIsOver()) {
        m_player->setAlive(false);
    }

    if (levelCompleted()) {
        if (!loadNextMap())
            setGameOver(true);
    }
    if (playerIsDead())
        loadMapObjects();
}

// check if door is reached
bool Board::levelCompleted() const {
    return m_player->getLevelComplete();
}

void Board::restartGame()
{
    setCurrentLevel(0);
    loadMapObjects();
}

void Board::drawLevelComplete() const
{
    sf::Sprite s;

    if (playerIsDead())
        s.setTexture(Resources::instance().getImage(DEAD_IMAGE));
    else
    if (m_gameIsOver)
        s.setTexture(Resources::instance().getImage(GAMEOVER_IMAGE));
    else 
        s.setTexture(Resources::instance().getImage(SOLVED_IMAGE));

    s.setPosition((getWindowWidth() - s.getLocalBounds().width) / 2, (getWindowHeight() - s.getLocalBounds().height) / 2);
    m_window->draw(s);
}

void Board::drawGameOver() const
{
    sf::Sprite sp = sf::Sprite(Resources::instance().getImage(GAMEOVER_IMAGE));
    sp.setPosition(sf::Vector2f(0, 0));
    m_window->draw(sp);
}

void Board::drawWelcome() const
{
    sf::Sprite s;
    s.setTexture(Resources::instance().getImage(WELCOME_IMAGE));
    s.setScale(getWindowWidth() / s.getLocalBounds().width, getWindowHeight() / s.getLocalBounds().height);
    s.setPosition(0,0);
    m_window->draw(s);
}


bool Board::gameIsOver() const
{
    return m_gameIsOver;
}

void Board::setGameOver(bool b)
{
    m_gameIsOver = b;
}



/*
* prints the stats below the game board using sf::
*/
void Board::printStats() const {

    std::string score =
        + "Level: "    + std::to_string(getCurrentLevel()+1);

    auto timeleft = int (m_levels.getCurrentMap().getTimerValue() - m_gameClock.getElapsedTime().asSeconds());
    if (timeleft > 0)
            score += "   |   Clock: " + std::to_string(timeleft);


    sf::Text text;
    text.setFont (Resources::instance().getFont()); 
    text.setStyle(sf::Text::Bold);
    text.setString(score);
    text.setFillColor(sf::Color::Blue);
    text.setOutlineThickness(2);
    text.setOutlineColor(sf::Color::White);
    text.setCharacterSize(50);

    sf::Rect bounds = text.getGlobalBounds();
    bounds.top = getWindowHeight() - STATUS_HEIGHT ;
    bounds.left = (getWindowWidth() - bounds.width) / 2;
    text.setPosition(bounds.left, bounds.top);

    m_window->draw(text);
}

 //david
void Board::resetWindowSize() {
    m_window->setView(sf::View(sf::FloatRect(-100, -100, getWindowWidth(),getWindowHeight() )));
    //m_window->setView(sf::View(sf::FloatRect(-100, -100, 2000,2000 )));
    m_window->setSize(sf::Vector2u((unsigned int) getWindowWidth(), (unsigned int) getWindowHeight()));
}

void Board::update() {
    Platform* standing = NULL;

    m_player->update();
    m_door->update();
    for (auto laser : m_lasers)
    {
        laser->shootLaser();
        laser->update();
    }
    for (auto met : m_meteors)
    {
        met->update();
    }
    for (auto spr : m_springboards)
    {
        spr->update();
    }
    for (auto pla : m_platforms)
    {
        pla->update();
    }


    sf::Vector2f playerPlatformPos;
    b2Vec2 playerPlatformPhysicalPos;

    // update center of rotation for platforms: must not rotate the player's platform
    if (m_player->getCollisionWith() != NULL) // if player is on platform
    {
        playerPlatformPos = m_player->getCollisionWith()->getPosition();
        playerPlatformPhysicalPos = m_player->getCollisionWith()->getPhysicalPosition();

        m_door->setWorldCenterOfRotation(playerPlatformPhysicalPos); // set

        float angleForBackground = 0;

        for (auto& p : m_platforms) {

                p->setWorldCenterOfRotation(playerPlatformPhysicalPos);

                // get rotation angle from any platform (but not the player's platform)
                if (p->getCollisionWith() == NULL) {// is it player's platform
                    angleForBackground = p->getPhysicalBody()->GetAngle();
                }
        }
        for (auto& p : m_springboards) {
            p->setWorldCenterOfRotation(playerPlatformPhysicalPos);
        }

        for (auto& l : m_lasers) {
            l->setWorldCenterOfRotation(playerPlatformPhysicalPos);
        }
        for (auto& m : m_meteors) {
            m->setWorldCenterOfRotation(playerPlatformPhysicalPos);
        }
        m_background.setRotation(toDegree(angleForBackground));
        m_background.setPosition(playerPlatformPos);
    }

    //// center player on screem
    if (m_player->getPhysicalBody()->GetLinearVelocity().x != 0) {
        
        sf::Vector2f pos = m_player->getPosition();

        // compute difference between player and center of window
        auto new_orig = b2Vec2(m_player->getPhysicalPosition().x - WIN_WIDTH / (2 * BOX2D_SCALE),
            m_player->getPhysicalPosition().y - WIN_HEIGHT / (2 * BOX2D_SCALE));

        m_world->ShiftOrigin(new_orig);
        m_background.move(sf::Vector2f(-new_orig.x * BOX2D_SCALE, -new_orig.y * BOX2D_SCALE));
    }
}

// b2d world steps
void Board::step() {
    m_world->Step(1/60.f, m_velocityIterations, m_positionIterations);
}

void Board::stopPlayer() {
    m_player.get()->stop();
}

bool Board::playerIsDead() const {
    if (!m_player->getAlive()) {
        return true;
    }
    if (timeIsOver()) {
        return true;
    }
    bool isdead = true;
    for (auto& p : m_platforms) {
        if (m_player->getPosition().y < p->getPosition().y + 200){
            isdead = false;
            break;
        }
    }
    return isdead;
}

void Board::setWorldRotating(bool b)
{
    m_isWorldRotating = b;
}

bool Board::getWorldRotating()
{
    return m_isWorldRotating;
}
