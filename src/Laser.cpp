#include "Laser.h"

// no template - code example from moodle
bool Laser::m_registerit = GameObjectFactory::registerit("Laser",
    [](b2World& world, float x, float y, float w, float h)-> std::shared_ptr<WorldObject> {return std::make_shared<Laser>(world, x, y, w, h); });


Laser::Laser(b2World& world, float x, float y, float w, float h)
    : MovableObject(world, x, y, w, h) {

    m_scale = 1.f;

    //*********************
    // after we stored them in m_x m_y we convert to screen coordinates
    x = x * TILE_WIDTH;
    y = y * TILE_HEIGHT;

    m_graphicBody.setTexture(Resources::instance().getImage(LASER_GUN_IMAGE));
    m_graphicBody.setTextureRect(sf::IntRect(0, 0, TILE_WIDTH * (w), TILE_HEIGHT * (h)));
    m_graphicBody.setOrigin(m_graphicBody.getTextureRect().width / 2, m_graphicBody.getTextureRect().height / 2);
    if (m_orientation % 2 == 1)
        m_graphicBody.rotate(90);

    m_graphicBody.setScale(m_scale, m_scale);

    //create b2BodyDef - the sprite with Box2D coord m_scaled
    b2BodyDef body;
    body.position = b2Vec2(x / BOX2D_SCALE, y / BOX2D_SCALE); // body is the sprite
    body.type = b2_kinematicBody; // ; b2_dynamicBody; // b2_kinematicBody;

    // add b2BodyDef to world ->> create physical body
    m_physicalBody = world.CreateBody(&body);
    m_physicalBody->SetUserData(this); // for collision
    m_centerOfRotation = m_physicalBody->GetPosition(); // by default

    // Shape is a b2PolygonShape - params hal width/height
    m_shape.SetAsBox(m_graphicBody.getTextureRect().width * m_scale / (2 * BOX2D_SCALE),
        m_graphicBody.getTextureRect().height * m_scale / (2 * BOX2D_SCALE));

    // set Shape to b2FixtureDef
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.05f; // 0.05f;
    FixtureDef.shape = &m_shape;

    // create fixture for physical body
    m_physicalBody->CreateFixture(&FixtureDef);

    // set rotation for our sprite
    m_graphicBody.setPosition(x, y);
    m_graphicBody.setRotation(m_physicalBody->GetAngle() * 180 / b2_pi);
    m_world = &world;
}

void Laser::setPlatform(std::shared_ptr<Platform> p)
{
    m_platform = p;
}

std::shared_ptr<Platform> Laser::getPlatform()
{
    return m_platform;
}

int Laser::getOrientation() const
{
    return m_orientation;
}

void Laser::setOrientation(int or )
{
    m_orientation = or ;
}

void Laser::setSwitch(bool b)
{
    m_switch = b;
}

bool Laser::getSwitch()
{
    return m_switch;
}

void Laser::setDirctionToShoot(bool b)
{
    m_shootToTheRight = b;
}

void Laser::shootLaser()
{
    
    if (m_shoots.size() < 18 && m_clock.getElapsedTime() > m_timeToShoot)
    {
        m_clock.restart();
        auto posisen = m_physicalBody->GetPosition();
        auto obj = GameObjectFactory::create("LaserBeam", *m_world, posisen.x, posisen.y, 1, 1);
        auto NewShoot = std::dynamic_pointer_cast<LaserBeam>(obj);
        b2Vec2 velosity =3 * setNewShootTheVelosity();//NEAD CONST!!!SPEED OF LASER
        NewShoot->getPhysicalBody()->SetLinearVelocity( velosity);
        NewShoot->setCenterOfRotation(getPhysicalBody()->GetPosition());
        m_shoots.push(NewShoot);
    }
    else if(m_shoots.size() >= 18)
    {
        auto b = m_shoots.front()->getPhysicalBody();
        m_shoots.pop();
        //m_world->DestroyBody(b);
    }
}

void Laser::update()
{
    auto copy = m_shoots;
    while (!copy.empty())
    {
        copy.front()->setWorldCenterOfRotation(m_worldCenterOfRotation);
        copy.front()->update();
        copy.pop();
    }
    MovableObject::update();
}

void Laser::draw(sf::RenderWindow& window)
{
    auto copy = m_shoots;
    while (!copy.empty())
    {
        copy.front()->draw(window);
        copy.pop();
    }
    MovableObject::draw(window);
}

void Laser::rotateLeft()
{
    m_orientation = ((m_orientation + 1) + 4) % 4;
    auto copy = m_shoots;
    while (!copy.empty())
    {
        copy.front()->rotateLeft();
        copy.pop();
    }
    MovableObject::rotateLeft();
    m_clock.restart();
}

void Laser::rotateRight()
{
    m_orientation = ((m_orientation - 1) + 4) % 4;
    auto copy = m_shoots;
    while (!copy.empty())
    {
        copy.front()->rotateRight();
        copy.pop();
    }
    MovableObject::rotateRight();
    m_clock.restart();
}
void Laser::setTimeForShooting(sf::Time t)
{
    m_timeToShoot = t;
}

b2Vec2 Laser::setNewShootTheVelosity()
{
    if ((m_orientation % 4 == 0 && m_shootToTheRight)||
        (m_orientation % 4 == 2 && !m_shootToTheRight))
            return { 1,0 };
    if ((m_orientation % 4 == 0 && !m_shootToTheRight) ||
        (m_orientation % 4 == 2 && m_shootToTheRight))
            return { -1,0 };
        
    if ((m_orientation % 4 == 1 && !m_shootToTheRight) ||
        (m_orientation % 4 == 3 && m_shootToTheRight))
            return { 0,1 };
        
    if ((m_orientation % 4 == 1 && m_shootToTheRight) ||
        (m_orientation % 4 == 3 && !m_shootToTheRight))
            return { 0,-1 };
        
}