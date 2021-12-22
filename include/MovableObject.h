#pragma once
#include "WorldObject.h"

class MovableObject : public WorldObject {

public:

    enum class State
    {
        onGround,
        isFalling,
        isJumping,
        inSpace
    };

    MovableObject(b2World& world, float x, float y,float w, float h);

    virtual void move(sf::Time deltaTime);
    virtual void rotateLeft();
    virtual void rotateRight();

    void setState(State);
    State getState();

protected:

    State m_state;

    sf::Clock m_clock; // for PNG animated
};

