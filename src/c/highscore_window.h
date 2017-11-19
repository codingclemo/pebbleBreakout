#pragma once
/*
typedef struct player_node* node_ptr;
typedef struct player_node {
	char name[3];
	int points;
	node_ptr next;
} player_node;

typedef player_node* list_ptr;*/
//list_ptr highscore_list = NULL;

//extern list_ptr highscore_list;

//node_ptr create_node(char *p_name, int points);
//void insert_node(list_ptr l, player_node* n);

void highscore_window_create();
void highscore_window_destroy();
Window *highscore_window_get_window();