#include "Menu.h"
#include <iostream>

Menu::Menu(sf::RenderWindow* w): m_playButton(67, 550, BUTTON_WIDTH, BUTTON_HEIGHT), 
                                m_quitButton(785, 550, BUTTON_WIDTH, BUTTON_HEIGHT),
                                m_helpButton(425, 550, BUTTON_WIDTH, BUTTON_HEIGHT)
{
    m_window = w;
    m_message = "";
    m_border.setOutlineThickness(10);
    m_border.setOutlineColor(sf::Color(255, 255,0));
    m_border.setFillColor(sf::Color::Transparent);
}

// display the opening image with play and quit buttons
void Menu::draw() {

    sf::Sprite sp = sf::Sprite(Resources::instance().getImage(WELCOME_IMAGE));
    sp.setPosition(sf::Vector2f(0, 0));
    m_window->draw(sp);

    if (m_message.length() > 0) {
        sf::Text text;
        text.setFont(Resources::instance().getFont());
        text.setStyle(sf::Text::Bold);
        text.setString(m_message);
        text.setFillColor(sf::Color(255, 255, 102)); 
        text.setCharacterSize(128);
        //text.setPosition(sf::Vector2f(350, 400));

        sf::Rect bounds = text.getGlobalBounds();
        bounds.top = 300;
        bounds.left = (MENU_WIDTH - bounds.width) / 2;
        text.setPosition(bounds.left, bounds.top);

        m_window->draw(text);
    }
    if (m_needBorder)
        m_window->draw(m_border);
}

void Menu::drawHelp() {
    sf::Sprite sp = sf::Sprite(Resources::instance().getImage(HELP_IMAGE));
    sp.setPosition(sf::Vector2f(0, 0));
    m_window->draw(sp);
}

void Menu::resetWindowSize() {
    m_window->setSize(sf::Vector2u(MENU_WIDTH, MENU_HEIGHT));
    m_window->setView(sf::View(sf::FloatRect(0, 0, MENU_WIDTH  , MENU_HEIGHT)));
    m_window->clear();

}

void Menu::trackMouse(int x, int y)
{
    if (m_playButton.contains(x, y)) {
        m_border.setSize(sf::Vector2f(m_playButton.width, m_playButton.height));
        m_border.setPosition(m_playButton.left, m_playButton.top);
        m_needBorder = true;
    }
    else
    if (m_quitButton.contains(x, y)) {
        m_border.setSize(sf::Vector2f(m_quitButton.width, m_quitButton.height));
        m_border.setPosition(m_quitButton.left, m_quitButton.top);
        m_needBorder = true;
    }
    else
    if (m_helpButton.contains(x, y)) {
        m_border.setSize(sf::Vector2f(m_helpButton.width, m_helpButton.height));
        m_border.setPosition(m_helpButton.left, m_helpButton.top);
        m_needBorder = true;
    }
    else
        m_needBorder = false;
}

void Menu::setMessage(std::string mess)
{
    m_message = mess;
}

// returns the clicked button, or None
Menu::Command Menu::click(int x, int y)
{
    if (m_playButton.contains(x, y)) {
        Resources::instance().playSound(CLICK_SOUND, NO_LOOP, 100);
        return Menu::Command::Play;
    }
    else
    if (m_quitButton.contains(x, y)) {
        Resources::instance().playSound(CLICK_SOUND, NO_LOOP, 100);
        return Menu::Command::Quit;
    }
    else
    if (m_helpButton.contains(x, y)) {
        Resources::instance().playSound(CLICK_SOUND, NO_LOOP, 100);
        return Menu::Command::Help;
    }

    return Menu::Command::None;
}


