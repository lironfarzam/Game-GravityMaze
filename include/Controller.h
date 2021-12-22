#pragma once
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Menu.h"

class StateMachine;

class Controller {

public:

	Controller();
	void run();

    enum class GameMode
    {
        Play,
        LevelCompleted,
        LevelComplete,
        LevelFailed,
        GameOver,
        MenuState,
        Help,
    };

    Board& getBoard();
    void displayBoard();
    void displayMenu();
    void displayHelp();
    void displayLevelComplete();
    void displayGameOver();

    //void handleEvents();
    void menuHandleEvent();
    void gameHandleEvent();

    Menu& getMenu();

private:
    sf::RenderWindow m_window;
    Board m_board;
    Menu m_menu;
    bool m_modeChanged = false;

    StateMachine* m_stateMachine;
};

