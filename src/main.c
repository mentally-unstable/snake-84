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
#define MAX_LEN (LCD_SIZE/SCALE)

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
int speed = 50;

struct Snake_t {
    uint8_t x;
    uint8_t y;
    int vel;
} snake = {
    WIDTH/2,
    HEIGHT/2,
    WEST
};

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t color;
} body_t;
body_t body[MAX_LEN];
int len = 0;

gfx_point_t edible;
int eaten = 1;

void fill_screen(uint16_t color) {
    memset(lcd_Ram, color, LCD_SIZE);
}

void fill_cell(int x, int y) {
    gfx_FillRectangle((x*SCALE)+1, (y*SCALE)+1, SCALE-2, SCALE-2);
}

void process_key(uint8_t key);
void draw_snake(void);
void update_snake(void);
void draw_edible(void);
void generate_edible(void);

int main(void) {
    srand(time(0));
    gfx_Begin();
    gfx_SetDrawBuffer();

    body[0] = (body_t) {snake.x, snake.y, GREEN};
    body[1] = (body_t) {snake.x, snake.y-1, GREEN};
    body[2] = (body_t) {snake.x, snake.y-2, GREEN};
    body[3] = (body_t) {snake.x, snake.y-3, GREEN};
    len += 4;

    uint16_t *pal = gfx_palette;
    pal[WHITE] = gfx_RGBTo1555(255, 255, 255);
    pal[BLACK] = gfx_RGBTo1555(0, 0, 0);
    pal[RED] = gfx_RGBTo1555(255, 0, 0);
    pal[GREEN] = gfx_RGBTo1555(0, 255, 0);
    pal[YELLOW] = gfx_RGBTo1555(255, 255, 0);

    // welcome
    draw_welcome();
    while (!os_GetCSC());

    uint8_t key;
    while ((key = os_GetCSC()) != sk_2nd)
    {
        if (dead) {
            while (!os_GetCSC());
            break;
        }

        process_key(key);
        if (paused) continue;

        // background
        gfx_SetColor(WHITE);
        if (darkmode) gfx_SetColor(BLACK);
        gfx_FillRectangle_NoClip(0, 0, LCD_WIDTH, LCD_HEIGHT);

        draw_snake();
        update_snake();
        if (eaten) generate_edible();
        draw_edible();

        gfx_BlitBuffer();
        delay(speed);
    }


    gfx_End();
    return 0;
}

void draw_snake(void) {
    gfx_SetColor(BLACK);
    if (darkmode) gfx_SetColor(GREEN);

    for (int i = 0; i < len; i++) {
        fill_cell(body[i].x, body[i].y);
    }
}

void update_snake(void) {
    if (snake.y < 0) dead = 1;
    if (snake.y > HEIGHT) dead = 1;
    if (snake.x < 0) dead = 1;
    if (snake.x > WIDTH) dead = 1;
    if (dead) return;

    // add snake head
    body_t new = (body_t) {snake.x, snake.y, GREEN};
    body_t prev;
    for (int i = 0; i <= len+1; i++) {
        prev = body[i];
        body[i] = new;
        new = prev;
    }
    len++;

    // remove end of snake
    if (snake.x == edible.x &&
        snake.y == edible.y)
    {
        eaten = 1;
        speed -= 1;
        if (speed < 10) speed = 10;
    } else {
        len--;
    }

    switch (snake.vel) {
        case NORTH:
            snake.y--;
        break;
        case SOUTH:
            snake.y++;
        break;
        case WEST:
            snake.x--;
        break;
        case EAST:
            snake.x++;
        break;
    }

    for (int i = 1; i <= len; i++) {
        if (body[i].x == snake.x &&
            body[i].y == snake.y)
        {
            dead = 1;
        }
    }
}

void draw_edible(void) {
    gfx_SetColor(RED);
    fill_cell(edible.x, edible.y);
}

void generate_edible(void) {
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
            snake.vel = NORTH;
            break;
        case sk_Down:
        case sk_2:
            snake.vel = SOUTH;
            break;
        case sk_Left:
        case sk_4:
            snake.vel = WEST;
            break;
        case sk_Right:
        case sk_6:
            snake.vel = EAST;
            break;
        default:
            break;
    }
}

void draw_welcome(void) {
    ;
}
