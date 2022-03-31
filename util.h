#ifndef _UTIL_H_
#define _UTIL_H_

#define ALIVE_CHAR '0'
#define CURSOR_CHAR '@'
#define UP_KEY 'w'
#define LEFT_KEY 'a'
#define DOWN_KEY 's'
#define RIGHT_KEY 'd'
#define MIN_WIDTH 12
#define MIN_HEIGHT 12

typedef struct {
    int x;
    int y;
} cursor_t;

extern char **screen;
extern int w;
extern int h;
extern int game_started;
extern cursor_t cursor;

void resize_screen(void);
void print_screen(void);
void clear_screen(void);
char get_input(void);
void input_handler(char input);
void game_handler();

#endif