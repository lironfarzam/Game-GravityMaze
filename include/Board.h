#pragma once

#include <vector>
#include <memory>

#include "constants.h"
#include "Levels.h"
#include "Player.h"

#include "CollisionListener.h"

#include <SFML/Graphics.hpp>
#include "Platform.h"
#include "Door.h"
#include "Meteor.h"
#include "Laser.h"
#include "Springboard.h"
#include "DebugDraw.h"

class MovableObject;

class Board {
public:
	Board(sf::RenderWindow*);

	// maps
	void loadMaps(); // load all maps
	bool loadNextMap(); // build data straucture 
	int getCurrentLevel() const;
	void setCurrentLevel(int) ;

	// check if level is complete and load next map if any
	void checkBoardState();

	// display game
	void drawGameWorld() const;
	void printStats() const;
	void drawLevelComplete() const;
	void drawGameOver() const;
	void drawWelcome() const;
    void execGameCommand(sf::Keyboard::Key key);

	// the SFML window
	float getWindowHeight() const;
	float getWindowWidth() const;
	void resetWindowSize();

	// the game clock
	bool timeIsOver() const;
	void resetGameClock();
	int getGameClock();

	// game states
	bool gameIsOver() const;
	void setGameOver(bool b);
	bool levelCompleted() const;
	void restartGame();

	void rotateWorld(sf::Keyboard::Key act);

	void update(); // update b2d world
	void step();

	void stopPlayer();
	bool playerIsDead() const;

	void setWorldRotating(bool);
	bool getWorldRotating();
	


private:
	void resetGame();

	// all maps of the games
	Levels m_levels;

	sf::Clock m_gameClock; // curent level time

	std::vector< std::shared_ptr<WorldObject>> m_gameObjects;

	std::vector< std::shared_ptr<Platform>> m_platforms; // list of platforms
    std::shared_ptr<Player> m_player;
    std::shared_ptr<Door> m_door;
	std::vector< std::shared_ptr<Meteor>> m_meteors;
	std::vector< std::shared_ptr<Springboard>> m_springboards;
	std::vector< std::shared_ptr<Laser>> m_lasers;

	// maybe to move
	void loadMapObjects();

	// the drawing window 
	sf::RenderWindow * m_window;

	bool m_gameIsOver = false;
	bool m_playerIsDead = false; // current level

	float m_xoffset = 0; // to center the board
	float m_yoffset = 0;

	int m_tapCount = 0;

	sf::Sprite m_background;

    std::unique_ptr<b2World> m_world;

    int32 m_velocityIterations = 6;
    int32 m_positionIterations = 2;

	CollisionListener m_collisionListener;

	bool m_isWorldRotating = false;
	int m_gravity_int = 0;

	// box2d debugger
	DebugDraw *m_debugDraw;

	// game hacks
	const b2Vec2 GRAVITY_LIKE_EARTH = { 0.f,9.8f };
	const b2Vec2 GRAVITY_TO_DOWN = { 0.f,1.8f };
	const b2Vec2 GRAVITY_TO_RIGHT = { 1.8f,0.f };
	const b2Vec2 GRAVITY_TO_UP = { 0.f,-1.8f };
	const b2Vec2 GRAVITY_TO_LEFT = { -1.8f,0.f };
	b2Vec2 m_gravity_arr[5] = { {GRAVITY_LIKE_EARTH},{GRAVITY_TO_DOWN}, {GRAVITY_TO_RIGHT},{GRAVITY_TO_UP},{GRAVITY_TO_LEFT} };
	void activCodeLiron();
	bool liron_code = false;
	void rotGravityTo(int i);
	sf::Keyboard::Key getGravityIndex(sf::Keyboard::Key);

};
