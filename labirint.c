/*Написать программу, которая реализует алгоритм с возвратом для нахождения пути
в лабиринте.*/

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <locale.h>
#define MAX_SIZE 10

typedef struct {
    int x, y;
} Point;

bool isValid(int maze[MAX_SIZE][MAX_SIZE], int visited[MAX_SIZE][MAX_SIZE], int x, int y) {
    return (x >= 0) && (x < MAX_SIZE) && (y >= 0) && (y < MAX_SIZE) &&
           maze[x][y] == 0 && !visited[x][y];
}

bool findPath(int maze[MAX_SIZE][MAX_SIZE], int visited[MAX_SIZE][MAX_SIZE], int x, int y, int *minPathLen, int pathLen, Point *path, Point *minPath, Point *exitPoint) {
    if (x == 0 || y == 0 || x == MAX_SIZE - 1 || y == MAX_SIZE - 1) {
        if (maze[x][y] == 0 && pathLen < *minPathLen) {
            *minPathLen = pathLen;
            for (int i = 0; i < pathLen; i++) {
                minPath[i] = path[i];
            }
            *exitPoint = (Point){x, y}; // Сохраняем точку выхода
            return true;
        }
        return false;
    }

    bool hasPath = false;
    if (isValid(maze, visited, x, y)) {
        visited[x][y] = 1;
        path[pathLen] = (Point){x, y};
        pathLen++;

        hasPath |= findPath(maze, visited, x + 1, y, minPathLen, pathLen, path, minPath, exitPoint);
        hasPath |= findPath(maze, visited, x, y + 1, minPathLen, pathLen, path, minPath, exitPoint);
        hasPath |= findPath(maze, visited, x - 1, y, minPathLen, pathLen, path, minPath, exitPoint);
        hasPath |= findPath(maze, visited, x, y - 1, minPathLen, pathLen, path, minPath, exitPoint);

        visited[x][y] = 0;
    }
    return hasPath;
}

void printSolution(int maze[MAX_SIZE][MAX_SIZE], Point *minPath, int minPathLen, Point exitPoint) {
    printf("Длина кратчайшего пути: %d\n", minPathLen);
    for (int i = 0; i < minPathLen; i++) {
        maze[minPath[i].x][minPath[i].y] = 2; // Отмечаем путь
    }
    maze[exitPoint.x][exitPoint.y] = 3; // Отмечаем выход 'E'

    for (int x = 0; x < MAX_SIZE; x++) {
        for (int y = 0; y < MAX_SIZE; y++) {
            if (maze[x][y] == 3) {
                printf("E "); // Выход
            } else if (maze[x][y] == 2) {
                printf("* "); // Путь
            } else {
                printf("%d ", maze[x][y]); // Стены и проходы
            }
        }
        printf("\n");
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int maze[MAX_SIZE][MAX_SIZE] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,},
        {1, 0, 1, 0, 1, 0, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 1, 0, 1},
        {1, 1, 1, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 1, 0, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    int visited[MAX_SIZE][MAX_SIZE] = {0};
    int minPathLen = INT_MAX;
    Point path[MAX_SIZE * MAX_SIZE];
    Point minPath[MAX_SIZE * MAX_SIZE];
    Point exitPoint;

    if (findPath(maze, visited, 1, 1, &minPathLen, 0, path, minPath, &exitPoint)) {
        printf("Самый короткий путь до выхода:\n");
        printSolution(maze, minPath, minPathLen, exitPoint);
    } else {
        printf("Путь не найден.\n");
    }

    return 0;
}
