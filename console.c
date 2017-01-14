#include <stdio.h>

#include "console.h"

#define BUFSIZE 30

int resize_console(int height, int width) {
    int cx;
    char cmd[BUFSIZE] = {0};

    /* Resize console window */
    cx = snprintf(cmd, BUFSIZE, "mode con cols=%d lines=%d", width, height);

    if (cx < 0 || cx >= BUFSIZE) {
        perror("Argument too long");
        return 1;
    }

    system(cmd);

    return 0;
}

void hide_cursor() {
    CONSOLE_CURSOR_INFO info;

    info.dwSize = 100;
    info.bVisible = FALSE;

    SetConsoleCursorInfo(hStdout, &info);
}

int set_pos(int x, int y) {
    csbiInfo.dwCursorPosition.X = (SHORT) x;
    csbiInfo.dwCursorPosition.Y = (SHORT) y;

    return !SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition);
}

int set_textcolor(int color) {
    return !SetConsoleTextAttribute(hStdout, (WORD) color);
}
