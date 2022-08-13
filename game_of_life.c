#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#define LOWER 0
#define UPPER 22
#define LEFT 0
#define RIGHT 77

int neighbours_count(char **matrix, int row, int column);
void evolution_process(char **matrix);
void rendering(char **matrix);
void getting_status(char **matrix);
void launching_changes(char **matrix);


int main() {
    char **states = malloc((UPPER + 1) * (RIGHT + 1) * sizeof(char) + 
                    (UPPER + 1) * sizeof(char*));
    char *ptr = (char*)(states + (UPPER + 1));
    for (int i = LOWER; i <= UPPER; i++)
        states[i] = ptr + (RIGHT + 1) * i;
    getting_status(states);
    //launching_changes(states);
    free(states);
}

int neighbours_count(char **matrix, int row, int column) {
    int count = 0;
    if (!((row == LOWER || row == UPPER) && (column == LEFT || column == RIGHT))) {
        if (row == LOWER || row == UPPER) // cage on the borders
            count += matrix[abs(row - UPPER)][column - 1] + matrix[abs(row - UPPER)][column] + matrix[abs(row - UPPER)][column + 1] +
                    + matrix[row][column - 1] + matrix[row][column + 1] + matrix[abs(1 - row)][column - 1] + matrix[abs(1 - row)][column] + matrix[abs(1 - row)][column + 1];
        else if (column == LEFT || column == RIGHT)
            count += matrix[row - 1][abs(column - RIGHT)] + matrix[row][abs(column - RIGHT)] + matrix[row + 1][abs(column - RIGHT)] + 
                    + matrix[row - 1][column] + matrix[row + 1][column] + matrix[row - 1][abs(1 - column)] + matrix[row][abs(1 - column)] + matrix[row + 1][abs(1 - column)];
        else // cell inside the field
            count += matrix[row - 1][column] + matrix[row + 1][column] + matrix[row][column - 1] + matrix[row][column + 1] +
                    + matrix[row - 1][column - 1] + matrix[row - 1][column + 1] + matrix[row + 1][column - 1] + matrix[row + 1][column + 1];
    } else { // cell is in the corner
        count += matrix[abs(row - UPPER)][abs(1 - column)] + matrix[abs(row - UPPER)][abs(column - RIGHT)] + matrix[abs(row - UPPER)][column] +
                + matrix[abs(1 - row)][abs(1 - column)] + matrix[abs(1 - row)][column] + matrix[abs(1 - row)][abs(column - RIGHT)] + matrix[row][abs(1 - column)] + matrix[row][abs(column - RIGHT)];
    }
    return count;
}

void evolution_process(char **matrix) {
    for (int row = LOWER; row <= UPPER; row++)
        for (int column = LEFT; column <= RIGHT; column++) {
            int count = neighbours_count(matrix, row, column);
            if (count < 2 || count > 3)
                matrix[row][column] = 0;
            else
                matrix[row][column] = 1;
        }
}

void rendering(char **matrix) {
    for (int row = LOWER - 1; row <= UPPER + 1; row++) {
        for (int column = LEFT - 1; column <= RIGHT + 1; column++) {
            if (row < LOWER || row > UPPER)
                printf("-");
            else if (column < LEFT || column > RIGHT)
                printf("|");
            else {
                if (matrix[row][column])
                    printf("*");
                else
                    printf(".");
            }
        }
        if (row != UPPER)
            printf("\n");
    }
}

void getting_status(char **matrix) {
    for (int row = LOWER; row <= UPPER; row++) {
        for (int column = LEFT; column <= RIGHT; column++) {
            scanf("%c", &(matrix[row][column]));
            printf("%c", matrix[row][column]);
        }
        printf("\n");
    }
    getchar();
}

void launching_changes(char **matrix) {
    evolution_process(matrix);
    rendering(matrix);
    usleep(50000);
    system("cls");
    printf("qqqq");
}
