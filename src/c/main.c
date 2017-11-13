#include <pebble.h>
#include "splash_window.h"
#include "initials_window.h"

void launch_initials_window(){
	window_stack_push(initials_window_get_window(), true);
}

int main(){
	/* Turn on the lights */
	light_enable(true);
	
	/* Create windows */
	splash_window_create();
	initials_window_create();
	
	window_stack_push(splash_window_get_window(), true);
	app_timer_register(1000, launch_initials_window, NULL); // sets display time for splash window before starting initials window
	
	
	/* Keep the app running */
	app_event_loop();
	
	/* Close windows */
	splash_window_destroy();
	//initials_window_destroy();
	
	/* Turn of the lights */
	light_enable(false);
}