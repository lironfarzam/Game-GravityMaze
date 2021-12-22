
#include "WorldObject.h"

WorldObject::WorldObject():m_x(0), m_y(0),m_isDisposed(false)
{
}

WorldObject::WorldObject(b2World& world, float x, float y, float w, float h) :
                m_x(x), m_y(y), m_width(w), m_height(h)
{
}

sf::FloatRect WorldObject::getGlobalBounds() const
{
    return m_graphicBody.getGlobalBounds();
}

bool WorldObject::isDisposed() const
{
    return m_isDisposed;
}

void WorldObject::draw(sf::RenderWindow & window)
{
    // also in update
    m_graphicBody.setRotation(toDegree(m_physicalBody->GetAngle()));
    m_graphicBody.setPosition(m_physicalBody->GetPosition().x * BOX2D_SCALE, m_physicalBody->GetPosition().y * BOX2D_SCALE);

    window.draw(m_graphicBody);
}

WorldObject *WorldObject::getCollisionWith() const {
    return collisionWith;
}

void WorldObject::setCollisionWith(WorldObject *collisionWith) {
    WorldObject::collisionWith = collisionWith;
}


void WorldObject::setPosition(float x, float y)
{
    b2Vec2 vect = b2Vec2(x / BOX2D_SCALE, y / BOX2D_SCALE); // was 10 - david
    m_physicalBody->SetTransform(vect, 0);
    m_graphicBody.setPosition(x, y);
}

sf::Vector2f WorldObject::getPosition() const {
    return m_graphicBody.getPosition();
}

b2Vec2 WorldObject::getPhysicalPosition() const {
    return m_physicalBody->GetPosition();
}

/*
 * update the sprite with box2d physica body position
 * and compute the new angle in case we have a targetAngle (used for rotating platforms)
 */
void WorldObject::update()
{

    // targetAngle is set for platforms rotating
    // http://www.iforce2d.net/b2dtut/rotate-to-angle
    float totalRotation = m_targetAngle - m_physicalBody->GetAngle();

    // http://www.iforce2d.net/b2dtut/rotate-to-angle
    // fix case when when the target position is below the body - ?
    while ( totalRotation < -180 * DEGTORAD ) totalRotation += 360 * DEGTORAD;
    while ( totalRotation > 180 * DEGTORAD ) totalRotation -= 360 * DEGTORAD;

    //if (totalRotation != 0) {
    if (abs(toDegree(totalRotation)) >= 0.001f) {
        float change = toRadian(5); //allow 5 degree rotation per time step

        if (totalRotation < 0)
            change = -change;

        float newAngle = m_physicalBody->GetAngle() + b2Min( change, b2Max(-change, totalRotation));
        auto diff = abs (newAngle -  (floor(newAngle / 0.25) * 0.25));

        if (diff <= 1.f) {
            newAngle = ceil(newAngle / 0.25) * 0.25; // was floor 
        }

        // rotate with translation

        if (abs(m_targetAngle - newAngle) != 0.f) {
            b2RotateAroundPoint(*m_physicalBody, change, m_worldCenterOfRotation);
        } 
        else {
            m_physicalBody->SetTransform(m_physicalBody->GetPosition(), m_targetAngle);
        }
    }

    // move graphics object - done in draw
}


int WorldObject::getNumContacts() const {
    return m_numContacts;
}

void WorldObject::setNumContacts(int mNumContacts) {
    m_numContacts = mNumContacts;
}

b2Body* WorldObject::getPhysicalBody() const
{
    return m_physicalBody;
}

void WorldObject::setCenterOfRotation(b2Vec2 center)
{
    m_centerOfRotation = center;
}

void WorldObject::setWorldCenterOfRotation(b2Vec2 v)
{
    m_worldCenterOfRotation = v;
}

b2Vec2 WorldObject::getWorldCenterOfRotation() const
{
    return m_worldCenterOfRotation;
}

// toda raba
// https://gamedev.stackexchange.com/questions/109115/rotating-kinematic-object-around-a-moving-point-box2d

void WorldObject::b2RotateAroundPoint(b2Body& body, float amount, b2Vec2 worldPoint)
{
    const b2Transform xfm = body.GetTransform();
    const b2Vec2 p = xfm.p - worldPoint;
    const float c = cos(amount);
    const float s = sin(amount);
    const float x = p.x * c - p.y * s;
    const float y = p.x * s + p.y * c;
    const b2Vec2 pos = b2Vec2(x, y) + worldPoint;
    float angle = xfm.q.GetAngle() + amount;
    body.SetTransform(pos, angle);
}


void  WorldObject::handleCollision() {
}
void  WorldObject::handleEndCollision() {
}

sf::Sprite WorldObject::getBody() {
    return m_graphicBody;
}

void WorldObject::kbdToAction(sf::Keyboard::Key key)
{
    switch (key)
    {
    case sf::Keyboard::Right:     m_dir = Action::Right; break;
    case sf::Keyboard::Left:     m_dir = Action::Left;  break;
    case sf::Keyboard::Up:     m_dir = Action::Up;    break;
    case sf::Keyboard::Down:     m_dir = Action::Down;  break;
    case sf::Keyboard::Space: m_dir = Action::Stay;  break;
    case sf::Keyboard::A: m_dir = Action::RotateLeft;  break;
    case sf::Keyboard::S: m_dir = Action::RotateRight;  break;
    }
}
