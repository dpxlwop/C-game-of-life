#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 25
#define WIDTH 80
#define PIXEL "$"
#define MAX_DELAY 5000
#define MIN_DELAY 9

void input(int **field);
void get_next_frame(int **field, int **next_frame, int *population);
int count_neighbour(int **field, int point_y, int point_x);
void draw_field(int **frame, int **old_frame, int *delay, int *population);

int main() {
    int delay = 160, flag = 1, population = 0;
    int **field = malloc(HEIGHT * sizeof(int *));  // Текущее поле
    for (int i = 0; i < HEIGHT; i++) field[i] = malloc(WIDTH * sizeof(int));
    int **next_frame = malloc(HEIGHT * sizeof(int *));  // Новое поле
    for (int i = 0; i < HEIGHT; i++) next_frame[i] = malloc(WIDTH * sizeof(int));
    input(field);                             // Ввод
    stdin = freopen("/dev/tty", "r", stdin);  // Передача stdin потока обратно в консоль
    initscr();                                // инициализация
    noecho();                                 // не вводить нажатое
    curs_set(0);                              // скрыть курсор
    nodelay(stdscr, TRUE);                    // не ждать enter
    mvprintw(0, 26, "birchibt & procluha presents");
    mvprintw(13, 28, "The Game Of Life");
    mvprintw(25, 7, "Please, make sure your terminal window is 26 height and 80 width.");
    refresh();
    napms(3000);
    draw_field(field, field, &delay, &population);
    while (flag > 0) {
        char command = getch();  // ожидаем команду
        if (command == ' ')
            flag = 0;
        else if (command == 'A' && delay - 50 > MIN_DELAY)
            delay -= 50;
        else if (command == 'Z' && delay + 50 < MAX_DELAY)
            delay += 50;
        get_next_frame(field, next_frame, &population);
        draw_field(next_frame, field, &delay, &population);
        napms(delay);
        population = 0;
    }
    endwin();
    for (int i = 0; i < HEIGHT; i++) {
        free(field[i]);
        free(next_frame[i]);
    }
    free(field);
    free(next_frame);
    return 0;
}

void input(int **field) {
    int num;
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) {
            num = getchar();
            if (num == '\n' || num == ' ') num = getchar();
            field[i][j] = num - '0';
        }
}

void get_next_frame(int **field, int **next_frame, int *population) {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) {
            int neighbours = count_neighbour(field, i, j);
            if (field[i][j] == 1) {
                if (neighbours == 2 || neighbours == 3) {
                    next_frame[i][j] = 1;
                    *population += 1;
                } else
                    next_frame[i][j] = 0;
            } else {
                if (neighbours == 3) {
                    next_frame[i][j] = 1;
                    *population += 1;
                } else
                    next_frame[i][j] = 0;
            }
        }
}

int count_neighbour(int **field, int point_y, int point_x) {
    int neighbours = 0;
    int x, y;
    for (int i = point_y - 1; i <= point_y + 1; i++)
        for (int j = point_x - 1; j <= point_x + 1; j++) {
            x = j;
            y = i;
            if (x < 0)
                x = WIDTH - 1;
            else if (x >= WIDTH)
                x = 0;
            if (y < 0)
                y = HEIGHT - 1;
            else if (y >= HEIGHT)
                y = 0;
            if (!(y == point_y && x == point_x)) {
                if (field[y][x] == 1) {
                    neighbours++;
                }
            }
        }
    return neighbours;
}

void draw_field(int **frame, int **old_frame, int *delay, int *population) {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (frame[i][j] == 0)
                printw(" ");
            else
                printw(PIXEL);
            old_frame[i][j] = frame[i][j];
        }
    }
    mvprintw(25, 0, "Population: %003d cells", *population);
    mvprintw(25, 66, "Delay: %003d ms", *delay);
    refresh();
}