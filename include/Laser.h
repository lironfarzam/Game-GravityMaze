#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Maths.h"
#include "MovableObject.h"
#include "GameObjectFactory.h"
#include "Resources.h"
#include "Platform.h"
#include "LaserBeam.h"
#include <queue>

// the base class for platforms
class Laser : public MovableObject {

public:

    Laser(b2World& world, float x, float y, float width, float height);
    virtual ~Laser() = default;
    void setPlatform(std::shared_ptr<Platform> p);
    std::shared_ptr<Platform> getPlatform();

    int getOrientation() const;
    void setOrientation(int or );

    void setSwitch(bool);
    bool getSwitch();

    void setDirctionToShoot(bool);
    void shootLaser();

    void update() override;
    void draw(sf::RenderWindow& window) override;

    void rotateLeft()override;
    void rotateRight()override;

    void setTimeForShooting(sf::Time);
    b2Vec2 setNewShootTheVelosity();

private:
    int m_orientation; // up (0) right (1) down (2) left (3)

    sf::Texture m_texture;
    
    static bool m_registerit;
    std::shared_ptr<Platform> m_platform = NULL;

    sf::Clock m_clock;
    sf::Time m_timeToShoot = sf::seconds(0.50f);
    bool m_switch = true;
    bool m_shootToTheRight = false;
    int m_range = 30;
    b2World* m_world;
    std::queue< std::shared_ptr<LaserBeam>> m_shoots;
};

