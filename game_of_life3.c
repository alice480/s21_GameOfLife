#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ncurses.h>
#include <unistd.h>
#define LOWER 0
#define UPPER 22
#define LEFT 0
#define RIGHT 77

int neighbours_count(char** matrix, int row, int column);
void evolution_process(char** matrix);
void rendering(char** matrix);
void getting_status(char** matrix);


int main() {
    char** states = malloc((UPPER + 1) * (RIGHT + 1) * sizeof(char) +
        (UPPER + 1) * sizeof(char*));
    char* ptr = (char*)(states + (UPPER + 1));
    for (int i = LOWER; i <= UPPER; i++)
        states[i] = ptr + (RIGHT + 1) * i;
    getting_status(states);
    freopen("/dev/tty", "r", stdin);
    initscr();
    noecho();
    nodelay(stdscr, TRUE);
    char command = ' ';
    int speed = 50000;
    while (command != 'q') {
        command = getch();
        printw("Press Q to exit\n");
        evolution_process(states);
        rendering(states);
        printw("\nDELAY: %d\n", speed);
        refresh();
        if (command == 'a' && 30000 < speed) {
            speed -= 10000;
        } else if (command == 'z' && speed < 250000) {
            speed += 10000;
        }
        usleep(speed);
        clear();
    }
    usleep(500000);
    printw("GAME OVER!");
    free(states);
    endwin();
}

int neighbours_count(char** matrix, int row, int column) {
    int count = 0;
    if (!((row == LOWER || row == UPPER) && (column == LEFT || column == RIGHT))) {
        if (row == LOWER || row == UPPER)  // cage on the borders
            count += matrix[abs(row - UPPER)][column - 1] + matrix[abs(row - UPPER)][column] +
                        matrix[abs(row - UPPER)][column + 1] + matrix[row][column - 1] +
                        matrix[row][column + 1] + matrix[abs(1 - row)][column - 1] +
                        matrix[abs(1 - row)][column] + matrix[abs(1 - row)][column + 1];
        else if (column == LEFT || column == RIGHT)
            count += matrix[row - 1][abs(column - RIGHT)] + matrix[row][abs(column - RIGHT)] +
                        matrix[row + 1][abs(column - RIGHT)] + matrix[row - 1][column] +
                        matrix[row + 1][column] + matrix[row - 1][abs(1 - column)] +
                        matrix[row][abs(1 - column)] + matrix[row + 1][abs(1 - column)];
        else  // cell inside the field
            count += matrix[row - 1][column] + matrix[row + 1][column] + matrix[row][column - 1] +
                        matrix[row][column + 1] + matrix[row - 1][column - 1] + matrix[row - 1][column + 1] +
                        matrix[row + 1][column - 1] + matrix[row + 1][column + 1];
    } else {  // cell is in the corner
        count += matrix[abs(row - UPPER)][abs(1 - column)] + matrix[abs(row - UPPER)][abs(column - RIGHT)] +
                        matrix[abs(row - UPPER)][column] + matrix[abs(1 - row)][abs(1 - column)] +
                        matrix[abs(1 - row)][column] + matrix[abs(1 - row)][abs(column - RIGHT)] +
                        matrix[row][abs(1 - column)] + matrix[row][abs(column - RIGHT)];
    }
    return count;
}

void evolution_process(char** matrix) {
    char** new_states = malloc((UPPER + 1) * (RIGHT + 1) * sizeof(char) +
        (UPPER + 1) * sizeof(char*));
    char* ptr = (char*)(new_states + (UPPER + 1));
    for (int i = LOWER; i <= UPPER; i++)
        new_states[i] = ptr + (RIGHT + 1) * i;
    for (int row = LOWER; row <= UPPER; row++)
        for (int column = LEFT; column <= RIGHT; column++) {
            int count = neighbours_count(matrix, row, column);
            if (matrix[row][column] && (count == 2 || count == 3))
                new_states[row][column] = 1;
            else if (matrix[row][column] && (count < 2 || count > 3))
                new_states[row][column] = 0;
            else if (!matrix[row][column] && count == 3)
                new_states[row][column] = 1;
        }
    for (int row = LOWER; row <= UPPER; row++)
        for (int column = LEFT; column <= RIGHT; column++)
            matrix[row][column] = new_states[row][column];
    free(new_states);
}

void rendering(char** matrix) {
    for (int row = LOWER - 1; row <= UPPER + 1; row++) {
        for (int column = LEFT - 1; column <= RIGHT + 1; column++) {
            if (row < LOWER || row > UPPER) {
                printw("-");
            } else if (column < LEFT || column > RIGHT) {
                printw("|");
            } else {
                if (matrix[row][column])
                    printw("*");
                else
                    printw(".");
            }
        }
        printw("\n");
    }
}

void getting_status(char** matrix) {
    char symbol;
    for (int row = LOWER; row <= UPPER; row++) {
        for (int column = LEFT; column <= RIGHT; column++) {
            scanf("%c", &(symbol));
            if (symbol == '*')
                matrix[row][column] = 1;
            else
                matrix[row][column] = 0;
        }
        getchar();
    }
}
