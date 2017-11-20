#include <pebble.h>
#include <stdlib.h>
#include <string.h>

#include "highscore_window.h"
#include "initials_window.h"
#include "main.h"

#define HS_CNT_PLAYERS 90
#define HS_NAMES 100
#define HS_POINTS 110

Window *highscore_window;
TextLayer *highscore_title;
TextLayer *highscore_text;

char highscore_list_text[100];

void import_highscores() {
	cntPlayersInHighscoreList = 0; 
	cntPlayersInHighscoreList = persist_read_int(HS_CNT_PLAYERS);

//<<<<<<< use_global_variable_for_highscore
	for (int i = 0; i <= cntPlayersInHighscoreList; i++) {
		if (!persist_exists(HS_POINTS + i) || !persist_exists(HS_NAMES + i)) {
			APP_LOG(APP_LOG_LEVEL_ERROR, "Error: there should be points/names in the storage");
		} else {
			highscores[i].points = persist_read_int(HS_POINTS + i);
			int result = persist_read_data(HS_NAMES, &highscores[i].name, 4);
			if (result != 4) {
				APP_LOG(APP_LOG_LEVEL_ERROR, "Error READING from storage");
			}
		}
		
		
/*/=======
void import_highscores(){
	char buffer[4];
	int stored_points;
	persist_write_string(103, "du3");
	persist_write_int(113, 7);
	
	for (int i = 1; i <= HIGHSCORE_LENGTH; i++){
		// Get name 
		if (persist_exists(100+i)) {
			persist_read_string(100+i, buffer, sizeof(buffer));
			names[i] = buffer;
		} else {
			//buffer = "ddd";
			names[i] = "ddd";
		}
		//names[i] = buffer;
		// Get points 
		stored_points = persist_exists(110+i) ? persist_read_int(110+i) : 0;	
		//stored_points = persist_read_int(110+i);
		snprintf(buffer, 10, "%d", stored_points);
		points[i] = buffer;
//>>>>>>> master */
	}

	// TODO: REMOVE DEBUG ONLY - write data to APP LOG
	for (int i = 0; i <= cntPlayersInHighscoreList; i++) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Player name %s, points: %d", highscores[i].name, highscores[i].points);
	}
}

void create_single_highscore_string() {
	// create a single string from the array of players
	highscore_list_text[0] = ' ';
	highscore_list_text[1] = '\0';

	char seperator[3] = ": ";
	char points[12];
	char newLine[2] = "\n";
	char position[2];

	APP_LOG(APP_LOG_LEVEL_DEBUG, "create_single_highscore_string() cntPlayersInHighscoreList = %d ", cntPlayersInHighscoreList);
	
	for (int i = 1; i <= cntPlayersInHighscoreList; i++) {
		// copy name of the player into "highscore_list_text"
		snprintf(position, sizeof(position), "%d", i);
		strcat(highscore_list_text, position);
		strcat(highscore_list_text, seperator);
		strcat(highscore_list_text, highscores[i].name);
		strcat(highscore_list_text, seperator);

		APP_LOG(APP_LOG_LEVEL_DEBUG, "1 full highscore_list_text string:   %s", highscore_list_text);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "1 highscores[i].nameg:   %s", highscores[i].name);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "1 seperator:   %s", seperator);

		// copy points of the player into "highscore_list_text"
		snprintf(points, sizeof(points), "%d", highscores[i].points);	
		strcat(highscore_list_text, points);
		
		APP_LOG(APP_LOG_LEVEL_DEBUG, "2 full highscore_list_text string:   %s", highscore_list_text);

		// add new line to  "highscore_list_text"		 
		strcat(highscore_list_text, newLine);
	}
	APP_LOG(APP_LOG_LEVEL_DEBUG, "END full highscore_list_text string:   %s", highscore_list_text);
}


void write_highscores(){
	// save, how many players are in the high score list 	
	persist_write_int(HS_CNT_PLAYERS, cntPlayersInHighscoreList);

	for (int i = 0; i <= cntPlayersInHighscoreList; i++) {
		persist_write_int(HS_POINTS + i, highscores[i].points);
		int result = persist_write_data(HS_NAMES, &highscores[i].name, 4);
		if (result != 4) {
			APP_LOG(APP_LOG_LEVEL_ERROR, "Error writing to storage");
		}
	}
}




static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
//	highscore_window_unload(highscore_window);
	highscore_window_destroy();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

/*** Window handling ***/
void highscore_window_load(Window *window){
	Layer *window_layer = window_get_root_layer(window);
	
	highscore_title = text_layer_create(GRect(0,10,144,20));
	text_layer_set_background_color(highscore_title, GColorOrange);
	text_layer_set_text_color(highscore_title, GColorWhite);
	text_layer_set_text(highscore_title, "HIGHSCORES");
	text_layer_set_text_alignment(highscore_title, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(highscore_title));
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "highscore_window_load()  calling create_single_highscore_string()");
	create_single_highscore_string();

	highscore_text = text_layer_create(GRect(0,50,144,160));
	text_layer_set_text(highscore_text, highscore_list_text);
	text_layer_set_text_alignment(highscore_text, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(highscore_text));
}

void highscore_window_unload(Window *window){
	text_layer_destroy(highscore_title);
	text_layer_destroy(highscore_text);
}

void highscore_window_create(){
	highscore_window = window_create();
	window_set_click_config_provider(highscore_window, click_config_provider);
	window_set_window_handlers(highscore_window, (WindowHandlers) {
		.load = highscore_window_load,
		.unload = highscore_window_unload
	});
}

void highscore_window_destroy(){
	window_destroy(highscore_window);
}

Window *highscore_window_get_window(){
	return highscore_window;
}