#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

// some non standard fuckery
#include <conio.h>

// adjust this according for desirable "game speed"
const int FPS = 8;

// size of the grid
const int SIZE = 12;

// linked list implementation
typedef struct Snake {
    int x;
    int y;
    struct Snake *next;
} Snake;

void print_snake(char grid[SIZE][SIZE]) {
    char c;
    for (int i=0; i<(SIZE+2)*2+1; i++) putchar(178);
    putchar(10);
    for (int i=0; i<SIZE; i++) {
        putchar(178);
        putchar(178);
        for (int j=0; j<SIZE; j++) {
            if      (grid[j][i] == 1) c = 64;
            else if (grid[j][i] == 2) c = 153;
            else if (grid[j][i] == 3) c = 254;
            else c = 176;
            putchar(176);
            putchar(c);
            if (j+1==SIZE) putchar(176);
        }
        putchar(178);
        putchar(178);
        putchar(10);
    }
    for (int i=0; i<(SIZE+2)*2+1; i++) putchar(178);
    putchar(10);
}

// 1 = fine
// 0 = gameover
int update_snake(char grid[SIZE][SIZE], Snake *head, int dx, int dy) {
    if (!dx && !dy) return 1;

    int px = head->x;
    int py = head->y;

    if (px + dx < 0 || px + dx >= SIZE) return 0;
    if (py + dy < 0 || py + dy >= SIZE) return 0;
    bool apple = grid[px+dx][py+dy];
    if (grid[px+dx][py+dy] > 1) return 0;

    head->x += dx;
    head->y += dy;

    grid[head->x][head->y] = 2;

    Snake *ptr = head->next;
    while (ptr) {
        grid[px][py] = 3;
        int ppx = ptr->x;
        int ppy = ptr->y;
        ptr->x = px;
        ptr->y = py;
        px = ppx;
        py = ppy;
        ptr = ptr->next;
    }
    if (apple) {
        Snake *tail = head->next;
        while (tail->next)
            tail = tail->next;
        tail->next = malloc(sizeof(Snake));
        tail->next->x = px;
        tail->next->y = py;
        tail->next->next = NULL;

        int ax = px, ay = py;
        while (grid[ax][ay] != 0) {
            ax = rand() % SIZE;
            ay = rand() % SIZE;
        }
        grid[ax][ay] = 1;
    }
    grid[px][py] = apple ? 3 : 0;

    return 1;
}

int main(int argc, char **argv) {
    srand(time(NULL));

    Snake head;
    head.x = (SIZE-1)>>1;
    head.y = (SIZE-1)>>1;
    head.next = NULL;


    Snake *snek = &head;
    for (int i=0; i<3; i++) {
        if (!snek->next) {
            snek->next = malloc(sizeof(Snake));
            snek->next->x = snek->x-1;
            snek->next->y = snek->y;
            snek->next->next = NULL;
            snek = snek->next;
        }
    }
    
    char grid[SIZE][SIZE];
    memset(&grid, 0, SIZE*SIZE);
    // 0 - empty
    // 1 - apple
    // 2 - snake head
    // 3 - snake body
    grid[head.x][head.y] = 2;

    Snake *ptr = head.next;
    while (ptr) {
        grid[ptr->x][ptr->y] = 3;
        ptr = ptr->next;
    }

    int ax, ay = head.y;
    while (ay == head.y) {  
        ax = rand() % SIZE;
        ay = rand() % SIZE;
    }
    grid[ax][ay] = 1;
    
    clock_t last  = clock();
    int dt = (int)floor(1000.0 / FPS);

    int dx = 0;
    int dy = 0;

    bool exit = false;
    while (!exit) {
        if (kbhit()) {
            char key = getch();
            switch (key) {
            case 'q':
                exit = true;
                break;

            case 'w':
                if (dy != 0) break;
                dx = 0;
                dy = -1;
                break;
            case 'a':
                if (dx != 0) break;
                dx = -1;
                dy = 0;
                break;
            case 's':
                if (dy != 0) break;
                dx = 0;
                dy = 1;
                break;
            case 'd':
                if (dx != 0) break;
                dx = 1;
                dy = 0;
                break;
            default:
                break;
            }
        }
        clock_t now = clock();
        if (now - last < dt) continue;
        last = now;

        // yes, i know this is terrible
        system("cls");

        print_snake(grid);
        exit = !update_snake(grid, &head, dx, dy);
        putchar(10);
    }

    return 0;
}
