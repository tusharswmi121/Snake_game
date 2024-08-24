#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 20
#define DELAY 100000
#define INITIAL_LIVES 3

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
int lives;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir, nextDir;

void setup() {
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    lives = INITIAL_LIVES;
    dir = STOP;
    nextDir = STOP;
    nTail = 0;
}

void draw() {
    clear();
    
    for (int i = 0; i < WIDTH + 2; i++)
        mvprintw(0, i, "#");

    for (int i = 0; i < HEIGHT; i++) {
        mvprintw(i + 1, 0, "#");
        for (int j = 0; j < WIDTH; j++) {
            if (i == y && j == x)
                mvprintw(i + 1, j + 1, "O");
            else if (i == fruitY && j == fruitX)
                mvprintw(i + 1, j + 1, "F");
            else {
                int print = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        mvprintw(i + 1, j + 1, "o");
                        print = 1;
                    }
                }
                if (!print)
                    mvprintw(i + 1, j + 1, " ");
            }
        }
        mvprintw(i + 1, WIDTH + 1, "#");
    }

    for (int i = 0; i < WIDTH + 2; i++)
        mvprintw(HEIGHT + 1, i, "#");
    
    mvprintw(HEIGHT + 2, 0, "Score: %d  Lives: %d", score, lives);
    refresh();
}

void input() {
    int ch = getch();
    switch (ch) {
        case 'a':
            if (dir != RIGHT) nextDir = LEFT;
            break;
        case 'd':
            if (dir != LEFT) nextDir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN) nextDir = UP;
            break;
        case 's':
            if (dir != UP) nextDir = DOWN;
            break;
        case 'q':
            dir = STOP;
            break;
    }
}

void logic() {
    dir = nextDir;
    
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }
    
    if (x >= WIDTH) {
        x = 0;
        lives--;
    } else if (x < 0) {
        x = WIDTH - 1;
        lives--;
    }
    if (y >= HEIGHT) {
        y = 0;
        lives--;
    } else if (y < 0) {
        y = HEIGHT - 1;
        lives--;
    }
    
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            lives--;
            if (lives <= 0) {
                endwin();
                printf("Game Over! Final Score: %d\n", score);
                exit(0);
            }
            setup();  // Reset the game state
            return;
        }
    }
    
    if (x == fruitX && y == fruitY) {
        score++;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }
}

int main() {
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(100);  // set input delay

    setup();
    
    while (1) {
        input();
        logic();
        draw();
        usleep(DELAY);
        
        if (lives <= 0) {
            endwin();
            printf("Game Over! Final Score: %d\n", score);
            exit(0);
        }
    }

    endwin();
    return 0;
}
