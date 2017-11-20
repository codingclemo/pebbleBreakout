#pragma once

#include "pebble.h"

#define HIGHSCORE_LENGTH 5

typedef struct GameResult {
    char name[5];
    int points;         // or int32_t ?? how to convert this to a string in highscore_window?
} GameResult;

GameResult highscores[HIGHSCORE_LENGTH];
int cntPlayersInHighscoreList; 
int current_player_points;
char current_player_name[5];

