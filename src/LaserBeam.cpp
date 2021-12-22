#include "LaserBeam.h"


// no template - code example from moodle
bool LaserBeam::m_registerit = GameObjectFactory::registerit("LaserBeam",
    [](b2World& world, float x, float y, float w, float h)-> std::shared_ptr<WorldObject> {return std::make_shared<LaserBeam>(world, x, y, w, h); });


LaserBeam::LaserBeam(b2World& world, float x, float y, float w, float h)
    : MovableObject(world, x, y, w, h) {

    m_scale = 1.f;

    //*********************
    // after we stored them in m_x m_y we convert to screen coordinates
    x = x * BOX2D_SCALE;
    y = y * BOX2D_SCALE;

    m_graphicBody.setTexture(Resources::instance().getImage(LASER_IMAGE));
    m_graphicBody.setTextureRect(sf::IntRect(0, 0, TILE_WIDTH * (w), TILE_HEIGHT * (h)));
    m_graphicBody.setOrigin(m_graphicBody.getTextureRect().width / 2, m_graphicBody.getTextureRect().height / 2);

    m_graphicBody.setScale(m_scale, m_scale);


    //create b2BodyDef - the sprite with Box2D coord m_scaled
    b2BodyDef body;
    body.position = b2Vec2(x / BOX2D_SCALE, y / BOX2D_SCALE); // body is the sprite
    body.type = b2_kinematicBody;

    // add b2BodyDef to world ->> create physical body
    m_physicalBody = world.CreateBody(&body);
    m_physicalBody->SetUserData(this); // for collision
    m_centerOfRotation = m_physicalBody->GetPosition(); // by default

    m_circleShape.m_radius = m_graphicBody.getTextureRect().width * m_scale / (2 * BOX2D_SCALE); // 1.0f;

    // set Shape to b2FixtureDef
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.05f; // 0.05f;
    FixtureDef.shape = &m_circleShape; // &m_shape; //

    // create fixture for physical body
    m_physicalBody->CreateFixture(&FixtureDef);

    // set rotation for our sprite
    m_graphicBody.setPosition(x, y);
    m_graphicBody.setRotation(m_physicalBody->GetAngle() * 180 / b2_pi);
}

void LaserBeam::rotateLeft()
{
    auto vel = m_physicalBody->GetLinearVelocity();
    const float c = cos(toRadian(-90));
    const float s = sin(toRadian(-90));
    const float x = vel.x * c - vel.y * s;
    const float y = vel.x * s + vel.y * c;
    m_physicalBody->SetLinearVelocity({ x ,y });
    MovableObject::rotateLeft();
}

void LaserBeam::rotateRight()
{
    auto vel = m_physicalBody->GetLinearVelocity();
    const float c = cos(toRadian(90));
    const float s = sin(toRadian(90));
    const float x = vel.x * c - vel.y * s;
    const float y = vel.x * s + vel.y * c;
    m_physicalBody->SetLinearVelocity({ x ,y });
    MovableObject::rotateRight();
}



