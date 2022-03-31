#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "util.h"

int w = 0,
    h = 0,
    game_started = 0;
char **screen = NULL;
cursor_t cursor;

int main(void) {
    int prec_w = 0,
        prec_h = 0,
        input = 0;

    cursor.x = MIN_WIDTH / 2;
    cursor.y = MIN_HEIGHT / 2;

    struct winsize win;

    for (;;) {
        // get the size of the screen
        ioctl(1, TIOCGWINSZ, &win);
        w = (win.ws_col % 2 == 0 ? win.ws_col - 1 : win.ws_col - 2) - 2;
        h = (win.ws_row % 2 == 0 ? win.ws_row : win.ws_row - 1) - 4;

        if (w < MIN_WIDTH) {
            printf("Error: the width is too small :\\\n");
            return -1;
        } else if (h < MIN_HEIGHT) {
            printf("Error: the height is too small :\\\n");
            return -1;
        }

        // check if the size of the screen changed
        if (prec_h != h || prec_w != w)
            resize_screen();

        // clear the screen
        clear_screen();
        print_screen();
                
        // get and handle the input
        input = get_input();
        input_handler(input);

        // save the size of the screen for the next iteration
        prec_w = w;
        prec_h = h;
    }

    return 0;
}