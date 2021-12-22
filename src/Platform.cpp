
#include "Platform.h"
#include <iostream>
#include "RoundedRectangleShape.h"


 bool Platform::m_registerit = GameObjectFactory::registerit("Platform",
     [](b2World& world, float x, float y, float w, float h)-> std::shared_ptr<WorldObject> {return std::make_shared<Platform>(world, x, y, w, h); });


Platform::Platform(b2World& world, float x, float y, float w, float h)
        : MovableObject(world, x, y, w, h) {

    m_scale = 1.f;

    // load a sprite and set a rect shape with repeated patterns
    sf::Texture m_texture = Resources::instance().getImage(PLATFORM_IMAGE);
    m_texture.setRepeated(true);

    //*********************
    // after we stored them in m_x m_y we convert to screen coordinates
    x = x * TILE_WIDTH;
    y = y * TILE_HEIGHT;
    // the platform

    m_graphicBody.setTexture(Resources::instance().getImage(PLATFORM_IMAGE));
    m_graphicBody.setTextureRect(sf::IntRect(0,0,TILE_WIDTH * w, TILE_HEIGHT * h ));
    m_graphicBody.setOrigin(m_graphicBody.getTextureRect().width / 2, m_graphicBody.getTextureRect().height / 2);
    m_graphicBody.setPosition(x, y);
    m_graphicBody.setScale(m_scale, m_scale);

    // the pivot sprite
    m_pivotTexture = Resources::instance().getImage(PIVOT_IMAGE);
    m_pivotSprite.setTexture(m_pivotTexture);
    m_pivotSprite.setScale(TILE_WIDTH, TILE_HEIGHT);
    m_pivotSprite.setOrigin(TILE_WIDTH / 2, TILE_HEIGHT / 2);
    m_pivotSprite.setPosition(x, y);
    m_pivotSprite.setScale(TILE_WIDTH / m_pivotSprite.getLocalBounds().width ,  TILE_HEIGHT / m_pivotSprite.getLocalBounds().height);

    //create b2BodyDef - the sprite with Box2D coord m_scaled
    b2BodyDef body;
    body.position = b2Vec2(x / BOX2D_SCALE, y / BOX2D_SCALE); // body is the sprite
    body.type = b2_kinematicBody; // b2_dynamicBody;
    body.gravityScale = 0; // NO GRAVITY for platforms!! DIFF

    // add b2BodyDef to world ->> create physical body
    m_physicalBody = world.CreateBody(&body);
    m_physicalBody->SetUserData(this); // for collision

    m_centerOfRotation = m_physicalBody->GetPosition(); // by default

    // Shape is a b2PolygonShape - params hal width/height
    m_shape.SetAsBox(m_graphicBody.getTextureRect().width  * m_scale / (2 * BOX2D_SCALE),
                     m_graphicBody.getTextureRect().height * m_scale / (2 * BOX2D_SCALE));

    // set Shape to b2FixtureDef
    b2FixtureDef FixtureDef;
    FixtureDef.density = 1.f;
    FixtureDef.friction = 0.05f; // DIFF
    FixtureDef.shape = &m_shape;

    // create fixture for physical body
    m_physicalBody->CreateFixture(&FixtureDef);

    // set rotation for our sprite
    m_graphicBody.setRotation(m_physicalBody->GetAngle() * 180/b2_pi);

}



void Platform::setPosition(float x, float y) {
    WorldObject::setPosition(x, y);
    m_pivotSprite.setPosition(x, y);
    //m_platformShape.setPosition(x, y);
}

void Platform::update()
{
    // ->> bouncing of player on platform if non zero
    m_physicalBody->SetAngularVelocity(toRadian(0));

    MovableObject::update();

}

void Platform::draw(sf::RenderWindow & window)
{
    WorldObject::draw(window);

    // adjust the pivot also
    m_pivotSprite.setPosition(m_physicalBody->GetPosition().x * BOX2D_SCALE, m_physicalBody->GetPosition().y * BOX2D_SCALE);
    m_pivotSprite.setRotation(toDegree(m_physicalBody->GetAngle()));

    if (m_rotatable)
        window.draw(m_pivotSprite);

    sf::RoundedRectangleShape roundedRectangle (sf::Vector2f(m_graphicBody.getLocalBounds().width, m_graphicBody.getLocalBounds().height),8,5); // RoundedRectangleShape
    roundedRectangle.setSize(sf::Vector2f(m_graphicBody.getLocalBounds().width, m_graphicBody.getLocalBounds().height));
    //roundedRectangle.setCornersRadius(5);
    roundedRectangle.setPosition(m_graphicBody.getPosition().x, m_graphicBody.getPosition().y);
    roundedRectangle.setOutlineThickness(4);
    roundedRectangle.setOutlineColor(sf::Color(128, 128, 128));
    roundedRectangle.setFillColor(sf::Color::Transparent);
    roundedRectangle.setOrigin(m_graphicBody.getOrigin());
    roundedRectangle.setRotation(m_graphicBody.getRotation());
    //roundedRectangle.setTexture(& Resources::instance().getImage(PLATFORM_IMAGE));

    window.draw(roundedRectangle);
}

int Platform::getOrientation() const {
    return m_orientation;
}

void Platform::setOrientation(int mOrientation) {
    m_orientation = mOrientation;
}

bool Platform::getRotatable() const
{
    return m_rotatable;
}

void Platform::setRotatable(bool r)
{
    m_rotatable = r;
}


int Platform::getLeftTiles() const {
    return m_leftTiles;
}

void Platform::setLeftTiles(int mLeftTiles) {
    m_leftTiles = mLeftTiles;
}

int Platform::getRightTiles() const {
    return m_rightTiles;
}

void Platform::setRightTiles(int mRightTiles) {
    m_rightTiles = mRightTiles;
}




