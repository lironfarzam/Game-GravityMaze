#pragma once


#include <vector>
#include <SFML/Graphics.hpp>
#include "constants.h"
#include "Resources.h"

class Menu {
public:
    Menu(sf::RenderWindow* w);
    void draw();
    void drawHelp();
    void resetWindowSize();
    void trackMouse(int x, int y);
    void setMessage(std::string);

    enum class Command
    {
        Play,
        Quit,
        None,
        Help
    };

    Command click(int, int);

private:
    std::vector<char> m_items;
    sf::RenderWindow* m_window;

    sf::IntRect m_quitButton;
    sf::IntRect m_playButton;
    sf::IntRect m_helpButton;

    sf::RectangleShape m_border;
    bool m_needBorder = false;

    std::string m_message;
};


