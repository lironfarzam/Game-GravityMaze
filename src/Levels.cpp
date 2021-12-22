
#include "Levels.h"
using namespace std;


// load the maps from file - called only once
void Levels::loadMapsFromFile(const char* file_name) {

    ifstream if1;
    if1.open(file_name);
    int linenumber = 0;

    if (!if1)
        throw std::runtime_error(OPEN_FILE_ERR);

    int tile_type, tile_rotation;
    int x, y, dir, left, right, rotate, orientation, numtiles, spring = 0, timer = 0, numlaser, shootPrtSec;
    int mapindex = 0;
    bool mapcomplete = false; // we read a whole map
    double zoom;

    try {
        while (true) {
            std::istringstream line = readLine(if1);
            if (line.eof() || (line >> std::ws).eof()) {
                // empty line means EOF
                break;
            }

            linenumber++;
            m_maps.push_back(GameMap());
            line >> skipws >> timer;
            m_maps[mapindex].setTimerValue(timer);
            line = readLine(if1);

            line >> skipws >> x;
            line >> skipws >> y;
            line >> skipws >> dir;

            if (!(line.eof() || (line >> std::ws).eof()))
                throw std::runtime_error(EXTRA_DATA_ERR + std::to_string(linenumber));

            m_maps[mapindex].setPlayer(x, y, dir);

            line = readLine(if1);
            line >> skipws >> x;
            line >> skipws >> y;
            line >> skipws >> dir;

            if (!(line.eof() || (line >> std::ws).eof()))
                throw std::runtime_error(EXTRA_DATA_ERR + std::to_string(linenumber));

            m_maps[mapindex].setDoor(x, y, dir);

            line = readLine(if1);
            line >> skipws >> numtiles;
            m_maps[mapindex].resizePlatform(numtiles);
            // read the map
            mapcomplete = false;
            for (int row = 0; row < numtiles; row++) {
                line = readLine(if1);
                linenumber++;

                line >> x;
                line >> y;
                line >> left;
                line >> right;
                line >> rotate;
                line >> orientation;
                line >> spring;

                m_maps[mapindex].addTile(row, x, y, left, right, rotate, orientation, spring);

                if (!(line.eof() || (line >> std::ws).eof())) {
                    throw std::runtime_error(EXTRA_DATA_ERR + std::to_string(linenumber));
                }
            }

            line = readLine(if1);
            line >> skipws >> numlaser;
            m_maps[mapindex].resizeLaser(numlaser);
            // read the map
            mapcomplete = false;
            for (int row = 0; row < numlaser; row++) {
                line = readLine(if1);
                linenumber++;

                line >> x;
                line >> y;
                line >> orientation;
                line >> right;
                line >> shootPrtSec;

                m_maps[mapindex].addLaser(row, x, y, orientation, right, spring);

                if (!(line.eof() || (line >> std::ws).eof())) {
                    throw std::runtime_error(EXTRA_DATA_ERR + std::to_string(linenumber));
                }
            }
            mapcomplete = true;
            mapindex++;
            CLogger::GetLogger()->Log("map loaded!");
        }

        if1.close();
        CLogger::GetLogger()->Log("Loading game maps completed!");
    }
    catch (std::ios_base::failure& )
    {
        throw std::runtime_error(MISSING_DATA_ERR + std::to_string(linenumber));
    }
    catch (std::runtime_error& e)
    {
        throw e;
    }
    catch (std::exception& )
    {
        if (!mapcomplete)
            throw std::runtime_error(INCOMPLETE_ERR + std::to_string(linenumber));
    }

}

std::istringstream Levels::readLine(ifstream& m_istr) {
    auto line = std::string();
    m_istr.clear();

    if (std::getline(m_istr, line)) {
        // store our current line
       auto curLine = std::istringstream(line);

        // configure stream to throw exception upon invalid data
        curLine.exceptions(std::ios::failbit | std::ios::badbit);

        return curLine;
    }
    else {
        // return empty string
        auto line = std::istringstream("");
        line.exceptions(std::ios::failbit | std::ios::badbit);
        return line;
    }
}

int Levels::getCurrentLevel() const
{
    return m_current_level;
}

void Levels::setCurrentLevel(int lev)
{
    if (lev <0 || lev > m_maps.size())
        lev = 0;
    m_current_level = lev;
}

bool Levels::loadNextMap()
{
    if (m_current_level < m_maps.size()-1) {
        m_current_level++;
        return true;
    }
    return false;
}

GameMap Levels::getCurrentMap() const
{
    return m_maps[m_current_level];
}

