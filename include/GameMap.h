#pragma once


#include <vector>

// a class to hold the levels data from file
class GameMap {

public:

    GameMap();

    void resizePlatform(int size);
    void resizeLaser(int size);

    void setPlayer (int, int, int) ;
    void setDoor (int, int, int);
    void addTile(int row, int x, int y, int left, int right, int rotate, int orientation, int connect);
    void addLaser(int row, int x, int y, int orientation, int right, int speed);

    std::vector<int> getPlayer() const;
    std::vector<std::vector<int>> getPlatforms() const;
    std::vector<std::vector<int>> getlasers() const;
    std::vector<int> getDoor() const;

    int getTimerValue() const;
    void setTimerValue(int timer);

private:

    std::vector<std::vector<int>> m_platforms; // 6 int per line
    std::vector<std::vector<int>> m_lasers; // 5 int per line
    std::vector<int> m_door; // x y direction
    std::vector<int> m_player; // x y direction

    int m_height;
    int m_width;
    int m_timer_value;
};


