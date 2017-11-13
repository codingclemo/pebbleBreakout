#include <pebble.h>
#include "splash_window.h"

Window *splash_window;
GBitmap *splash_Image;
BitmapLayer *splash_Image_Layer;
TextLayer *splash_Image_Title;


void splash_window_load(Window *window){
	Layer *window_layer = window_get_root_layer(window);
	
	splash_Image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SPLASH);
	splash_Image_Layer = bitmap_layer_create(GRect(0,0,144,120));
	bitmap_layer_set_bitmap(splash_Image_Layer, splash_Image);
	bitmap_layer_set_compositing_mode(splash_Image_Layer, GCompOpSet); //GCompOpSet for having transparent png splash images
	layer_add_child(window_layer, bitmap_layer_get_layer(splash_Image_Layer));
	
	splash_Image_Title = text_layer_create(GRect(0,120,144,50));
	text_layer_set_text(splash_Image_Title, "Get started!");
	text_layer_set_text_alignment(splash_Image_Title, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(splash_Image_Title));
}

void splash_window_unload(Window *window){
	gbitmap_destroy(splash_Image);
	bitmap_layer_destroy(splash_Image_Layer);
	text_layer_destroy(splash_Image_Title);
}

void splash_window_create(){
	splash_window = window_create();
	window_set_window_handlers(splash_window, (WindowHandlers) {
		.load = splash_window_load,
		.unload = splash_window_unload
	});
}

void splash_window_destroy(){
	window_destroy(splash_window);
}

Window *splash_window_get_window(){
	return splash_window;
}