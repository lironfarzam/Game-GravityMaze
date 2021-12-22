#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Maths.h"
#include "MovableObject.h"
#include "GameObjectFactory.h"
#include "Resources.h"
#include "Platform.h"

// the base class for platforms
class Meteor : public MovableObject {

public:

    Meteor(b2World& world, float x, float y, float width, float height);
    void update() override;
    void rotateLeft() override;
    void rotateRight() override;

private:
    b2CircleShape m_circleShape;
    static bool m_registerit;

};