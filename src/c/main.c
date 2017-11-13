#include <pebble.h>
#include "splash_window.h"

int main(){
	light_enable(true);
	splash_window_create();
	window_stack_push(splash_window_get_window(), true);
	
	app_event_loop();
	
	light_enable(false);
	splash_window_destroy();
}