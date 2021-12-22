#pragma once
#include <box2d/box2d.h>
#include <typeinfo>
#include "WorldObject.h"
#include <iostream>


struct UnknownCollision : public std::runtime_error
{
    UnknownCollision(WorldObject& a, WorldObject& b)
            : std::runtime_error(std::string("Unknown collision of ") + typeid(a).name() + " and " + typeid(b).name())
    {
    }
};

void processCollision(WorldObject* object1, WorldObject* object2);
