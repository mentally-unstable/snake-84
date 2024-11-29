// hello
// project snake
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <sys/lcd.h>
#include <sys/timers.h>
#include <graphx.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SCALE 10
#define WIDTH (LCD_WIDTH/SCALE)
#define HEIGHT (LCD_HEIGHT/SCALE)
#define MAX_LEN 20

enum {
    WHITE,
    BLACK,
    RED,
    GREEN,
    YELLOW,

    NORTH,
    SOUTH,
    EAST,
    WEST,
};

int darkmode = 1;
int dead = 0;
int paused = 0;

typedef struct {
    int x;
    int y;
    int vel;
    int nextv;
} seg_t;

seg_t snake[MAX_LEN];
int tail;

gfx_point_t edible;
int edible_safe = 1;
int eaten = 1;

void init(void);
void fill_screen(uint16_t color);
void fill_cell(int x, int y);
void snake_grow(void);
void process_key(uint8_t key);
void draw_snake(void);
void update_snake(void);
void draw_edible(void);
void generate_edible(void);

int main(void) {
    srand(time(0));
    init();
    gfx_Begin();
    gfx_SetDrawBuffer();

    uint16_t *pal = gfx_palette;
    pal[WHITE] = gfx_RGBTo1555(255, 255, 255);
    pal[BLACK] = gfx_RGBTo1555(0, 0, 0);
    pal[RED] = gfx_RGBTo1555(255, 0, 0);
    pal[GREEN] = gfx_RGBTo1555(0, 255, 0);
    pal[YELLOW] = gfx_RGBTo1555(255, 255, 0);

    uint8_t key;
    while ((key = os_GetCSC()) != sk_2nd)
    {
        if (dead) {
            while (!os_GetCSC());
            break;
        }

        process_key(key);
        if (paused) continue;

        gfx_SetColor(WHITE);
        if (darkmode) gfx_SetColor(BLACK);
        gfx_FillRectangle_NoClip(0, 0, LCD_WIDTH, LCD_HEIGHT);

        draw_snake();
        update_snake();

        if (eaten) generate_edible();
        draw_edible();

        gfx_BlitBuffer();
        delay(10);
    }


    gfx_End();
    return 0;
}

void draw_snake(void) {
    gfx_SetColor(GREEN);
    if (darkmode) gfx_SetColor(BLACK);

    for (int i = 0; i <= tail; i++) {
        fill_cell(snake[i].x, snake[i].y);
    }
}

void update_snake(void) {
    for (int i = 0; i <= tail; i++) {
        if (snake[i].y <= 0) snake[i].y = HEIGHT+snake[i].y;
        if (snake[i].y >= HEIGHT) snake[i].y -= HEIGHT;
        if (snake[i].x <= 0) snake[i].x = WIDTH+snake[i].x;
        if (snake[i].x >= WIDTH) snake[i].x -= WIDTH;

        switch (snake[i].vel) {
            case NORTH:
                snake[i].y--;
            break;
            case SOUTH:
                snake[i].y++;
            break;
            case WEST:
                snake[i].x--;
            break;
            case EAST:
                snake[i].x++;
            break;
        }

        if (i == 0) continue;
        snake[i].nextv = snake[i-1].vel;
    }

    for (int i = 1; i <= tail; i++) {
        snake[i].vel = snake[i].nextv;

        if (snake[i].x == snake[0].x &&
            snake[i].y == snake[0].y)
        {
            dead = 1;
        }

    }

    if (snake[0].x == edible.x &&
        snake[0].y == edible.y)
    {
        eaten = 1;
        snake_grow();
    }
}

void draw_edible(void) {
    gfx_SetColor(RED);

    if (!edible_safe) {
        gfx_SetColor(rand()%2 ? RED : YELLOW);
    }

    fill_cell(edible.x, edible.y);
}

void generate_edible(void) {
    // if ((rand()%25)%5 == 0) edible_safe = 0;

    edible = (gfx_point_t) {
        rand()%WIDTH,
        rand()%HEIGHT,
    };

    eaten = 0;
}

void process_key(uint8_t key) {
    switch (key) {
        case sk_Mode:
            darkmode = darkmode ? 0 : 1;
        break;

        case sk_Enter:
            paused = paused ? 0 : 1;
        break;

        case sk_Up:
        case sk_8:
            snake[0].vel = NORTH;
            break;
        case sk_Down:
        case sk_2:
            snake[0].vel = SOUTH;
            break;
        case sk_Left:
        case sk_4:
            snake[0].vel = WEST;
            break;
        case sk_Right:
        case sk_6:
            snake[0].vel = EAST;
            break;
        default:
            break;
    }
}

void init(void) {
    tail = 0;
    snake[0] = (seg_t) {
        .x = (LCD_WIDTH/SCALE)/2,
        .y = (LCD_HEIGHT/SCALE)/2,
        .vel = WEST,
        .nextv = WEST,
    };

    snake_grow();
}

void snake_grow(void) {
    if (tail >= MAX_LEN)
        return;

    switch (snake[tail].vel) {
        case NORTH:
            snake[tail+1] = (seg_t) {
                .x = snake[tail].x,
                .y = snake[tail].y+1,
                .vel = snake[tail].vel,
                .nextv = snake[tail].nextv,
            };
            ++tail;
        break;
        case SOUTH:
            snake[tail+1] = (seg_t) {
                .x = snake[tail].x,
                .y = snake[tail].y-1,
                .vel = snake[tail].vel,
                .nextv = snake[tail].nextv,
            };
            ++tail;
        break;
        case EAST:
            snake[tail+1] = (seg_t) {
                .x = snake[tail].x-1,
                .y = snake[tail].y,
                .vel = snake[tail].vel,
                .nextv = snake[tail].nextv,
            };
            ++tail;
        break;
        case WEST:
            snake[tail+1] = (seg_t) {
                .x = snake[tail].x+1,
                .y = snake[tail].y,
                .vel = snake[tail].vel,
                .nextv = snake[tail].nextv,
            };
            ++tail;
        break;
    }
}

void fill_screen(uint16_t color) {
    memset(lcd_Ram, color, LCD_SIZE);
}

void fill_cell(int x, int y) {
    gfx_FillRectangle(x*SCALE, y*SCALE, SCALE, SCALE);
}
