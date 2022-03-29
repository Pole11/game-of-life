#ifndef _UTIL_H_
#define _UTIL_H_

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