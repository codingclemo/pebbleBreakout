#pragma once

#include "pebble.h"

#define HIGHSCORE_LENGTH 5

typedef struct GameResult {
    char name[5];
    int points;         // or int32_t ?? how to convert this to a string in highscore_window?
} GameResult;

extern GameResult highscores[HIGHSCORE_LENGTH];
extern int cntPlayersInHighscoreList; 
extern int current_player_points;
extern char current_player_name[5];
