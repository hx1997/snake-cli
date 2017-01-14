#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "console.h"

#define LINES	20
#define COLS    40
#define SPEED   60

#define POINT_X(n) n % COLS
#define POINT_Y(n) n / COLS

struct point {
    int x;
    int y;
};
struct point food;                  // Stores coordinates for food

struct node {
    int x;
    int y;
    struct node *prev;
    struct node *next;
};                                  // Doubly linked list
struct node snake;                  // Stores coordinates for snake body, first node for the head
struct node *tail;

enum direction {
    LEFT, RIGHT, UP, DOWN
};

int snake_direct = RIGHT;           // The direction toward which the snake is moving
int snake_len = 4;                  // The length of snake body
int pause = 0;                      // 0 = playing; 1 = pause

void init() ;
void repaint() ;
void move() ;
void new_food() ;
int is_dead() ;
void game_won() ;
void game_over() ;
void add_tail() ;
void cleanup() ;

int main(void) {
    if (resize_console(LINES, COLS)) return 1;

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    hide_cursor();
    init();
    repaint();

    /* Game begins */
    while(1) {
        if (_kbhit()) {
            int ch = _getch();

            if (ch == ' ' && snake.next)    // Pause on space keypress if game isn't over
                pause = !pause;
            if (ch == 27)                   // Exit on ESC keypress
                break;

            if (!pause) {
                if (ch == 72) {
                    if (snake_direct != UP && snake_direct != DOWN) snake_direct = UP;
                } else if (ch == 75) {
                    if (snake_direct != LEFT && snake_direct != RIGHT) snake_direct = LEFT;
                } else if (ch == 80) {
                    if (snake_direct != UP && snake_direct != DOWN) snake_direct = DOWN;
                } else if (ch == 77) {
                    if (snake_direct != LEFT && snake_direct != RIGHT) snake_direct = RIGHT;
                } else {
                    move();
                }
            }
        } else {
            if (!pause) move();
        }

        Sleep(SPEED);                       // This controls the speed of the snake
    }

    return 0;
}

void init() {
    int i, j;
    struct node *ptr_prev = NULL;
    struct node *ptr_this = NULL;

    i = LINES;
    j = COLS;

    /* Position the snake halfway down the left side of the window */
    snake.x = j / 3 + 1;
    snake.y = i / 2;
    snake.prev = NULL;
    snake.next = malloc(sizeof(snake));
    ptr_prev = &snake;
    ptr_this = snake.next;

    /* Now we initialize each node (except the head) in the linked list */
    for (int k = 1; k < snake_len; k++) {
        ptr_this->x = j / 3 - (k - 1);
        ptr_this->y = i / 2;

        ptr_this->prev = ptr_prev;
        ptr_prev = ptr_this;

        ptr_this->next = malloc(sizeof(snake));
        ptr_this = ptr_this->next;
    }
    free(ptr_prev->next);
    ptr_prev->next = NULL;

    tail = ptr_prev;

    /* Initialize random seed and generate food at a random position */
    srand((unsigned)time(NULL));
    new_food();
}

void repaint() {
    int i;
    struct node *ptr_this = snake.next;

    system("cls");

    /* Draw the food */
    set_pos(food.x, food.y);
    putchar('*');

    /* Draw snake head */
    set_pos(snake.x, snake.y);
    putchar('@');

    /* Draw the body */
    for (i = 1; i < snake_len; i++) {
        set_pos(ptr_this->x, ptr_this->y);
        putchar('o');

        ptr_this = ptr_this->next;
    }
}

void move() {
    struct node *ptr_this = tail;

    /* Move the body forward */
    for (int i = snake_len - 1; i > 0; i--) {
        ptr_this->x = ptr_this->prev->x;
        ptr_this->y = ptr_this->prev->y;

        ptr_this = ptr_this->prev;
    }

    /* Move the head */
    switch (snake_direct) {
        case LEFT:
            snake.x--;
            break;
        case RIGHT:
            snake.x++;
            break;
        case UP:
            snake.y--;
            break;
        case DOWN:
            snake.y++;
            break;
        default:
            break;
    }

    if (!is_dead()) {
        repaint();
    } else {
        game_over();
    }

    /* Food eaten? */
    if (snake.x == food.x && snake.y == food.y) {
        add_tail();

        if (snake_len != LINES * COLS)
            new_food();
        else
            game_won();
    }
}

void add_tail() {
    snake_len++;

    /* Insert a node at the end of the linked list */
    tail->next = malloc(sizeof(snake));
    tail->next->prev = tail;
    tail->next->next = NULL;
    tail = tail->next;
}

void new_food() {
    int random;
    struct node *ptr_this = &snake;

    random = rand() % (LINES * COLS);

    food.x = POINT_X(random);
    food.y = POINT_Y(random);

    /* Make sure the location generated for the food is free */
    while (ptr_this) {
        if (food.x == ptr_this->x && food.y == ptr_this->y) new_food();
        ptr_this = ptr_this->next;
    }
}

int is_dead() {
    struct node *ptr_this = snake.next;

    /* Hit the snake itself? */
    for (int i = 1; i < snake_len; i++) {
        if (ptr_this->x == snake.x && ptr_this->y == snake.y)
            return 1;

        ptr_this = ptr_this->next;
    }

    /* Hit the border? */
    if (snake.x >= COLS || snake.x < 0)
        return 1;
    else if (snake.y >= LINES || snake.y < 0)
        return 1;
    else
        return 0;
}

void game_won() {
    pause = 1;
    set_pos(COLS / 2, LINES / 2);
    set_textcolor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("YOU WON!");

    cleanup();
}

void game_over() {
    pause = 1;
    set_pos(COLS / 2, LINES / 2);
    set_textcolor(FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("GAME OVER");

    cleanup();
}

/* Free the memory taken by the linked list */
void cleanup() {
    struct node *ptr_this = snake.next;
    struct node *ptr_next = NULL;

    for (int i = 1; i < snake_len; i++) {
        ptr_next = ptr_this->next;
        free(ptr_this);
        ptr_this = ptr_next;
    }

    snake.next = NULL;
}