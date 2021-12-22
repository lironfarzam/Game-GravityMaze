#include "Meteor.h"

// no template - code example from moodle
bool Meteor::m_registerit = GameObjectFactory::registerit("Meteor",
    [](b2World& world, float x, float y, float w, float h)-> std::shared_ptr<WorldObject> {return std::make_shared<Meteor>(world, x, y, w, h); });


Meteor::Meteor(b2World& world, float x, float y, float w, float h)
    : MovableObject(world, x, y, w, h) {

    m_scale = 1.f;

    //*********************
    // after we stored them in m_x m_y we convert to screen coordinates
    x = x * TILE_WIDTH;
    y = y * TILE_HEIGHT;

    m_graphicBody.setTexture(Resources::instance().getImage(METEOR_IMAGE));
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
    
    m_graphicBody.setRotation(toDegree(m_physicalBody->GetAngle())); // ->GetAngle() * 180 / b2_pi);
}

void Meteor::update()
{
    if (getPosition().x > WIN_WIDTH || getPosition().y > WIN_HEIGHT) {
        int vx = randRange(-5, 5);
        int vy = randRange(1, 5);
        getPhysicalBody()->SetLinearVelocity(b2Vec2(vx, vy));
        m_physicalBody->SetTransform(b2Vec2(vy*10, 0), atan2(vx, vy) + 3 * M_PI / 4);
    }
    m_graphicBody.setRotation(toDegree(m_physicalBody->GetAngle()));
}


void Meteor::rotateLeft()
{
    auto vel = m_physicalBody->GetLinearVelocity();
    const float c = cos(toRadian(-90));
    const float s = sin(toRadian(-90));
    const float x = vel.x * c - vel.y * s;
    const float y = vel.x * s + vel.y * c;
    m_physicalBody->SetLinearVelocity({ x ,y });
    MovableObject::rotateLeft();
}

void Meteor::rotateRight()
{
    auto vel = m_physicalBody->GetLinearVelocity();
    const float c = cos(toRadian(90));
    const float s = sin(toRadian(90));
    const float x = vel.x * c - vel.y * s;
    const float y = vel.x * s + vel.y * c;
    m_physicalBody->SetLinearVelocity({ x ,y });
    MovableObject::rotateRight();
}

