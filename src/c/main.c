#include <pebble.h>
#include "splash_window.h"
#include "initials_window.h"
#include "highscore_window.h"
#include "main.h"
#include "game.h"

GameResult highscores[HIGHSCORE_LENGTH];

int cntPlayersInHighscoreList; 
int current_player_points;
char current_player_name[5];


// void launch_game_window() {
// 	window_stack_pop(false);
// 	window_stack_push(game_window_get_window(), true);
// }

int main() {
	cntPlayersInHighscoreList = -1;

	/* Turn on the lights */
	light_enable(true);
	
	/* Create windows */
	splash_window_create();

// initials_window_create(); // creates highscore_window before unloading	

	game_window_create();

	window_stack_push(splash_window_get_window(), true);
	app_timer_register(TIMEOUT_SPLASH_SCREEN, launch_game_window, NULL); // sets display time for splash window before starting initials window

	// load highscores, before a new game starts	
	import_highscores();
	
	/* Keep the app running */
	app_event_loop();

	/* save highscores in highscore_window_load */

	/* Close windows */
	splash_window_destroy();
	initials_window_destroy();
	highscore_window_destroy();
	game_window_destroy();
	
	/* Turn of the lights */
	light_enable(false);
}
