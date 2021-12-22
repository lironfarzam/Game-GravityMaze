#include "Springboard.h"

bool Springboard::m_registerit = GameObjectFactory::registerit("Springboard",
    [](b2World& world, float x, float y, float w, float h)-> std::shared_ptr<WorldObject> {return std::make_shared<Springboard>(world, x, y, w, h); });


Springboard::Springboard(b2World& world, float x, float y, float w, float h)
    : MovableObject(world, x, y, w, h) {

    m_scale = 1.f;

    m_graphicBody.setTexture(Resources::instance().getImage(SPRINGBOARD_IMAGE));
    x = x * TILE_WIDTH;// m_graphicBody.getTextureRect().width;
    y = y * TILE_HEIGHT;// m_graphicBody.getTextureRect().height;

    m_graphicBody.setTextureRect(sf::IntRect(0, 0, m_graphicBody.getTextureRect().width * (w), m_graphicBody.getTextureRect().height * (h)));
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
    m_graphicBody.setRotation(toDegree(m_physicalBody->GetAngle()));

}

void Springboard::setPlatform(std::shared_ptr<Platform> p)
{
    m_platform = p;
}

std::shared_ptr<Platform> Springboard::getPlatform()
{
    return m_platform;
}

int Springboard::getOrientation() const
{
    return m_orientation;
}

void Springboard::setOrientation(int or )
{
    m_orientation = or ;
}

