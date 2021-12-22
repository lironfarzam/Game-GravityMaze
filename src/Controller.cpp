#include "Controller.h"
#include <iostream>

#include "StateMachine.h"

Controller::Controller() : m_window(sf::RenderWindow(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "Gravity Maze")),
m_board(&m_window), m_menu(&m_window)//, m_commands(this)
{
    m_window.setFramerateLimit(60);

    m_stateMachine = new StateMachine(this);

    // add our game states
    m_stateMachine->addState(Controller::GameMode::Play, std::make_shared<PlayState>());
    m_stateMachine->addState(Controller::GameMode::LevelCompleted, std::make_shared<LevelCompletedState>());
    m_stateMachine->addState(Controller::GameMode::LevelComplete, std::make_shared<LevelCompleteState>());
    m_stateMachine->addState(Controller::GameMode::LevelFailed, std::make_shared<LevelFailedState>());
    m_stateMachine->addState(Controller::GameMode::GameOver, std::make_shared<GameOverState>());
    m_stateMachine->addState(Controller::GameMode::MenuState, std::make_shared<MenuState>());
    m_stateMachine->addState(Controller::GameMode::Help, std::make_shared<HelpState>());
    m_stateMachine->setCurrentState(GameMode::MenuState);

    Resources::instance().playSound(BG_SOUND, LOOP, 80);

}

void Controller::run()
{
    m_menu.resetWindowSize();
    try {
        m_board.loadMaps();
        CLogger::GetLogger()->Log("maps loaded");

        while (m_window.isOpen())
        {
            m_stateMachine->handleState();
        }
    }
    catch (std::runtime_error& e) {
        // exit - failure to load maps
        CLogger::GetLogger()->Log(e.what());
        CLogger::GetLogger()->Log("Failure to load game, exiting!");
    }
    
}

// create one COMMAND per case for Command Pattern
void Controller::menuHandleEvent() {
    for (auto event = sf::Event(); m_window.pollEvent(event); ) {
        switch (event.type) {
            case sf::Event::Closed:
                m_window.close();
                break;
            case sf::Event::MouseMoved:
                if (m_stateMachine->getCurrentState() == GameMode::MenuState || m_stateMachine->getCurrentState() == GameMode::GameOver)
                    m_menu.trackMouse(event.mouseMove.x, event.mouseMove.y);
                break;
            case sf::Event::KeyPressed:
                if (m_stateMachine->getCurrentState() == GameMode::Help) {
                    m_stateMachine->setCurrentState(GameMode::MenuState);
                    break;
                }
                if (m_stateMachine->getCurrentState() == GameMode::LevelComplete 
                    || m_stateMachine->getCurrentState() == GameMode::LevelFailed) {
                    m_stateMachine->setCurrentState(GameMode::Play);
                    break;
                }
                if (m_stateMachine->getCurrentState() == GameMode::GameOver) {
                    m_stateMachine->setCurrentState(GameMode::MenuState);
                }
                // game over, menu, help
                switch (event.key.code) {
                    case sf::Keyboard::Escape:
                        m_window.close();
                        break;
                    case sf::Keyboard::P:
                        m_stateMachine->setCurrentState(GameMode::Play);
                        break;
                    case sf::Keyboard::H:
                        m_stateMachine->setCurrentState(GameMode::Help);
                        break;
                    default:
                        break;
                }
                break;

            case sf::Event::MouseButtonReleased: {
                if (m_stateMachine->getCurrentState() == GameMode::Help) {
                    m_stateMachine->setCurrentState(GameMode::MenuState);
                    break;
                }
                if (m_stateMachine->getCurrentState() == GameMode::LevelComplete 
                    || m_stateMachine->getCurrentState() == GameMode::LevelFailed) {
                    m_stateMachine->setCurrentState(GameMode::Play);
                    break;
                }
                if (m_stateMachine->getCurrentState() == GameMode::GameOver) {
                    m_stateMachine->setCurrentState(GameMode::MenuState);
                    m_board.restartGame();
                }

                Menu::Command m = m_menu.click(event.mouseButton.x, event.mouseButton.y);
                if (m == Menu::Command::Play) {
                    m_stateMachine->setCurrentState(GameMode::Play);
                } else if (m == Menu::Command::Help) {
                    m_stateMachine->setCurrentState(GameMode::Help);
                }

                if (m == Menu::Command::Quit)
                    m_window.close();
            }
                break;
            default:
                break;
        }
    }
}

void Controller::gameHandleEvent()
{
    for (auto event = sf::Event(); m_window.pollEvent(event); ) {
        switch (event.type) {
            case sf::Event::Closed:{
                m_window.close();
                break;
            }
            case sf::Event::KeyPressed: {
                if (event.key.code == sf::Keyboard::Escape)
                    m_stateMachine->setCurrentState(GameMode::MenuState);
                else
                    m_board.execGameCommand(event.key.code);
                break;
            }
            case sf::Event::KeyReleased: {
                m_board.stopPlayer();
                m_board.setWorldRotating(false); // -> find a new place for this.
                break;
            }
        }
    }
}

Menu& Controller::getMenu()
{
    return m_menu;
}

void Controller::displayLevelComplete()
{
    m_window.clear();
    m_board.drawGameWorld();
    m_board.drawLevelComplete();
    m_window.display();
}



void Controller::displayHelp()
{
    m_window.clear();
    m_menu.drawHelp();
    m_window.display();
}

void Controller::displayMenu()
{
    m_window.clear();
    m_menu.draw();
    m_window.display();
}

void Controller::displayGameOver()
{

    m_window.clear();
    m_board.drawGameOver();
    m_window.display();
}

Board& Controller::getBoard()
{
    return m_board;
}

void Controller::displayBoard()
{
    m_window.clear();
    m_board.update(); // b2d world update
    m_board.drawGameWorld();
    m_window.display();
}

