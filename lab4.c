/*Написать программу, вводить данные для построения BST, автоматически
подсчитывает количество листьев в поддеревьях каждого узла, и предоставляет
визуализацию построенного дерева"
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_HEIGHT 100
#define SPACING 5
#define NODE_WIDTH 7

typedef struct node {
    int data;
    int leafCount;
    struct node* left;
    struct node* right;
} Node;

Node* newNode(int data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->leafCount = 0;
    node->left = node->right = NULL;
    return node;
}

Node* insert(Node* node, int data) {
    if (node == NULL) return newNode(data);

    if (data < node->data)
        node->left = insert(node->left, data);
    else if (data > node->data)
        node->right = insert(node->right, data);

    return node;
}

int countLeaves(Node* node) {
    if (node == NULL)
        return 0;
    if (node->left == NULL && node->right == NULL)
        return 1; // Узел является листом, возвращаем 1
    return countLeaves(node->left) + countLeaves(node->right);
}


void updateLeafCounts(Node* node) {
    if (node == NULL) 
        return;

    // Обновляем количество листьев в поддеревьях для текущего узла
    if (node->left == NULL && node->right == NULL) {
        // Если узел является листом, его leafCount равен 0
        node->leafCount = 0;
    } else {
        // В противном случае, leafCount равен сумме листьев в поддеревьях
        node->leafCount = (node->left ? countLeaves(node->left) : 0) + 
                          (node->right ? countLeaves(node->right) : 0);
    }

    // Рекурсивно обновляем количество листьев для поддеревьев
    updateLeafCounts(node->left);
    updateLeafCounts(node->right);
}
int maxHeight(Node* node) {
    if (node == NULL) 
        return 0;
    int leftHeight = maxHeight(node->left);
    int rightHeight = maxHeight(node->right);
    return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
}

void printMatrix(Node* root, char matrix[MAX_HEIGHT][MAX_HEIGHT * NODE_WIDTH], int row, int column, int width) {
    if (root == NULL || row >= MAX_HEIGHT || column < 0 || column >= MAX_HEIGHT * NODE_WIDTH) {
        return;
    }
    
    char buffer[NODE_WIDTH];
    snprintf(buffer, NODE_WIDTH, "(%d,%d)", root->data, root->leafCount);
    memcpy(&matrix[row][column], buffer, strlen(buffer));

    printMatrix(root->left, matrix, row + 1, column - width / 2, width / 2);
    printMatrix(root->right, matrix, row + 1, column + width / 2, width / 2);
}

void printTreeMatrix(Node* root) {
    char matrix[MAX_HEIGHT][MAX_HEIGHT * NODE_WIDTH];
    memset(matrix, ' ', sizeof(matrix));

    int height = maxHeight(root);
    int rootPosition = (MAX_HEIGHT * NODE_WIDTH) / 2;

    printMatrix(root, matrix, 0, rootPosition, rootPosition / 2);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < MAX_HEIGHT * NODE_WIDTH; j++) {
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
}

void printTreeRecursive(Node* root, int space) {
    if (root == NULL)
        return;

    space += SPACING;

    printTreeRecursive(root->right, space);

    printf("\n");
    for (int i = SPACING; i < space; i++)
        printf(" ");
    printf("(%d,%d)\n", root->data, root->leafCount);

    printTreeRecursive(root->left, space);
}

void testLeafCount(Node* root) {
    if (root == NULL) return;

    int leftLeaves = countLeaves(root->left);
    int rightLeaves = countLeaves(root->right);

    if (root->leafCount != leftLeaves + rightLeaves) {
        printf("Ошибка в узле с данными: %d. Ожидаемое количество листьев: %d, полученное: %d\n", root->data, leftLeaves + rightLeaves, root->leafCount);
    } else {
        printf("Узел с данными: %d. Количество листьев корректно: %d\n", root->data, root->leafCount);
    }

    testLeafCount(root->left);
    testLeafCount(root->right);
}

int main() {
    setlocale(LC_ALL, "Russian");
    Node* root = NULL;
    int data;
    printf("Введите элементы дерева (0 для завершения):\n");
    while (1) {
        scanf("%d", &data);
        if (data == 0) break;
        root = insert(root, data);
    }

    updateLeafCounts(root);

    printf("\nПостроенное бинарное дерево (матрица):\n");
    printTreeMatrix(root);

    printf("\nПостроенное бинарное дерево (рекурсивно):\n");
    printTreeRecursive(root, 0);

    printf("\nТестирование корректности подсчета листьев:\n");
    testLeafCount(root);

    return 0;
}
