#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Maths.h"
#include "MovableObject.h"
#include "GameObjectFactory.h"
#include "Resources.h"
#include "Platform.h"

// the object fired by the laser
class LaserBeam : public MovableObject {

public:
    LaserBeam(b2World& world, float x, float y, float width, float height);
    void rotateLeft() override;        
    void rotateRight() override;

private:
    b2CircleShape m_circleShape;
    static bool m_registerit; // for factory register

};