#pragma once

class Controller;
class StateMachine;

#include "Controller.h"

/**************** STATE MACHINE *************************/

class GameState {
private:
    Controller::GameMode m_state;
public:
    GameState() {}
    virtual void execute(StateMachine* m, Controller* c) = 0;
};

class StateMachine {

public:
    StateMachine(Controller* c);
    virtual ~StateMachine() = default;

    void addState(Controller::GameMode m, std::shared_ptr<GameState> state);

    void handleState();

    void setCurrentState(Controller::GameMode  newstate);
    Controller::GameMode getCurrentState() const;

private:

    Controller::GameMode m_currentState;
    std::map<Controller::GameMode, std::shared_ptr<GameState>> m_states;
    Controller* m_controller;
};

class PlayState : public GameState {
public:
    void execute(StateMachine* m, Controller* c) override;
};

class LevelCompletedState : public GameState {
public:
    void execute(StateMachine* m, Controller* c) override;
};

class LevelCompleteState : public GameState {
public:
    void execute(StateMachine* m, Controller* c) override;
};

class LevelFailedState : public GameState {
public:
    void execute(StateMachine* m, Controller* c) override;
};

class GameOverState : public GameState {
public:
    void execute(StateMachine* m, Controller* c) override;
};

class MenuState : public GameState {
public:
    void execute(StateMachine* m, Controller* c) override;
};


class HelpState : public GameState {
public:
    void execute(StateMachine* m, Controller* c) override;
};


