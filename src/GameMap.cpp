

#include "GameMap.h"

GameMap::GameMap()
{
}

void GameMap::addTile(int row, int x, int y, int left, int right, int rotate, int orientation, int spring) {
    m_platforms[row].emplace_back(x);
    m_platforms[row].emplace_back(y);
    m_platforms[row].emplace_back(left);
    m_platforms[row].emplace_back(right);
    m_platforms[row].emplace_back(rotate);
    m_platforms[row].emplace_back(orientation);
    m_platforms[row].emplace_back(spring);
}

void GameMap::addLaser(int row, int x, int y, int orientation, int right, int speed)
{
    m_lasers[row].emplace_back(x);
    m_lasers[row].emplace_back(y);
    m_lasers[row].emplace_back(orientation);
    m_lasers[row].emplace_back(right);
    m_lasers[row].emplace_back(speed);

}

void GameMap::resizePlatform(int size)
{
    m_platforms.resize(size);
}
void GameMap::resizeLaser(int size)
{
    m_lasers.resize(size);
}

void GameMap::setPlayer (int x, int y, int dir) {
    m_player.emplace_back(x);
    m_player.emplace_back(y);
    m_player.emplace_back(dir);
}
void GameMap::setDoor (int x, int y , int dir) {
    m_door.emplace_back(x);
    m_door.emplace_back(y);
    m_door.emplace_back(dir);
}


std::vector<int> GameMap::getPlayer() const {
    return m_player;
}

std::vector<std::vector<int>> GameMap::getPlatforms() const {
    return m_platforms;
}

std::vector<std::vector<int>> GameMap::getlasers() const
{
    return m_lasers;
}

std::vector<int> GameMap::getDoor() const {
    return m_door;
}

int GameMap::getTimerValue() const
{
    return m_timer_value;
}

void GameMap::setTimerValue(int timer)
{
    m_timer_value = timer;
}

