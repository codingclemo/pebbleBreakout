#include <pebble.h>
#include <stdlib.h>
#include <string.h>

#include "highscore_window.h"
#include "initials_window.h"

#define HIGHSCORE_LENGTH 5
#define HS_NAMES 101
#define HS_POINTS 111

char highscore_list_text[100];

Window *highscore_window;
TextLayer *highscore_title;
TextLayer *highscore_text;


char* names[HIGHSCORE_LENGTH];
char* points[HIGHSCORE_LENGTH];

void import_highscores(){
	char buffer[4];
	int stored_points;
	persist_write_string(103, "du3");
	persist_write_int(113, 7);
	
	for (int i = 1; i <= HIGHSCORE_LENGTH; i++){
		/* Get name */
		if (persist_exists(100+i)) {
			persist_read_string(100+i, buffer, sizeof(buffer));
		} else {
			buffer = "ddd";
		}
		names[i] = buffer;
		/* Get points */
		stored_points = persist_exists(110+i) ? persist_read_int(110+i) : 0;	
		//stored_points = persist_read_int(110+i);
		snprintf(buffer, 10, "%d", stored_points);
		points[i] = buffer;
	}
}


void update_highscores(){

		
}

void write_highscores(){
	/*persist_write_string(101, "du1");
	persist_write_string(102, "du2");
	persist_write_string(103, "du3");
	persist_write_string(104, "du4");
	persist_write_string(105, "du5");
	persist_write_int(111, 9);
	persist_write_int(112, 8);
	persist_write_int(113, 7);
	persist_write_int(114, 6);
	persist_write_int(115, 5);*/
	
	int text_length = sizeof(highscore_list_text);
	char text_buffer[10];
	
	//names[1] = "wtf";
	for (int i = 1; i <= HIGHSCORE_LENGTH; i++){
		snprintf(text_buffer, 10, "%d", i);
		strncat(highscore_list_text, text_buffer, text_length);
		strncat(highscore_list_text, ".  ", text_length);

		strncat(highscore_list_text, names[i], text_length);
		strncat(highscore_list_text, "  ", text_length);
		strncat(highscore_list_text, points[i], text_length);
		strncat(highscore_list_text, "\n", text_length);	
	}
	
}

void save_highscores(){
	//persist_write_int(NUM_DRINKS_PKEY, s_num_drinks);
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
	
	import_highscores();
	write_highscores();
	highscore_text = text_layer_create(GRect(0,50,144,160));
	//text_layer_set_text(highscore_text, "1. name  23\n2. name  9"); //"1. name  23\n2. name  9"
	text_layer_set_text(highscore_text, highscore_list_text); //"1. name  23\n2. name  9"
	text_layer_set_text_alignment(highscore_text, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(highscore_text));
}

void highscore_window_unload(Window *window){
	text_layer_destroy(highscore_title);
	text_layer_destroy(highscore_text);
}

void highscore_window_create(){
	highscore_window = window_create();
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