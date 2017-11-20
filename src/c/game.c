#include <pebble.h>
#include "game.h"

#define DELTA_Y_MOVING_THINGI  12


static Window *game_window;
static Layer *canvas_layer;

static const uint32_t tick_ms = 300;
static AppTimer *timer = NULL;

static void refresh(void *data);


typedef struct MovingThingi {
    int32_t m_x, m_y; 
    int32_t w_2, h_2; 
    GColor color;
} MovingThingi;

typedef struct Ball {
    int32_t m_x, m_y; 
    int32_t r; 
    GColor color;
    int32_t width; 
} Ball;

MovingThingi movingThingi;
Ball ball; 

// static void start() {
//   timer = app_timer_register(tick_ms, refresh, NULL);
//   layer_mark_dirty(canvas_layer);
//   // snprintf(score_text, sizeof(score_text), "%11llu", score);
//   // text_layer_set_text(text_layer, score_text);
// }

static void refresh(void *data) {
    // check if game is over?
//   if (game_is_over || is_game_over()) {
//     game_over();
//     return;
//   }
  
  timer = app_timer_register(tick_ms, refresh, NULL);
  
  // redraw the canvas
  layer_mark_dirty(canvas_layer);
}

static void draw_moving_thingi(Layer *layer, GContext *ctx) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "draw_moving_thingi");
    APP_LOG(APP_LOG_LEVEL_DEBUG, "m_x = %d, m_y = %d, w_2 = %d, h_2 = %d", movingThingi.m_x, movingThingi.m_y, movingThingi.w_2, movingThingi.h_2 );

    graphics_context_set_fill_color(ctx, movingThingi.color);
    GRect rect_bounds = GRect(movingThingi.m_x - movingThingi.w_2, movingThingi.m_y - movingThingi.h_2, 
                              2* movingThingi.w_2, 2 * movingThingi.h_2);
    
    graphics_fill_rect(ctx, rect_bounds, 3, GCornersAll);
}

static void draw_ball(Layer *layer, GContext *ctx) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "draw_ball");
    APP_LOG(APP_LOG_LEVEL_DEBUG, "m_x = %d, m_y = %d, r = %d", ball.m_x, ball.m_y, ball.r);

    graphics_context_set_fill_color(ctx, ball.color);
    graphics_fill_circle(ctx, GPoint(ball.m_x, ball.m_y), ball.r);
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "canvas_update_proc");


    // GRect bounds = layer_get_bounds(layer);
  
    // graphics_context_set_stroke_color(ctx, GColorRed);
    // graphics_context_set_stroke_width(ctx, 2);
    // graphics_draw_line(ctx, GPoint(0, 0), GPoint(100, 30)); 

    draw_moving_thingi(layer, ctx);
    draw_ball(layer, ctx);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "SELECT clicked!");

}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "UP clicked!");
    movingThingi.m_y -= DELTA_Y_MOVING_THINGI;
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DOWN clicked!");
    movingThingi.m_y += DELTA_Y_MOVING_THINGI;
    // layer_mark_dirty(canvas_layer);
}


static void game_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void game_window_load(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "'game_window_load'   ");

    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    canvas_layer = layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h));
    layer_set_update_proc(canvas_layer, canvas_update_proc);
    layer_add_child(window_layer, canvas_layer);

    // initialize our moving thingi which reflects the ball
    // 7 px from the right border
    // vertically centered
    movingThingi.w_2 = 4;
    movingThingi.h_2 = 18;
    movingThingi.m_x = bounds.size.w - 6 - movingThingi.w_2;
    movingThingi.m_y = bounds.size.h / 2;
    
    movingThingi.color = GColorRed;

    // initlaize our ball
    ball.r = 3; 
    ball.m_x = movingThingi.m_x - movingThingi.w_2 - ball.r; 
    ball.m_y = movingThingi.m_y; 
    ball.color = GColorGreen;

    timer = app_timer_register(tick_ms, refresh, NULL);
}

static void game_window_unload(Window *window) {
    if (timer != NULL) {
        app_timer_cancel(timer);
        timer = NULL;
    }
}

// static void init(void) {
//   game_window = window_create();
//   window_set_click_config_provider(game_window, click_config_provider);
//   window_set_window_handlers(game_window, (WindowHandlers) {
//     .load = window_load,
//     .unload = window_unload,
//   });
//   light_enable(true);
// }

// static void deinit(void) {
//   window_destroy(window);
//   light_enable(false);
// }

// int main(void) {
//   init();

//   APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

//   app_event_loop();
//   deinit();
// }



Window *game_window_get_window(){
	return game_window;
}

void game_window_destroy(){
	window_destroy(game_window);
}

void game_window_create() {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "'game_window_create'   ");



	game_window = window_create();
	window_set_click_config_provider(game_window, game_click_config_provider);
	window_set_window_handlers(game_window, (WindowHandlers) {
		.load = game_window_load,
		.unload = game_window_unload
	});
}

