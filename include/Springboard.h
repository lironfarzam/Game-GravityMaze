#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Maths.h"
#include "MovableObject.h"
#include "GameObjectFactory.h"
#include "Resources.h"
#include "Platform.h"

// the base class for platforms
class Springboard : public MovableObject {

public:

    Springboard(b2World& world, float x, float y, float width, float height);
    virtual ~Springboard() = default;
    void setPlatform(std::shared_ptr<Platform> p);
    std::shared_ptr<Platform> getPlatform();
    int getOrientation() const;
    void setOrientation(int or );

private:
    int m_orientation; // up (0) right (1) down (2) left (3)
    sf::Texture m_texture;
    static bool m_registerit;
    std::shared_ptr<Platform> m_platform = NULL;
};
