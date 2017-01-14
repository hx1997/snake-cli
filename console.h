#ifndef CLI_SNAKE_CONSOLE_H
#define CLI_SNAKE_CONSOLE_H

#endif //CLI_SNAKE_CONSOLE_H

#include <conio.h>
#include <windows.h>

HANDLE hStdout;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

extern int resize_console(int height, int width);
extern void hide_cursor();
extern int set_pos(int x, int y);
extern int set_textcolor(int color);