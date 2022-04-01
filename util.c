#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"

int w,
    h,
    game_started;
char **screen;
cursor_t cursor;

int nigga(int i, int j) {
    // returns the number of niggas around it
    int n = 0;

    for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++) {
            if (i + k >= 0 && 
                i + k < h && 
                j + l >= 0 && 
                j + l < w &&
                screen[i + k][j + l] == ALIVE_CHAR)
                n++;
        }
    }

    if (screen[i][j] == ALIVE_CHAR)
        return n - 1;

    return n;
}

void resize_screen(void) {
    screen = (char **) realloc(screen, h * sizeof(char *));
    for (int i = 0; i < h; i++)
        screen[i] = (char *) realloc(screen[i], (w / 2) * sizeof(char));

    return;
}

void print_screen(void) {
    // print first line
    for (int i = 0; i < w + 2; i+=2)
        printf("+ ");
    printf("\n");

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w/2; j++) {
            // is a border
            if (j == 0)
                printf("%c ", '+');
            if (i == cursor.y && j == cursor.x)
                printf("%c ", CURSOR_CHAR);
            else 
                printf("%c ", screen[i][j] == '\0' ? ' ' : screen[i][j]);
                //printf("%c", screen[i][j] == '\0' ? ' ' : (char) nigga(i, j) + '0');
                //printf("%c", screen[i][j] == '\0' ? nigga(i, j) + '0' : ALIVE_CHAR);
                //printf("%d", nigga(i, j));
                
        }
        printf("+\n");
    }

    // print last line
    for (int i = 0; i < w + 2; i+=2)
        printf("+ ");
    printf("\n");

    return;
}

void clear_screen(void) {
    printf("\033[H\033[J"); 

    return;
}

char get_input(void) {
    char input = 0;
    system("/bin/stty raw");
    input = getc(stdin);
    input = 'A' <= input && input <= 'Z' ? input - ('a' - 'A') : input; // to lower case
    printf("\r"); // clear the input text
    fflush(stdin); 
    system("/bin/stty cooked");

    return input;
}

void input_handler(char input) {
    if (input == 'q')
        exit(0);
    else if (input == UP_KEY && cursor.y > 0)
        cursor.y--;
    else if (input == DOWN_KEY && cursor.y < h - 1)
        cursor.y++;
    else if (input == LEFT_KEY && cursor.x > 0)
        cursor.x--;
    else if (input == RIGHT_KEY && cursor.x < w / 2 - 1)
        cursor.x++;
    else if (input == ADD_KEY)
        screen[cursor.y][cursor.x] = ALIVE_CHAR; 
    else if (input == REMOVE_KEY)
        screen[cursor.y][cursor.x] = '\0'; 
    else if (input == PLAY_KEY) {
        game_handler();
        cursor.x = -1;
        cursor.y = -1;
    }

    return;
}

void game_handler() {
    // build a new matrix, do not modify the current one
    // use realloc
    static char **temp = NULL;
    temp = (char **) realloc(temp, h * sizeof(char *));
    for (int i = 0; i < h; i++)
        temp[i] = (char *) realloc(temp[i], (w / 2) * sizeof(char));


    for (int j = 0; j < w/2; j++) {
        for (int i = 0; i < h; i++) {
            temp[i][j] = '\0'; // initialize the matrix
            if (screen[i][j] == '\0' && nigga(i, j) == 3)
                temp[i][j] = ALIVE_CHAR;
            else if (screen[i][j] == ALIVE_CHAR && nigga(i, j) <= 1) 
                temp[i][j] = '\0';
            else if (screen[i][j] == ALIVE_CHAR && nigga(i, j) >= 4) 
                temp[i][j] = '\0';
            else if (screen[i][j] == ALIVE_CHAR)
                temp[i][j] = ALIVE_CHAR; 
        }
    }

    // copy the temp into the screen
    for (int j = 0; j < w; j++) {
        for (int i = 0; i < h; i++) 
            screen[i][j] = temp[i][j];
    }

    return;
}

void tutorial() {
    clear_screen();
    printf("  ____    _    __  __ _____\n / ___|  / \\  |  \\/  | ____|\n| |  _  / _ \\ | |\\/| |  _|\n| |_| |/ ___ \\| |  | | |___\n \\____/_/   \\_\\_|  |_|_____|\n");
    sleep(1);
    printf("  ___  _____ \n / _ \\|  ___|\n| | | | |_\n| |_| |  _|\n \\___/|_|\n");
    sleep(1);
    printf(" _     ___ _____ _____ \n| |   |_ _|  ___| ____|\n| |    | || |_  |  _|\n| |___ | ||  _| | |___\n|_____|___|_|   |_____|\n");
    sleep(1);
    clear_screen();
    printf("Press '%c' to go up\n", UP_KEY);
    printf("Press '%c' to go left\n", LEFT_KEY);
    printf("Press '%c' to go down\n", DOWN_KEY);
    printf("Press '%c' to go right\n", RIGHT_KEY);
    printf("Press '%c' to add\n", ADD_KEY);
    printf("Press '%c' to go remove\n", REMOVE_KEY);
    printf("Press '%c' to go play\n", PLAY_KEY);
    sleep(3);

    return;
}
