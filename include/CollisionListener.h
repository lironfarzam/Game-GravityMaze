#pragma once
#include <box2d/box2d.h>
#include "WorldObject.h"
#include "CollisionMultiMap.h"

// Box2d collision listener - based on multimethods
class CollisionListener : public b2ContactListener {
public:
    CollisionListener();
    void BeginContact(b2Contact * contact) override;
    void EndContact(b2Contact * contact)  override;
};
