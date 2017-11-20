#include <pebble.h>
#include "splash_window.h"
#include "initials_window.h"
#include "highscore_window.h"
#include "main.h"


GameResult highscores[HIGHSCORE_LENGTH];
int cntPlayersInHighscoreList; 
int current_player_points;
char current_player_name[5];


void launch_initials_window(){
	window_stack_push(initials_window_get_window(), true);
}

int main(){

	cntPlayersInHighscoreList = -1;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "'main'     cntPlayersInHighscoreList = %d   ",  cntPlayersInHighscoreList);

	/* Turn on the lights */
	light_enable(true);
	
	/* Create windows */
	splash_window_create();

	initials_window_create(); // creates highscore_window before unloading	
	window_stack_push(splash_window_get_window(), true);
	app_timer_register(100, launch_initials_window, NULL); // sets display time for splash window before starting initials window

	// load highscores, before a new game starts	
	import_highscores();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "'main'     cntPlayersInHighscoreList = %i   ",  cntPlayersInHighscoreList);
	
	/* Keep the app running */
	app_event_loop();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "'main'     cntPlayersInHighscoreList = %i   ",  cntPlayersInHighscoreList);

	/* save highscores in highscore_window_load */
	APP_LOG(APP_LOG_LEVEL_DEBUG, "'main'     cntPlayersInHighscoreList = %i   ",  cntPlayersInHighscoreList);

	/* Close windows */
	splash_window_destroy();
	initials_window_destroy();
	highscore_window_destroy();
	
	/* Turn of the lights */
	light_enable(false);
}
