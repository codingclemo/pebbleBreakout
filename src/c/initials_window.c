#include <pebble.h>
#include <stdio.h>
#include <string.h>
#include "initials_window.h"
#include "highscore_window.h"
#include "main.h"

Window *initials_window;
TextLayer *initials_title;
TextLayer *user_initials;
TextLayer *indicator;

char username[] = "aaa";
int charPosition = 0;
char arrow[] = "^     ";
const int arrowLength = sizeof(arrow);
void initials_window_unload(Window *window);


void setArrow(){
	for (int i = 0; i < arrowLength; i++){
		arrow[i] = ' ';
	}
	arrow[charPosition*2+1] = '^';
}

void launch_highscore_window(){
	highscore_window_create();
	window_stack_push(highscore_window_get_window(), true);
}

/*** Button handling ***/
static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	  if (charPosition == 0) {
		/* go back to menu or do nothing */
	} else {
		charPosition --;
		setArrow();
		text_layer_set_text(indicator, arrow);
	}
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (charPosition == 2) {
		/* adapt display */
		text_layer_set_text(initials_title, "Name saved!");
		text_layer_set_text_color(user_initials, GColorRed);
		text_layer_destroy(indicator);
		/* save the name and score in Highscore list */

		// TODO: insert sorted by points
		if (cntPlayersInHighscoreList < HIGHSCORE_LENGTH) {
			cntPlayersInHighscoreList++;

			APP_LOG(APP_LOG_LEVEL_DEBUG, " username:   %s", username);
			APP_LOG(APP_LOG_LEVEL_DEBUG, "select_click_handler() cntPlayersInHighscoreList = %d ", cntPlayersInHighscoreList);

			// copy the username to the "name" filed of the struct 
			for(int i = 0; i < 3; i++) {
				highscores[cntPlayersInHighscoreList].name[i] = username[i];
			}
			highscores[cntPlayersInHighscoreList].name[4] = '\0';

			// dummy value 
			highscores[cntPlayersInHighscoreList].points = 12;

			APP_LOG(APP_LOG_LEVEL_DEBUG, "initals final values: ");
			APP_LOG(APP_LOG_LEVEL_DEBUG, "player name:   %s", highscores[cntPlayersInHighscoreList].name);
			APP_LOG(APP_LOG_LEVEL_DEBUG, "player points:   %d", highscores[cntPlayersInHighscoreList].points);

		}

		// persist_write_string(1, username); // save current player name
		// int user_points = 9; //hardcoded for now
		// persist_write_int(2, user_points); // save current player points // needs to be saved after game
		
		//node_ptr user_data;
		//user_data = create_node(username, user_points);
		//highscore_list = NULL;
		//insert_node(highscore_list, user_data);
		
		app_timer_register(2500, launch_highscore_window, NULL);
	} else {
		charPosition ++;
		setArrow();
		text_layer_set_text(indicator, arrow);
	}
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	char currentChar = username[charPosition];
	if (currentChar >= 122) {
		username[charPosition] = 'a';
	} else {
		username[charPosition] = currentChar + 1;	
	}
	text_layer_set_text(user_initials, username);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	char currentChar = username[charPosition];
	if (currentChar <= 97) {
		username[charPosition] = 'z';
	} else {
		username[charPosition] = currentChar - 1;	
	}
	text_layer_set_text(user_initials, username);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

/*** Window handling ***/
void initials_window_load(Window *window){
	Layer *window_layer = window_get_root_layer(window);
	
	
	/* Display headline */
	initials_title = text_layer_create(GRect(0,50,144,70));
	text_layer_set_text(initials_title, "Enter initials:");
	text_layer_set_text_alignment(initials_title, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(initials_title));
	
	/* Display user initials */
	user_initials = text_layer_create(GRect(0,70,144,90));
	text_layer_set_text(user_initials, username);
	text_layer_set_text_alignment(user_initials, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(user_initials));
	
	/* Indicator */
	indicator = text_layer_create(GRect(0,90,144,140));
	text_layer_set_text(indicator, arrow);
	text_layer_set_text_color(indicator, GColorRed);
	text_layer_set_text_alignment(indicator, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(indicator));


}

void initials_window_unload(Window *window){
	text_layer_destroy(user_initials);
	text_layer_destroy(indicator);
}

void initials_window_create(){
	initials_window = window_create();
	window_set_click_config_provider(initials_window, click_config_provider);
	window_set_window_handlers(initials_window, (WindowHandlers) {
		.load = initials_window_load,
		.unload = initials_window_unload
	});
}

void initials_window_destroy(){
	window_destroy(initials_window);
}

Window *initials_window_get_window(){
	return initials_window;
}