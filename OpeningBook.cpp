#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

bool gameover;
const int width = 20;
const int height = 10;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirecton {
    STOP = 0, LEFT, RIGHT, UP, DOWN
};
eDirecton dir;

void setup() {
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    gameover = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void draw() {
    clear();
    for (int i = 0; i < width + 2; i++)
        printw("#");
    printw("\n");

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                printw("#"); // Левая граница поля

            if (i == y && j == x)
                printw("O"); // Голова змеи
            else if (i == fruitY && j == fruitX)
                printw("F"); // Фрукт
            else {
                bool printTail = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printw("o"); // Хвост змеи
                        printTail = true;
                    }
                }
                if (!printTail)
                    printw(" ");
            }

            if (j == width - 1)
                printw("#"); // Правая граница поля
        }
        printw("\n");
    }

    for (int i = 0; i < width + 2; i++)
        printw("#");
    printw("\n");

    printw("Score: %d\n", score);
    refresh();
}

void input() {
    keypad(stdscr, TRUE);
    halfdelay(10); // Устанавливаем таймаут ожидания ввода

    int c = getch();
    switch (c) {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameover = true;
            break;
    }
}

void algorithm() {
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

    if (x >= width)
        x = 0;
    else if (x < 0)
        x = width - 1;

    if (y >= height)
        y = 0;
    else if (y < 0)
        y = height - 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameover = true;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main() {
    setup();
    while (!gameover) {
        draw();
        input();
        algorithm();
        usleep(100000); // Пауза для замедления движения змейки
    }

    endwin(); // Завершаем работу с ncurses
    return 0;
}