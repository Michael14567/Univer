/*В данной работе требуется Реализовать операцию сложения двух разряженных
матриц, применяя метод хранения матрицы в виде массива списков.*/
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

typedef struct Node {
    int value;
    int index;
    struct Node* next;
} Node;

typedef struct {
    Node** rows;
    int height;
} SparseMatrix;

SparseMatrix* SparseMatrix_create(int height) {
    SparseMatrix* matrix = (SparseMatrix*)malloc(sizeof(SparseMatrix));
    matrix->height = height;
    matrix->rows = (Node**)calloc(height, sizeof(Node*));
    return matrix;
}

void SparseMatrix_read(SparseMatrix* matrix, FILE* file, int width) {
    for (int i = 0; i < matrix->height; i++) {
        Node* tail = NULL;
        for (int j = 0; j < width; j++) {
            int elem;
            fscanf(file, "%d", &elem);
            if (elem) {
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->value = elem;
                newNode->index = j;
                newNode->next = NULL;

                if (tail) {
                    tail->next = newNode;
                    tail = newNode;
                } else {
                    matrix->rows[i] = newNode;
                    tail = newNode;
                }
            }
        }
    }
}

void SparseMatrix_sumAndPrint(const SparseMatrix* first, const SparseMatrix* second, int width) {
    for (int i = 0; i < first->height; i++) {
        Node* a = first->rows[i];
        Node* b = second->rows[i];
        for (int j = 0; j < width; j++) {
            int sum = 0;
            if (a && a->index == j) {
                sum += a->value;
                a = a->next;
            }
            if (b && b->index == j) {
                sum += b->value;
                b = b->next;
            }
            printf("%d ", sum);
        }
        printf("\n");
    }
}

void SparseMatrix_destroy(SparseMatrix* matrix) {
    for (int i = 0; i < matrix->height; i++) {
        Node* row = matrix->rows[i];
        while (row) {
            Node* tmp = row;
            row = row->next;
            free(tmp);
        }
    }
    free(matrix->rows);
    free(matrix);
}

int main() {
    setlocale(LC_ALL, "Russian");
    FILE* file = fopen("matrix.txt", "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return 1;
    }

    int width, height;
    fscanf(file, "%d %d", &width, &height);

    SparseMatrix* firstMatrix = SparseMatrix_create(height);
    SparseMatrix* secondMatrix = SparseMatrix_create(height);

    SparseMatrix_read(firstMatrix, file, width);
    SparseMatrix_read(secondMatrix, file, width);

    SparseMatrix_sumAndPrint(firstMatrix, secondMatrix, width);

    SparseMatrix_destroy(firstMatrix);
    SparseMatrix_destroy(secondMatrix);

    fclose(file);
    return 0;
}