#include <stdio.h>
#include <stdlib.h>
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
            if (i + k >= 0 
                && i + k < h 
                && j + l >= 0 
                && j + l < w 
                && screen[i + k][j + l] == '*')
                n++;
        }
    }

    if (screen[i][j] == '*')
        return n - 1;

    return n;
}

void resize_screen(void) {
    screen = (char **) realloc(screen, h * sizeof(char *));
    for (int i = 0; i < h; i++)
        screen[i] = (char *) realloc(screen[i], w * sizeof(char));

    return;
}

void print_screen(void) {
    // print first line
    for (int i = 0; i < w + 2; i+=2)
        printf("+ ");
    printf("\n");

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            // is a border
            if (j == 0)
                printf("%c", '+');
            if (i == cursor.y && j == cursor.x)
                printf("%c", '@');
            else 
                printf("%c", screen[i][j] == '\0' ? ' ' : screen[i][j]);
                //printf("%c", screen[i][j] == '\0' ? ' ' : (char) nigga(i, j) + '0');
                //printf("%c", screen[i][j] == '\0' ? nigga(i, j) + '0' : '*');
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
    else if (input == 'w' && cursor.y > 0)
        cursor.y--;
    else if (input == 's' && cursor.y < h - 1)
        cursor.y++;
    else if (input == 'a' && cursor.x > 0)
        cursor.x--;
    else if (input == 'd' && cursor.x < w - 1)
        cursor.x++;
    else if (input == ' ')
        screen[cursor.y][cursor.x] = '*'; 
    else if (input == 'r')
        screen[cursor.y][cursor.x] = '\0'; 
    else if (input == 'p') {
        game_handler();
        cursor.x = -1;
        cursor.y = -1;
    }

    return;
}

void game_handler() {
    // build a new matrix, do not modify the current one
    // use realloc
    char **temp = NULL;
    temp = (char **) realloc(temp, h * sizeof(char *));
    for (int i = 0; i < h; i++)
        temp[i] = (char *) realloc(temp[i], w * sizeof(char));


    for (int j = 0; j < w; j++) {
        for (int i = 0; i < h; i++) {
            temp[i][j] = '\0'; // initialize the matrix
            if (screen[i][j] == '\0' && nigga(i, j) == 3)
                temp[i][j] = '*';
            else if (screen[i][j] == '*' && nigga(i, j) <= 1) 
                temp[i][j] = '\0';
            else if (screen[i][j] == '*' && nigga(i, j) >= 4) 
                temp[i][j] = '\0';
            else if (screen[i][j] == '*')
                temp[i][j] = '*'; 
        }
    }

    // copy the temp into the screen
    for (int j = 0; j < w; j++) {
        for (int i = 0; i < h; i++) 
            screen[i][j] = temp[i][j];
    }

    // free the temp matrix
    for (int i = 0; i < h; i++)
        free(temp[i]);
    // it does not work ???
    //free(temp);

    return;
}
