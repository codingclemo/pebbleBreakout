#include <pebble.h>
#include "splash_window.h"
#include "initials_window.h"
#include "highscore_window.h"
#include "main.h"

void launch_initials_window(){
	window_stack_push(initials_window_get_window(), true);
}

int main(){

	cntPlayersInHighscoreList = -1;

	/* Turn on the lights */
	light_enable(true);
	
	/* Create windows */
	splash_window_create();
	initials_window_create(); // creates highscore_window before unloading
	
	window_stack_push(splash_window_get_window(), true);
	app_timer_register(1000, launch_initials_window, NULL); // sets display time for splash window before starting initials window
	
	// load highscores, before a new game starts	
				//for testing
				persist_write_string(103, "du3");
				persist_write_int(113, 7);
	import_highscores();
	
	/* Keep the app running */
	app_event_loop();
	
	/* save highscores in highscore_window_load */
	
	/* Close windows */
	splash_window_destroy();
	initials_window_destroy();
	highscore_window_destroy();
	
	/* Turn of the lights */
	light_enable(false);
}