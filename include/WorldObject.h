#pragma once

#include <SFML/Graphics.hpp>
#include "constants.h"
#include <iostream>
#include <box2d/box2d.h>
#include "Maths.h"

class Enemy;
class Player;

class WorldObject {

public:

    enum class Action
    {
        Up,
        Right,
        Down,
        Left,
        Stay,
        RotateLeft,
        RotateRight
    };

    WorldObject();

    WorldObject(b2World& world, float x, float y, float w, float h);

    virtual ~WorldObject() = default;

    sf::FloatRect getGlobalBounds() const;
    void kbdToAction(sf::Keyboard::Key key);

    bool isDisposed() const;

    virtual void draw(sf::RenderWindow& window) ;
    void virtual handleCollision();
    void virtual handleEndCollision();

    virtual void update();

    virtual void setPosition(float x, float y);
    b2Vec2 getPhysicalPosition() const; // box2d world
    sf::Vector2f getPosition() const; // graphics window

    virtual sf::Sprite getBody();

    WorldObject *getCollisionWith() const;
    void setCollisionWith(WorldObject *collisionWith);

    int getNumContacts() const;
    virtual void setNumContacts(int mNumContacts);

    b2Body* getPhysicalBody() const;

    void setCenterOfRotation(b2Vec2 center); // object's center

    void setWorldCenterOfRotation(b2Vec2 v); // world center
    b2Vec2 getWorldCenterOfRotation() const;
    void b2RotateAroundPoint(b2Body& body, float amount, b2Vec2 worldPoint);

protected:

    b2Vec2 m_worldCenterOfRotation;
    float m_targetAngle = 0; // in radians

    std::vector<sf::Texture> all_textures; // loading the images
    bool m_isDisposed = false;

    float m_x = 0;
    float m_y = 0;
    float m_width = 0;
    float m_height = 0;

    Action m_dir = Action::Stay;

    // box2d data
    sf::Sprite m_graphicBody;
    b2Body* m_physicalBody;
    b2PolygonShape m_shape;

    b2Vec2 m_centerOfRotation;

    float m_scale = 20.f;

    int m_numContacts = 0;

private:
    WorldObject * collisionWith = NULL;
};

// for box2d collision filters
enum objectCategory {
    PLAYER =   0x0001,
    PLATFORM = 0x0002,
    DOOR =     0x0004,
    METEOR =   0x0008,
    SPRING =   0x0016,
    LASER  =   0x0032
};