/*Oct-tree
2. ������������ ������� ����������� ������������ � ���������� ������������ �
�������������� ������ Oct-tree*/
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float x, y, z;
    float radius; 
} Point;

typedef struct OctreeNode {
    Point position;     // ������� ���� (����� �������������� ��� �������� ������)
    struct OctreeNode* children[8]; 
} OctreeNode;
OctreeNode* createNode(float x, float y, float z) {
    OctreeNode* node = (OctreeNode*)malloc(sizeof(OctreeNode));
    node->position = (Point){x, y, z};
    for (int i = 0; i < 8; ++i) {
        node->children[i] = NULL;
    }
    return node;
}
int areColliding(Point a, Point b) {
    // �������� �� ������������ �����
    if (a.x == b.x && a.y == b.y && a.z == b.z) {
        return 0; 
    }

    float distanceSquared = (a.x - b.x) * (a.x - b.x) +
                            (a.y - b.y) * (a.y - b.y) +
                            (a.z - b.z) * (a.z - b.z);
    float radiusSum = a.radius + b.radius;
    return distanceSquared <= (radiusSum * radiusSum);
}
void checkCollisions(OctreeNode* node, Point point, void (*collisionCallback)(Point, Point)) {
    if (node == NULL) {
        return;
    }

    // �������� ������������ � ������� �����
    if (areColliding(node->position, point)) {
        collisionCallback(node->position, point);
    }

    // ����������� �������� � �������� �����
    for (int i = 0; i < 8; ++i) {
        checkCollisions(node->children[i], point, collisionCallback);
    }
}
void collisionDetected(Point a, Point b) {
    printf("Collision detected between (%f, %f, %f) and (%f, %f, %f)\n",
           a.x, a.y, a.z, b.x, b.y, b.z);
}
void insert(OctreeNode* node, Point point) {
    if (node == NULL) {
        return;
    }

    int index = 0;
    // ����������� ������� ��� �������
    if (point.x > node->position.x) index += 4;
    if (point.y > node->position.y) index += 2;
    if (point.z > node->position.z) index += 1;

    if (node->children[index] == NULL) {
        // ���� ��������� ���� �� ����������, ������� ���
        node->children[index] = createNode(point.x, point.y, point.z);
        node->children[index]->position = point; // ��������� ������� ������ ����
    } else {
        insert(node->children[index], point);
    }

    // ��������� ������������ ������ ����� ���������� �����
    checkCollisions(node, point, collisionDetected);
}
void freeOctree(OctreeNode* node) {
    if (node == NULL) {
        return;
    }
    for (int i = 0; i < 8; ++i) {
        freeOctree(node->children[i]);
    }
    free(node);
}
void printOctree(OctreeNode* node, int level) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < level; ++i) printf("  "); 

    printf("Node at level %d: (%f, %f, %f)\n", level, node->position.x, node->position.y, node->position.z);
    
    for (int i = 0; i < 8; ++i) {
        printOctree(node->children[i], level + 1);
    }
}

void runCollisionTests() {
    OctreeNode* root = createNode(0, 0, 0);

    // ���������� ����� � ���������
    insert(root, (Point){1, 1, 1, 0.1f});
    insert(root, (Point){0.4, 0.9, 0.9, 0.1f});
    insert(root, (Point){0.4, 0.6, 0.4, 0.1f}); 
    insert(root, (Point){0.6, 0.6, 0.5, 0.1f}); 

    // �������� �� ������������
    printf("Running collision tests:\n");
    checkCollisions(root, (Point){0.6, 0.6, 0.5, 0.1f}, collisionDetected); // ����� ������� ��������� 

    freeOctree(root);
    printf("End collision tests.\n");
}
int main() {
    runCollisionTests();
    printf("You have given all the points, if they are empty, then there is no collision of meanings\n");
    return 0;
}