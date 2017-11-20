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

void insert_new_score(char *name, int points) {
	cntPlayersInHighscoreList++;
	if (cntPlayersInHighscoreList > HIGHSCORE_LENGTH-1) {
		cntPlayersInHighscoreList = HIGHSCORE_LENGTH-1; 
	}

	// find the index in the array, where we want to insert the new player/score
	int idx = 0;
	while (highscores[idx].points > points && idx < HIGHSCORE_LENGTH && idx <= cntPlayersInHighscoreList)  {
		idx++; 
	}
	APP_LOG(APP_LOG_LEVEL_DEBUG, "'insert_new_score' idx= %d     cntPlayersInHighscoreList = %d   ", idx, cntPlayersInHighscoreList);

	if (idx >= HIGHSCORE_LENGTH) {
		// we only have room for 5 highscores - we are done here (play better next time buddy!)
		return; 
	}
	// if the new score is the last entry in the array, we dont have to move
	// the other entries - otherwise: move all entries down by 1
	for (int i = HIGHSCORE_LENGTH-2; i >= idx; i--) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "'insert_new_score' moving entry i = %d down to i+1 = %d  ", i, i+1);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "'insert_new_score' moving name %s down ", highscores[i].name);

		highscores[i+1].points = highscores[i].points;
		for (int m = 0; m < 3; m++) {
			highscores[i+1].name[m] = highscores[i].name[m];
		}
		// better safe than sorry
		highscores[i].name[4] = '\0';
		highscores[i+1].name[4] = '\0';
	}
	// insert new entry for highscore 
	highscores[idx].points = points;
	for (int m = 0; m < 3; m++) {
		highscores[idx].name[m] = name[m];
	}
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (charPosition == 2) {
		/* adapt display */
		text_layer_set_text(initials_title, "Name saved!");
		text_layer_set_text_color(user_initials, GColorRed);
		text_layer_destroy(indicator);
		
		/* save the name and score in Highscore list */
		// TODO remove this line, when the game is finally implemented 
		current_player_points = 12 + cntPlayersInHighscoreList;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "'select_click_handler' insert new highscore with  username:   %s", username);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "'select_click_handler' and points =   %d", current_player_points);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "'select_click_handler' cntPlayersInHighscoreList =   %d", cntPlayersInHighscoreList);

		insert_new_score(username, current_player_points);

		app_timer_register(500, launch_highscore_window, NULL);

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