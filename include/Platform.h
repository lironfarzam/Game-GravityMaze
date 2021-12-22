#pragma once
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Maths.h"
#include "MovableObject.h"
#include "GameObjectFactory.h"
#include "Resources.h"

// the class for platforms
class Platform : public MovableObject {

public:

	Platform(b2World& world, float x, float y, float width, float height);

    void draw(sf::RenderWindow & window) override;
    void setPosition(float x, float y) override;
	void update() override;

    int getLeftTiles() const;
    void setLeftTiles(int mLeftTiles);
    int getRightTiles() const;
    void setRightTiles(int mRightTiles);
    int getOrientation() const;
    void setOrientation(int mOrientation);

    void setRotatable(bool r);
    bool getRotatable() const;

private:

    int m_rightTiles;
    bool rotatable;
    int m_leftTiles;

	int m_pivotRow, m_pivotCol; // pivot coordinates in block units
	int m_orientation; // up (0) right (1) down (2) left (3)
    bool m_rotatable;

	sf::Texture m_texture;
    sf::Texture m_pivotTexture;
    sf::Sprite m_pivotSprite;

    static bool m_registerit;
};

