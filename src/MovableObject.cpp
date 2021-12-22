

#include "MovableObject.h"


namespace
{
    sf::Vector2f dirToVector(MovableObject::Action dir)
    {
        switch (dir)
        {
        case MovableObject::Action::Right: return { 1, 0 };
        case MovableObject::Action::Left:  return { -1, 0 };
        case MovableObject::Action::Up:    return { 0, -1 };
        case MovableObject::Action::Down:  return { 0, 1 };
        case MovableObject::Action::Stay:  return { 0, 0 };
        }
        return { 0, 0 }; // just to silence the warning about no return
        // would be better to throw an exception but we haven't learned about it yet
    }
}

MovableObject::MovableObject(b2World& world, float x, float y, float w, float h) :
                    WorldObject(world,x,y,w,h)
{
    m_clock.restart();
}

void MovableObject::move(sf::Time deltaTime)
{
    sf::Vector2f v = dirToVector(m_dir) * ShapeSpeed * deltaTime.asSeconds();
    m_x = m_x + v.x;
    m_y = m_y + v.y;
}



void MovableObject::setState(State s)
{
    m_state = s;
}

MovableObject::State MovableObject::getState()
{
    return m_state;
}

void MovableObject::rotateLeft()
{
    m_targetAngle -= toRadian(90);
    //b2d angles are between -180 / +180
    if (m_targetAngle < (-b2_pi))
        m_targetAngle = m_targetAngle + (2 * b2_pi);
}

void MovableObject::rotateRight()
{
    m_targetAngle += toRadian(90);
    //b2d angles are between -180 / +180
    if (m_targetAngle > b2_pi)
        m_targetAngle = m_targetAngle - (2 * b2_pi);
}