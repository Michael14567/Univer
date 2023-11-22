/*В данной работе требуется считать информацию из уже отсортированного по
количеству рабочих часов файла (при совпадении количества рабочих часов сортировать
список по алфавиту). Затем программа выводит информацию о тех сотрудниках (фамилия
и дата), которые отработали более N часов.
Итак, пользователь нашей программы может делать следующие запросы к
ней:
1) Задать необходимое количество рабочих часов
Завершить программу.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Employee {
    char date[11];
    char name[50];
    int hours_worked;
    struct Employee* next;
};

typedef struct Employee Employee;

Employee* createEmployee(char date[11], char name[50], int hours_worked) {
    Employee* newEmployee = (Employee*)malloc(sizeof(Employee));
    strcpy(newEmployee->date, date);
    strcpy(newEmployee->name, name);
    newEmployee->hours_worked = hours_worked;
    newEmployee->next = NULL;
    return newEmployee;
}

void insertSorted(Employee** head, char date[11], char name[50], int hours_worked) {
    Employee* newEmployee = createEmployee(date, name, hours_worked);

    if (*head == NULL || hours_worked > (*head)->hours_worked ||
        (hours_worked == (*head)->hours_worked && strcmp(name, (*head)->name) < 0)) {
        newEmployee->next = *head;
        *head = newEmployee;
    } else {
        Employee* current = *head;
        while (current->next != NULL && (hours_worked < current->next->hours_worked ||
                                         (hours_worked == current->next->hours_worked &&
                                          strcmp(name, current->next->name) > 0))) {
            current = current->next;
        }
        newEmployee->next = current->next;
        current->next = newEmployee;
    }
}

void displayEmployees(Employee* head) {
    Employee* current = head;
    while (current != NULL) {
        printf("%s %s\n", current->name, current->date);
        current = current->next;
    }
}

void freeEmployees(Employee* head) {
    while (head != NULL) {
        Employee* temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    FILE* file = fopen("employees.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    Employee* head = NULL;
    int N;

    printf("Enter the threshold value (N): ");
    scanf("%d", &N);

    char date[11];
    char name[50];
    int hours_worked;

    while (fscanf(file, "%s %s %d", date, name, &hours_worked) != EOF) {
        insertSorted(&head, date, name, hours_worked);
    }

    fclose(file);

    printf("Employees with  than %d hours worked:\n", N);
    Employee* current = head;
    while (current != NULL) {
        if (current->hours_worked >= N) {
            printf("%s %s\n", current->name, current->date);
        }
        current = current->next;
    }

    freeEmployees(head);

    return 0;
}
