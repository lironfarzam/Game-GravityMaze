
#include "Player.h"

#include <iostream>


// no template - code example from moodle
bool Player::m_registerit = GameObjectFactory::registerit("Player",
    [](b2World& world, float x, float y, float w, float h)-> std::shared_ptr<WorldObject> {return std::make_shared<Player>(world, x, y, w, h); });


 Player::Player(b2World& world, float x, float y, float w, float h)
     : MovableObject(world, x, y, w, h), m_rectInitSprite(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT)
{
    m_scale = 1.5f; // sprite scaling
    m_state = MovableObject::State::onGround;

    // after we stored them in m_x m_y we convert to screen coordinates
    x = x * TILE_WIDTH;
    y = y * TILE_HEIGHT;

    // the player sprite
    m_graphicBody.setTexture(Resources::instance().getImage(ASTRONAUT_IMAGE));

    //m_graphicBody.setTextureRect(sf::IntRect(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT));
        // offset to sprite img
    m_rectInitSprite.top = PLAYER_HEIGHT;
    m_rectInitSprite.left = 0;
    m_graphicBody.setTextureRect(m_rectInitSprite);

    m_graphicBody.setOrigin(m_graphicBody.getTextureRect().width / 2, m_graphicBody.getTextureRect().height / 2);
    m_graphicBody.setScale(m_scale, m_scale);

    //create b2BodyDef - the sprite with Box2D coord m_scaled
    b2BodyDef body;
    body.position = b2Vec2(x / BOX2D_SCALE, y / BOX2D_SCALE); // body is the sprite
    body.type = b2_dynamicBody;

    // add b2BodyDef to world ->> create physical body
    m_physicalBody = world.CreateBody(&body);
    m_physicalBody->SetUserData(this); // for collision

    //m_centerOfRotation = m_physicalBody->GetPosition(); // by default
    // player rotates around its base
    m_centerOfRotation = b2Vec2(m_physicalBody->GetPosition().x,
        m_graphicBody.getTextureRect().height * m_scale / (2 * BOX2D_SCALE));

    // Shape is a b2PolygonShape - params hal width/height / REDUCE WIDTH BY HALF FOR BETTER MOVEMENTS
    m_shape.SetAsBox(m_graphicBody.getTextureRect().width  * m_scale / (2 * 2 * BOX2D_SCALE), 
        m_graphicBody.getTextureRect().height * m_scale / (2  * BOX2D_SCALE));

    // set Shape to b2FixtureDef
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.3f; // 0.05f;
    FixtureDef.shape = &m_shape;

    // create fixture for physical body
    m_physicalBody->CreateFixture(&FixtureDef);

    // set rotation for our sprite
    m_graphicBody.setRotation(m_physicalBody->GetAngle() * 180/b2_pi);

    /// PLAYER SPECIFIC

    b2MassData mass{5, m_physicalBody->GetLocalCenter(), m_physicalBody->GetInertia()};
    m_physicalBody->SetMassData(&mass);
    m_physicalBody->SetFixedRotation(true); //  do no fall on side
    m_graphicBody.setPosition(x, y);

    Resources::instance().getImage(PLATFORM_IMAGE).setRepeated(true);

}

void Player::moveLeft()
{
    std::cout << "VELO left = " << m_physicalBody->GetLinearVelocity().y << std::endl;
    if ( getCollisionWith() != NULL ) {
        m_physicalBody->SetLinearVelocity(b2Vec2(-m_force, 0));
    }
    if (m_physicalBody->GetLinearVelocity().y > -m_maxSpeed/2) {
        m_physicalBody->SetLinearVelocity(b2Vec2(-6, m_physicalBody->GetLinearVelocity().y));
    }
}

void  Player::handleEndCollision() {

}

void Player::moveRight()
{
    std::cout << "VELO RIGHT = " << m_physicalBody->GetLinearVelocity().y << std::endl;
    if (getCollisionWith() != NULL ) {
        m_physicalBody->SetLinearVelocity(b2Vec2(m_force, 0));
    }
    if (m_physicalBody->GetLinearVelocity().y < m_maxSpeed/2) {
        m_physicalBody->SetLinearVelocity(b2Vec2(6, m_physicalBody->GetLinearVelocity().y));
    }
}

void Player::jump()
{
    float impulse = -m_physicalBody->GetMass() * 9.5;
    m_physicalBody->ApplyLinearImpulse(b2Vec2(0,impulse), m_physicalBody->GetWorldCenter() , true);

    m_onGround = false;
}

void Player::stop() {
    if (getCollisionWith() != NULL) {
        m_dir = Action::Stay;
        m_physicalBody->SetLinearVelocity(b2Vec2(0, 0));
    }
}

void Player::rotateLeft()
{
}

void Player::rotateRight()
{
}

/*
 * box2d listener handler for collision detection
 */
void Player::handleCollision() {
    m_onGround = true;
}

// dir is a keycode
void Player::execAction(sf::Keyboard::Key act) {

    kbdToAction(act); // update m_dir

    if (m_dir == Action::Left)
        moveLeft();

    if (m_dir == Action::Right)
        moveRight();

    if (m_dir == Action::Up)
    {
        if (getCollisionWith() != NULL /*getNumContacts() >= 1 */ )
        {
            jump();
        }
    }   
}

void Player::update()
{
    // eliminate the effects of the angular velocity from the previous time step
    m_physicalBody->SetAngularVelocity(0); 
    MovableObject::update();
}

int Player::getLives() const { return m_lives; }
int Player::getScore() const { return m_score;}
void Player::setLives(int l) { m_lives = l;}
void Player::setScore(int s) { m_score = s; }

bool Player::getLevelComplete() const
{
    return m_levelComplete;
}

void Player::setLevelComplete(bool b)
{
    m_levelComplete = b;
}

void Player::setNumContacts(int mNumContacts) {
    WorldObject::setNumContacts(mNumContacts);
    if (m_numContacts == 0) {
        m_state = State::inSpace;
    } else {
        m_state = State::onGround;
    }
}

bool Player::getAlive() const
{
    return m_isAlive;
}

void Player::setAlive(bool b)
{
    m_isAlive = b;
}

void Player::draw(sf::RenderWindow& window)
{
    sf::IntRect rectSprite = m_graphicBody.getTextureRect();

    if (m_dir == Action::Stay) {
        m_rectInitSprite.top = 0;
    }

    if (m_dir == Action::Left) {
        m_rectInitSprite.top = PLAYER_HEIGHT;
    }
    else
        if (m_dir == Action::Right) {
            m_rectInitSprite.top = 2 * PLAYER_HEIGHT;
        }

    // check if we changed state/position, reset the starting image
    if (m_rectInitSprite.left > rectSprite.left || (m_rectInitSprite.left + m_rectInitSprite.width) < rectSprite.left
        || rectSprite.top < m_rectInitSprite.top || rectSprite.top >(m_rectInitSprite.top + ShapeHeight))
    {
        rectSprite.left = m_rectInitSprite.left;
        rectSprite.top = m_rectInitSprite.top;
    }

    // rotate on images in PNG file
    if (m_clock.getElapsedTime().asSeconds() > ANIMATION_SPEED) {

        if (rectSprite.left >= m_rectInitSprite.left + (PLAYER_WIDTH * 4))
            rectSprite.left = (int)m_rectInitSprite.left;
        else
            rectSprite.left += PLAYER_WIDTH;

        m_graphicBody.setTextureRect(rectSprite);
        m_clock.restart();
    }
    MovableObject::draw(window);
}




