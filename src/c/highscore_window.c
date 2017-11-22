#include <pebble.h>
#include <stdlib.h>
#include <string.h>

#include "highscore_window.h"
#include "initials_window.h"
#include "main.h"
#include "game.h"

#define HS_CNT_PLAYERS 90
#define HS_NAMES 100
#define HS_POINTS 110

Window *highscore_window;
TextLayer *highscore_title;
TextLayer *highscore_text;

char highscore_list_text[100];

// TODO: REMOVE DEBUG ONLY - write data to APP LOG
void DEBUG_ARRAY() {
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "current content of the array 'highscores'  " );
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "--------------------------- " );
	for (int i = 0; i <= cntPlayersInHighscoreList; i++) {
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "%d.  name %s, points: %d", i, highscores[i].name, highscores[i].points);
	}
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "--------------------------- " );
}

void import_highscores() {
	cntPlayersInHighscoreList = -1; 
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "'import_highscores': cntPlayersInHighscoreList should be --1 is = %d", cntPlayersInHighscoreList);
	if (!persist_exists(HS_CNT_PLAYERS)) {
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "'import_highscores': no high scores in persist storage found");
		return;
	}
	cntPlayersInHighscoreList = persist_read_int(HS_CNT_PLAYERS);
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "'import_highscores': value from persist_read_int is  cntPlayersInHighscoreList = %d", cntPlayersInHighscoreList);
	
	for (int i = 0; i <= cntPlayersInHighscoreList; i++) {
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "'import_highscores' reading player %d from storage ", i);
		if (!persist_exists(HS_POINTS + i) || !persist_exists(HS_NAMES + i)) {
			// APP_LOG(APP_LOG_LEVEL_ERROR, "'import_highscores' Error: there should be points/names in the storage");
			//persist_write_int(HS_POINTS + i, 9);
			//persist_write_data(HS_NAMES + i, "dum", 4);
			//highscores[i].points = 0;
			//highscores[i].name[0] = 'd'; highscores[i].name[1] = '\0';
		} else {
			highscores[i].points = persist_read_int(HS_POINTS + i);
			int result = persist_read_data(HS_NAMES + i, &highscores[i].name, 4);
			// APP_LOG(APP_LOG_LEVEL_DEBUG, "'import_highscores'  player %d name = %s, points = %d", i,highscores[i].name, highscores[i].points);
			if (result != 4) {
				// APP_LOG(APP_LOG_LEVEL_ERROR, "'import_highscores'  Error READING from storage");
			}
		}
	}
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "'import_highscores'  array highscores");
	DEBUG_ARRAY();
}



void create_single_highscore_string() {
	// create a single string from the array of players
	highscore_list_text[0] = ' ';
	highscore_list_text[1] = '\0';

	// APP_LOG(APP_LOG_LEVEL_DEBUG, "'create_single_highscore_string'  array highscores");
	DEBUG_ARRAY();

	char seperator[3] = ": ";
	char points[12];
	char newLine[2] = "\n";
	char position[2];

	// APP_LOG(APP_LOG_LEVEL_DEBUG, "create_single_highscore_string() (int)cntPlayersInHighscoreList = %d ", (int)cntPlayersInHighscoreList);
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "create_single_highscore_string() (int)cntPlayersInHighscoreList = %i ", (int)cntPlayersInHighscoreList);

	for (int i = 0; i <= cntPlayersInHighscoreList; i++) {
		// copy name of the player into "highscore_list_text"
		snprintf(position, sizeof(position), "%d", i+1);
		strcat(highscore_list_text, position);
		strcat(highscore_list_text, seperator);
		strcat(highscore_list_text, highscores[i].name);
		strcat(highscore_list_text, seperator);

		// APP_LOG(APP_LOG_LEVEL_DEBUG, "'create_single_highscore_string' 1/2 full highscore_list_text string:   %s", highscore_list_text);
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "'create_single_highscore_string' 1/2 highscores[i].nameg:   %s", highscores[i].name);
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "'create_single_highscore_string' 1/2 seperator:   %s", seperator);

		// copy points of the player into "highscore_list_text"
		snprintf(points, sizeof(points), "%d", highscores[i].points);	
		strcat(highscore_list_text, points);
		
		// APP_LOG(APP_LOG_LEVEL_DEBUG, "'create_single_highscore_string' 2/2   full highscore_list_text string:   %s", highscore_list_text);

		// add new line to  "highscore_list_text"		 
		strcat(highscore_list_text, newLine);
	}
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "'create_single_highscore_string' END full highscore_list_text string:   %s", highscore_list_text);
}


void write_highscores() {
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "'write_highscores'  array highscores");
	DEBUG_ARRAY();
	// save, how many players are in the high score list 	
	persist_write_int(HS_CNT_PLAYERS, cntPlayersInHighscoreList);

	for (int i = 0; i <= cntPlayersInHighscoreList; i++) {
		persist_write_int(HS_POINTS + i, highscores[i].points);
		persist_write_data(HS_NAMES + i, &highscores[i].name, 4);
	}
}

/*** Click any button to exit program ***/
// static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
// 	// highscore_window_destroy();
// 	app_timer_register(10, launch_game_window, NULL);
// }

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	// highscore_window_destroy();
	app_timer_register(10, launch_game_window, NULL);
}

// static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
// 	// highscore_window_destroy();
// 	app_timer_register(10, launch_game_window, NULL);
// }

// static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
// 	// highscore_window_destroy();
// 	app_timer_register(10, launch_game_window, NULL);
// }

static void click_config_provider(void *context) {
  //window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  //window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  //window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
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
	
	// APP_LOG(APP_LOG_LEVEL_DEBUG, "highscore_window_load()  calling create_single_highscore_string()");
	// insert_new_score();
	create_single_highscore_string();
	write_highscores();
	
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
