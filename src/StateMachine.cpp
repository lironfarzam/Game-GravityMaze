#include "StateMachine.h"

#include "Controller.h"

/************** STATE MACHINE ****************/

/*     state pattern  */

StateMachine::StateMachine(Controller* c)
{
    m_controller = c;
}


void StateMachine::addState(Controller::GameMode m, std::shared_ptr<GameState> state)
{
    m_states.insert({ m, state });
}

void StateMachine::handleState()
{
    m_states[m_currentState]->execute(this, m_controller);
}

void StateMachine::setCurrentState(Controller::GameMode newstate) {
    m_currentState = newstate;
}

Controller::GameMode StateMachine::getCurrentState() const
{
    return m_currentState;
}

void PlayState::execute(StateMachine* m, Controller* c)
{
    // playing GameMode::Play

    c->getBoard().checkBoardState();

    c->getBoard().step();

    c->gameHandleEvent();
    c->displayBoard();
    //
    if (c->getBoard().levelCompleted())
        m->setCurrentState(Controller::GameMode::LevelCompleted);
    if (c->getBoard().gameIsOver())
        m->setCurrentState(Controller::GameMode::GameOver);
    if (c->getBoard().playerIsDead())
        m->setCurrentState(Controller::GameMode::LevelFailed);
}

void LevelCompletedState::execute(StateMachine* m, Controller* c)
{
    m->setCurrentState(Controller::GameMode::LevelComplete);
}

void LevelCompleteState::execute(StateMachine* m, Controller* c)
{

    c->displayLevelComplete();
    c->menuHandleEvent();
}

void LevelFailedState::execute(StateMachine* m, Controller* c)
{
    c->displayLevelComplete();
    c->menuHandleEvent();
}

void GameOverState::execute(StateMachine* m, Controller* c)
{
    c->displayGameOver();
    c->menuHandleEvent();
}

void MenuState::execute(StateMachine* m, Controller* c)
{
    c->displayMenu();
    c->menuHandleEvent();
}

void HelpState::execute(StateMachine* m, Controller* c)
{
    c->displayHelp();
    c->menuHandleEvent();
    c->getBoard().setGameOver(false);
}
