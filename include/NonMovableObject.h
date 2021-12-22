#pragma once

#include "WorldObject.h"

// unused in current game but we leave it for
// future implementation of non movable objects in the game
class NonMovableObject : public WorldObject {
public:
	NonMovableObject();
	bool isObstacle() const;

protected:
	bool m_isObstacle = false;
};


