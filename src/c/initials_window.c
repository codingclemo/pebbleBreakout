#include <pebble.h>
#include <stdio.h>
#include <string.h>
#include "initials_window.h"

Window *initials_window;
TextLayer *initials_title;
TextLayer *user_initials;

char username[] = "aaa";
int charPosition = 0;

/*** Button handling ***/
static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
	  if (charPosition == 0) {
		/* go back to menu or do nothing */
	} else {
		charPosition --;
	}
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (charPosition == 2) {
		/* save the name and score in Highscore list */
		text_layer_set_text(user_initials, "Name saved!");
	} else {
		charPosition ++;
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
	initials_title = text_layer_create(GRect(0,0,144,70));
	text_layer_set_text(initials_title, "Enter initials:");
	text_layer_set_text_alignment(initials_title, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(initials_title));
	
	/* Display user initials */
	user_initials = text_layer_create(GRect(0,70,144,140));
	text_layer_set_text(user_initials, username);
	text_layer_set_text_alignment(user_initials, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(user_initials));
}

void initials_window_unload(Window *window){
	
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