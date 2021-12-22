#pragma once

#include "MovableObject.h"
#include "constants.h"
#include "Resources.h"
#include "GameObjectFactory.h"

#include "Platform.h"
#include "Maths.h"


class Player : public MovableObject {

public:
    Player(b2World& world, float x, float y, float w, float h);

    void execAction(sf::Keyboard::Key);
    void update() override;

    // new b2d

    void moveLeft();
    void moveRight();
    void jump();
    void stop();

    void rotateLeft() override;
    void rotateRight() override;

    void setNumContacts(int mNumContacts) override;
    bool getAlive() const;
    void setAlive(bool);
    int getLives() const;
    int getScore() const;
    void setLives(int);
    void setScore(int);
    bool getLevelComplete() const;
    void setLevelComplete(bool b);

    void handleCollision() override;
    void draw(sf::RenderWindow& window) override;
    void  handleEndCollision() override;

private:

    bool m_levelComplete = false;

    // b2d data
    float m_maxSpeed = 1.f;
    float m_force = 9.0f;
    bool m_onGround = true;

    // our stuff
    int m_score = 0;
    int m_lives = 3;
    int m_direction = 0;
    bool m_isAlive = true;

    float rotatingBaseDX;
    float rotatingBaseDY;

    Platform* currentTile;

    // for our factory
    static bool m_registerit;

    sf::IntRect m_rectInitSprite; // image start position for animation in PNG

};


