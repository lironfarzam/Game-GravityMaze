
#include "NonMovableObject.h"


NonMovableObject::NonMovableObject() :WorldObject()
{
}

bool NonMovableObject::isObstacle() const
{
    return m_isObstacle;
}
