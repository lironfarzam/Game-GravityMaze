#pragma once

#include "CLogger.h"
#include "GameMap.h"
#include "constants.h"
#include "Resources.h"
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <iostream>
#include <istream>

// a class to load game levels
class Levels {
public:
    void loadMapsFromFile(const char* file_name);
    bool loadNextMap();

    int getCurrentLevel() const;
    void setCurrentLevel(int lev);

    GameMap getCurrentMap() const;

private:

    std::vector<GameMap> m_maps;
    int m_current_level = 0;

    std::istringstream readLine(std::ifstream& m_istr);

};


