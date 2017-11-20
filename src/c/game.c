#include <pebble.h>
#include "game.h"

#define DELTA_Y_MOVING_THINGI  24

#define MAX_BLOCKS 100

#define DELTA_X_STEEP  2
#define DELTA_Y_STEEP  5

#define DELTA_X_FLAT  3
#define DELTA_Y_FLAT  4

static Window *game_window;
static Layer *canvas_layer;
static TextLayer *info_layer;

static const uint32_t tick_ms = 100;
static AppTimer *timer = NULL;

static void refresh(void *data);

char score_text[100];
static uint64_t score; 

typedef enum GameStatus {
    NotStarted, 
    Running, 
    Finished,
    GameWon
} GameStatus;

typedef struct Direction {
    int dx; 
    int dy; 
} Direction;

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
    Direction direction; 
} Ball;

typedef struct Block {
    int32_t x, y; 
    int32_t w, h;
    GColor color;
    bool visible;
} Block; 


GameStatus game_status; 
MovingThingi moving_thingi;
Ball ball; 

GRect board; 

Block blocks[MAX_BLOCKS];
int cnt_blocks; 

static void updateBallPosition() {
    ball.m_x += ball.direction.dx; 
    ball.m_y += ball.direction.dy; 

    if (ball.m_x < board.origin.x) {
        ball.m_x = board.origin.x;
        ball.direction.dx *= -1; 
    }
    if (ball.m_y < board.origin.y) {
        ball.m_y = board.origin.y;
        ball.direction.dy *= -1; 
    }
    if (ball.m_y > (board.origin.y+ board.size.h)) {
        ball.m_y = board.origin.y + board.size.h;
        ball.direction.dy *= -1; 
    }

    // check if ball.m_y > board.size.y is not necessary - thats done in "checkBallThingiCollision"
    
}

static void checkBallThingiCollision() {
    // check if the user missed the ball
    if (ball.m_x > (moving_thingi.m_x - moving_thingi.w_2)) {
        game_status = Finished; 
    } else {
        // is the ball touching the "thingi" ?
        if ((ball.m_x >= (moving_thingi.m_x - moving_thingi.w_2 - ball.r)) &&
            (ball.m_y > (moving_thingi.m_y - 2 * moving_thingi.h_2)) &&
            (ball.m_y < (moving_thingi.m_y + 2 * moving_thingi.h_2)) &&
            ball.direction.dx > 0) {
            // simple reflection, just change the sign of dx

            // where hit the ball the thingi ?
            // there are 4 zones: left outer, left inner, right inner, right outer
            // and depending on the zone, the reflection angle is different
            if (ball.m_y >= (moving_thingi.m_y)) {
                // left zone
                if (ball.m_y >= (moving_thingi.m_y + moving_thingi.h_2 / 2)) {
                    // left outer zone -> "flat" angle
                    if (ball.direction.dy > 0) {
                        ball.direction.dx *= -1;
                    } else {
                        ball.direction.dx *= -1;
                        ball.direction.dy *= -1;
                    }
                } else {
                    // left inner  zone -> "steep" angle
                    if (ball.direction.dy > 0) {
                        ball.direction.dx *= -1;
                    } else {
                        ball.direction.dx *= -1;
                        ball.direction.dy *= -1;
                    }
                }
            } else {
                // right zone
                if (ball.m_y <= (moving_thingi.m_y - moving_thingi.h_2 / 2)) {
                    // left outer zone -> "flat" angle
                    if (ball.direction.dy > 0) {
                        ball.direction.dx *= -1;
                        ball.direction.dy *= -1;
                    } else {
                        ball.direction.dx *= -1;
                    }
                } else {
                    // right inner  zone -> "steep" angle
                    if (ball.direction.dy > 0) {
                        ball.direction.dx *= -1;
                        ball.direction.dy *= -1;
                    } else {
                        ball.direction.dx *= -1;
                    }
                }
            }
            //ball.direction.dx *= -1;
            score--;
        }
    }
}


static void checkBallBlockCollision() {
    for (int i = 0; i < cnt_blocks; i++) {
        if (blocks[i].visible) {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "draw_checkBallBlockCollision   checking collision with block %d", i);
            if ((ball.m_x <= (blocks[i].x + blocks[i].w + ball.r)) &&
                (ball.m_y > blocks[i].y) &&
                (ball.m_y < (blocks[i].y + blocks[i].h)) &&
                ball.direction.dx < 0) {
                
                // if the ball touches the block, then reflect  the ball
                // and hide the block
                ball.direction.dx *= -1;
                blocks[i].visible = false; 
            }
        }
    }
}

static void refresh(void *data) {
    // check if game is over?
    //   if (game_is_over || is_game_over()) {
    //     game_over();
    //     return;
    //   }
    timer = app_timer_register(tick_ms, refresh, NULL);
    if (game_status == Running) {
        updateBallPosition();
        checkBallThingiCollision();
        checkBallBlockCollision();

        // check if all blocks invisible -> Game Won!
        bool one_block_visible = false;
        int i = 0; 
        while (i < cnt_blocks && !one_block_visible) {
            if (blocks[i].visible) {
                one_block_visible = true; 
            }
            i++;
        } 
        if (!one_block_visible) {
            game_status = GameWon;
        }

        // if the game is running: show the points in the info layer
        snprintf(score_text, sizeof(score_text), "%11llu", score);
        text_layer_set_text(info_layer, score_text);
    }
    if (game_status == Finished) {
        strcpy(score_text, "Game Over!");
        text_layer_set_text(info_layer, score_text);
    }
    if (game_status == GameWon) {
        char tmp[100];
        snprintf(tmp, sizeof(tmp), "%3llu", score);
        strcpy(score_text, "You won! Pts: ");
        strcat(score_text, tmp);
        text_layer_set_text(info_layer, score_text);
    }
   // redraw the canvas
   layer_mark_dirty(canvas_layer);
}

static void draw_moving_thingi(Layer *layer, GContext *ctx) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "draw_moving_thingi");
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "m_x = %d, m_y = %d, w_2 = %d, h_2 = %d", moving_thingi.m_x, moving_thingi.m_y, moving_thingi.w_2, moving_thingi.h_2 );

    graphics_context_set_fill_color(ctx, moving_thingi.color);
    GRect rect_bounds = GRect(moving_thingi.m_x - moving_thingi.w_2, moving_thingi.m_y - moving_thingi.h_2, 
                              2* moving_thingi.w_2, 2 * moving_thingi.h_2);
    
    graphics_fill_rect(ctx, rect_bounds, 3, GCornersAll);
}

static void draw_ball(Layer *layer, GContext *ctx) {
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "draw_ball");
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "m_x = %d, m_y = %d, r = %d", ball.m_x, ball.m_y, ball.r);

    graphics_context_set_fill_color(ctx, ball.color);
    graphics_fill_circle(ctx, GPoint(ball.m_x, ball.m_y), ball.r);
}

static void draw_blocks(Layer *layer, GContext *ctx) {
    for (int i = 0; i < cnt_blocks; i++) {
        if (blocks[i].visible) {
            graphics_context_set_fill_color(ctx, blocks[i].color);
            GRect rect_bounds = GRect(blocks[i].x, blocks[i].y, blocks[i].w, blocks[i].h);
            graphics_fill_rect(ctx, rect_bounds, 0, GCornersAll);
        }
    }
}

static void canvas_update_proc(Layer *layer, GContext *ctx) {
    // APP_LOG(APP_LOG_LEVEL_DEBUG, "canvas_update_proc");

    draw_moving_thingi(layer, ctx);
    draw_ball(layer, ctx);
    draw_blocks(layer, ctx);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "SELECT clicked!");
    if (game_status == NotStarted) {
        game_status = Running;
        APP_LOG(APP_LOG_LEVEL_DEBUG, "SELECT clicked and game_status set to RUNINNG");
    }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "UP clicked!");
    if (game_status == Running) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "UP clicked   - game is running - moving thingi");
        moving_thingi.m_y -= DELTA_Y_MOVING_THINGI;

        if (moving_thingi.m_y - moving_thingi.h_2 <= 0) {
            moving_thingi.m_y = moving_thingi.h_2;
        }
    }
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "DOWN clicked!");
    
    // layer_mark_dirty(canvas_layer);
    if (game_status == Running) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "DOWN clicked   - game is running - moving thingi");
        moving_thingi.m_y += DELTA_Y_MOVING_THINGI;

        if (moving_thingi.m_y + moving_thingi.h_2 >= board.size.h) {
            moving_thingi.m_y = board.size.h - moving_thingi.h_2;
        }
    }
}

static void game_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void init_blocks() {
    // add 2 blocks for starters 
    int margin = 5; 
    int width = 10; 

    cnt_blocks = 4; 
    blocks[0].x = margin; 
    blocks[0].y = margin; 
    blocks[0].w = width; 
    blocks[0].h = board.size.h / 2 - 2*margin; 
    blocks[0].color = GColorArmyGreen;
    blocks[0].visible = true; 

    blocks[1].x = margin; 
    blocks[1].y = board.size.h / 2; 
    blocks[1].w = width; 
    blocks[1].h = board.size.h / 2  - 2*margin; 
    blocks[1].color = GColorFashionMagenta;
    blocks[1].visible = true; 

    blocks[2].x = 4*margin; 
    blocks[2].y = margin; 
    blocks[2].w = width; 
    blocks[2].h = board.size.h / 2 - 2*margin; 
    blocks[2].color = GColorIslamicGreen;
    blocks[2].visible = true; 

    blocks[3].x = 4*margin; 
    blocks[3].y = board.size.h / 2; 
    blocks[3].w = width; 
    blocks[3].h = board.size.h / 2  - 2*margin; 
    blocks[3].color = GColorCyan;
    blocks[3].visible = true; 
}

static void game_window_load(Window *window) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "'game_window_load'   ");

    int height_info_layer = 20;

    // our game baords
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    canvas_layer = layer_create(GRect(bounds.origin.x, bounds.origin.y, bounds.size.w, bounds.size.h - height_info_layer));
    layer_set_update_proc(canvas_layer, canvas_update_proc);
    layer_add_child(window_layer, canvas_layer);

    // info layer with points and status info
    info_layer = text_layer_create(GRect(bounds.origin.x, bounds.size.h - height_info_layer, 
                                         bounds.size.w, height_info_layer));
    text_layer_set_text(info_layer, "\"Select\" to Start!");
    text_layer_set_text_alignment(info_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(info_layer));

    // how big is the area where the ball can fly around
    board = layer_get_bounds(canvas_layer);
    
    // initialize our moving thingi which reflects the ball
    // 6 px from the right border
    // vertically centered

    moving_thingi.w_2 = 4;
    moving_thingi.h_2 = 28;
    moving_thingi.m_x = board.size.w - 6 - moving_thingi.w_2;
    moving_thingi.m_y = board.size.h / 2;
    
    moving_thingi.color = GColorRed;

    // initlaize our ball
    ball.r = 3; 
    ball.m_x = moving_thingi.m_x - moving_thingi.w_2 - ball.r; 
    ball.m_y = moving_thingi.m_y; 
    ball.color = GColorGreen;
    ball.direction.dx = -5; 
    ball.direction.dy = -3; 

    // game status: ist notStarted, start with select Button
    game_status = NotStarted; 

    // create some blocks we have to "hit"
    init_blocks();

    // score is 100 to start with
    score = 100; 

    // register timer
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

