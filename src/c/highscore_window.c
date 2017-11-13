#include <pebble.h>
#include "highscore_window.h"


Window *highscore_window;
TextLayer *highscore_title;
TextLayer *highscore_text;

/*** Datastructure Highscore list ***/
typedef struct player_node* node_ptr;
typedef struct player_node {
	char name[3];
	int points;
	node_ptr next;
} player_node;

typedef player_node* list_ptr;
list_ptr highscore_list = NULL;

/*** Datastructure functions ***/
player_node* create_node(char *p_name, int points){
	player_node *n;
	n = (player_node*)malloc(sizeof(player_node));
	for(int i = 0; i < 3; i++){
		n->name[i] = p_name[i];
	}
	n->points = points;
	n->next = NULL;
	return n;	
}

void destroy_node(player_node **n){
	if(*n == NULL)
		return;
	
	free(*n);
	*n = NULL;
}



/*** Window handling ***/
void highscore_window_load(Window *window){
	Layer *window_layer = window_get_root_layer(window);
	
	highscore_title = text_layer_create(GRect(0,10,144,50));
	text_layer_set_text(highscore_title, "HIGHSCORES");
	text_layer_set_text_alignment(highscore_title, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(highscore_title));
	
	highscore_text = text_layer_create(GRect(0,50,144,160));
	text_layer_set_text(highscore_text, "1. name  23\n2. name  9");
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