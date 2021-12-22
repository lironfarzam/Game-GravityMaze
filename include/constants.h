#pragma once


/* BOX2D */
const static float BOX2D_SCALE = 20.f;
const static float DEGTORAD = 0.0174532925199432957f;

/* SHAPES */
constexpr auto ShapeHeight = 44.f;
constexpr auto ShapeWidth = 40.f;
constexpr auto ShapeSpeed = ShapeWidth * 3;
const static int TILE_WIDTH = 44;
const static int TILE_HEIGHT = 44;
const static int PLAYER_WIDTH = 44;
const static int PLAYER_HEIGHT = 44;

/* WINDOW */
const static int WIN_HEIGHT = 767;
const static int WIN_WIDTH = 1024;

const static int RES_X = 1400;
const static int RES_Y = 1000;


/* ANIMATION */
const float ROTATE_SNAP_SPEED = 0.15f;
constexpr auto ANIMATION_SPEED = 0.2f;

const static int DIR_UP = 0;
const static int DIR_RIGHT = 1;
const static int DIR_DOWN = 2;
const static int DIR_LEFT = 3;
const static int DIR_NONE = 4; // no rotation
const static int MAX_DIRECTION = DIR_LEFT; // valid file input

/* IMAGES */
const static int PLAYER_IMAGE = 0;
const static int ASTRONAUT_IMAGE = 1;
const static int PIVOT_IMAGE = 2;
const static int BG_IMAGE = 3;
const static int PLATFORM_IMAGE = 4;
const static int SOLVED_IMAGE = 5;
const static int GAMEOVER_IMAGE = 6;
const static int WELCOME_IMAGE = 7;
const static int DOOR_IMAGE = 8;
const static int HELP_IMAGE = 9;
const static int METEOR_IMAGE = 10;
const static int DEAD_IMAGE = 11;
const static int SPRINGBOARD_IMAGE = 12;
const static int LASER_IMAGE = 13;
const static int LASER_GUN_IMAGE = 14;

/* SOUNDS */
const static int WATER_SOUND = 0; 
const static int BG_SOUND = 1;
const static int CLICK_SOUND = 2; 
const static int ROTATE_SOUND = 3;
const static int DEAD_SOUND = 4;
const static int LEVELCOMPLETE_SOUND = 5;

const static bool LOOP = true;
const static bool NO_LOOP = false;

/*  ERROR MESSAGES */
const static char* OPEN_FILE_ERR = "Cannot open game file";
const static char* EXTRA_DATA_ERR = "Corrupted input file (extra data) at line ";
const static char* MISSING_DATA_ERR = "Missing or unexpected data in file at line ";
const static char* INCOMPLETE_ERR = "Incomplete map at line ";

/* UI */
const int MENU_WIDTH = 1120;
const int MENU_HEIGHT = 768;
const int BUTTON_WIDTH = 270;
const int BUTTON_HEIGHT = 130;
constexpr auto GAME_OVER_MESSAGE = "You Won !";
constexpr auto WELCOME_MESSAGE = "Welcome";
const static int STATUS_HEIGHT = 80;

