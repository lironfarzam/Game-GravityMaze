
#define _USE_MATH_DEFINES
#include <math.h>

#include <random>
#include "Maths.h"

void centerOrigin(sf::Sprite& sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}


float toDegree(float radian)
{
    return 180.f / 3.141592653589793238462643383f * radian;
}

float toRadian(float degree)
{
    return 3.141592653589793238462643383f / 180.f * degree;
}

int randomInt(int exclusiveMax)
{
    std::uniform_int_distribution<> distr(0, exclusiveMax - 1);
    //return distr(RandomEngine);
    return 0;
}


/// Angle in radians.
void rotateVector(float* dx, float* dy, float angle) {
    float ndx = *dx * cos(angle) - *dy * sin(angle);
    float ndy = *dx * sin(angle) + *dy * cos(angle);
    *dx = ndx;
    *dy = ndy;
}

float orientationToAngle(int orientation) {
    return orientation * M_PI_2;
}


// clamped between -PI and PI.
float clampedAngularDifference(float fromAngle, float toAngle) {
    float diff = toAngle - fromAngle;
    while (diff <= -M_PI) diff += M_2PI;
    while (diff > M_PI) diff -= M_2PI;
    return diff;
}

// clamped between -PI and PI.
float clampedAngle(float angle) {
    while (angle <= -M_PI) angle += M_2PI;
    while (angle > M_PI) angle -= M_2PI;
    return angle;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

bool rectsIntersectSafe(int x1a, int y1a, int x2a, int y2a, int x1b, int y1b, int x2b, int y2b) {
    if (x2a < x1a) swap(&x1a, &x2a);
    if (y2a < y1a) swap(&y1a, &y2a);
    if (x2b < x1b) swap(&x1b, &x2b);
    if (y2b < y1b) swap(&y1b, &y2b);
    return (x2a > x1b && x2b > x1a && y2a > y1b && y2b > y1a);
}

bool rectsIntersect(int x1a, int y1a, int x2a, int y2a, int x1b, int y1b, int x2b, int y2b) {
    return (x2a > x1b && x2b > x1a && y2a > y1b && y2b > y1a);
}

bool isWithinRect(int x, int y, int x1, int y1, int x2, int y2) {
    return (x > x1 && y > y1 && x < x2 && y < y2);
}

bool isWithinRectSafe(int x, int y, int x1, int y1, int x2, int y2) {
    if (x2 < x1) swap(&x1, &x2);
    if (y2 < y1) swap(&y1, &y2);
    return (x > x1 && y > y1 && x < x2 && y < y2);
}

float dotProduct(float dx1, float dy1, float dx2, float dy2) {
    return dx1*dx2 + dy1*dy2;
}

int div_floor(int n, int d) {
    int q = n / d;
    int r = n%d;
    if ((r != 0) && ((r < 0) != (d < 0)))--q;
    return q;
}

float randomAngle() {
    return rand() % 5000 / 5000.0 * M_2PI;
}

float randRange(float a, float b) {
    return rand() % 5000 / 5000.0 * (b - a) + a;
}