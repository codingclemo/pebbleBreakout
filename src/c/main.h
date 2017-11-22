#pragma once

#include "pebble.h"

#define HIGHSCORE_LENGTH 5

#define TIMEOUT_SPLASH_SCREEN 10   // 3000
#define TIMEOUT_INITIALS_SCREEN 10   // 1000
#define TIMEOUT_HIGHSCORE_SCREEN 10   // 500

typedef struct GameResult {
    char name[5];
    int points;         // or int32_t ?? how to convert this to a string in highscore_window?
} GameResult;

extern GameResult highscores[HIGHSCORE_LENGTH];
extern int cntPlayersInHighscoreList; 
extern int current_player_points;
extern char current_player_name[5];

