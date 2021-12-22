#pragma once

#include <math.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <random>
#include <cmath>
#include <ctime>
#include <cassert>

#define _USE_MATH_DEFINES
#include <math.h>

namespace sf
{
    class Sprite;
    class Text;
}

// Call setOrigin() with the center of the object
void			centerOrigin(sf::Sprite& sprite);
void			centerOrigin(sf::Text& text);

// Degree/radian conversion
float			toDegree(float radian);
float			toRadian(float degree);

// Random number generation
int				randomInt(int exclusiveMax);



const double M_2PI = 2 * M_PI;

void rotateVector(float* dx, float* dy, float angle);

float orientationToAngle(int orientation);

float clampedAngularDifference(float fromAngle, float toAngle);

float clampedAngle(float angle);

void swap(int* a, int* b);

bool rectsIntersectSafe(int x1a, int y1a, int x2a, int y2a, int x1b, int y1b, int x2b, int y2b);

bool rectsIntersect(int x1a, int y1a, int x2a, int y2a, int x1b, int y1b, int x2b, int y2b);

bool isWithinRect(int x, int y, int x1, int y1, int x2, int y2);

bool isWithinRectSafe(int x, int y, int x1, int y1, int x2, int y2);

float dotProduct(float dx1, float dy1, float dx2, float dy2);

int div_floor(int n, int d);

float randomAngle();

float randRange(float a, float b);
