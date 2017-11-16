#include <pebble.h>
#include "highscore_window.h"
#include "initials_window.h"

#define POINTS_PLAYERONE 3
#define POINTS_PLAYERTWO 2
#define POINTS_PLAYERTHREE 1
#define NAME_PLAYERONE "plo"
#define NAME_PLAYERTWO "plt"
#define NAME_PLAYERTHREE "pth"

char highscore_list_text[60];

Window *highscore_window;
TextLayer *highscore_title;
TextLayer *highscore_text;


/*** Datastructure Highscore list 
typedef struct player_node* node_ptr;
typedef struct player_node {
	char name[3];
	int points;
	node_ptr next;
} player_node;***/

typedef player_node* list_ptr;
//list_ptr highscore_list;

/*** Datastructure functions ***/
node_ptr create_node(char *p_name, int points){
	node_ptr n = NULL;
	//n = (player_node*)malloc(sizeof(player_node));
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

void insert_node(list_ptr l, player_node* n){
	list_ptr head;
	head = l;
	if (head == NULL){
		head = n;
	} 
	else {
		player_node *current, *previous;
		current = (player_node*)malloc(sizeof(player_node));
		previous = (player_node*)malloc(sizeof(player_node));
		current = head;
		previous = NULL;
		
		while(current != NULL && current->points >= n->points){
			previous = current;
			current = current->next;
		}
		if (current == head){
			n->next = head;
			head = n;	
		}
		else if(current == NULL){
			previous->next = n;
		}
		else {
			n->next = current;
			previous->next = n;
		}
	}
}

void write_highscores(list_ptr l){
	node_ptr n;
	
	int player_position = 1;
	int i = 0;

	n = l;
	while (n != NULL){
		//insert position
		highscore_list_text[i] = player_position;
		i++;
		player_position++;
		//insert spaces
		highscore_list_text[i] = ' ';
		i++;
		highscore_list_text[i] = ' ';
		i++;
		//insert name
		for (int t = 0; t < 3; t++){
			highscore_list_text[i] = n->name[t];
			i++;
		}
		//insert spaces
		highscore_list_text[i] = ' ';
		i++;
		highscore_list_text[i] = ' ';
		i++;
		//insert points
		// need to check for points > 10 and > 100 as well...maybe
		highscore_list_text[i] = n->points;
		i++;
		//insert return
		highscore_list_text[i] = '\n';
		i++;
		
		n = n->next;		
	}
	highscore_list_text[i] = '\0';
}

void import_highscores(list_ptr l){
	//num_drinks = persist_exists(NUM_DRINKS_PKEY) ? persist_read_int(NUM_DRINKS_PKEY) : NUM_DRINKS_DEFAULT;
}

void save_highscores(list_ptr l){
	//rsist_write_int(NUM_DRINKS_PKEY, s_num_drinks);
}
 
 
/*** Window handling ***/
void highscore_window_load(Window *window){
	Layer *window_layer = window_get_root_layer(window);
	
	highscore_title = text_layer_create(GRect(0,10,144,50));
	text_layer_set_text(highscore_title, "HIGHSCORES");
	text_layer_set_text_alignment(highscore_title, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(highscore_title));
	
	highscore_text = text_layer_create(GRect(0,50,144,160));
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