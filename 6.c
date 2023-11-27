#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
// Объявление функций для работы с матрицами и векторами (определяются отдельно)

double vec_norm(double *x, int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += x[i] * x[i];
    }
    return sqrt(sum);
}
void vec_subtract(double *x, double *y, double *result, int n) {
    for (int i = 0; i < n; i++) {
        result[i] = x[i] - y[i];
    }
}
void mat_vec_mult(double **A, double *x, double *result, int n) {
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += A[i][j] * x[j];
        }
    }
}
void gauss_elimination(double **A, double *b, double *x, int n, double mu) {
    // Создаем копию матрицы A и модифицируем ее
    double **A_copy = malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        A_copy[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            A_copy[i][j] = A[i][j];
            if (i == j) {
                A_copy[i][j] -= mu;
            }
        }
    }

    // Прямой ход метода Гаусса
    for (int i = 0; i < n; i++) {
        // Обработка отсутствия ведущего элемента
        if (fabs(A_copy[i][i]) < 1e-10) {
            // Код обработки ошибки или выбора другого ведущего элемента
        }

        for (int j = i + 1; j < n; j++) {
            double factor = A_copy[j][i] / A_copy[i][i];
            for (int k = i; k < n; k++) {
                A_copy[j][k] -= factor * A_copy[i][k];
            }
            b[j] -= factor * b[i];
        }
    }

    // Обратный ход метода Гаусса
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A_copy[i][j] * x[j];
        }
        x[i] /= A_copy[i][i];
    }

    // Освобождение памяти
    for (int i = 0; i < n; i++) {
        free(A_copy[i]);
    }
    free(A_copy);
}

// Функция для метода обратных итераций
void inverse_iteration(double **A, double mu, double tol, int max_iter, int n, double *eigenvalue, double *eigenvector) {
    double *x = malloc(n * sizeof(double));
    double *y = malloc(n * sizeof(double));
    int num_iter = 0;

    // Начальное приближение
    for (int i = 0; i < n; i++) {
        x[i] = rand() / (double)RAND_MAX;
    }

    while (num_iter < max_iter) {
        gauss_elimination(A, x, y, n, mu);
        // Нормировка
        double norm = vec_norm(y, n);
        for (int i = 0; i < n; i++) {
            y[i] /= norm;
        }

        // Проверка сходимости
        double *diff = malloc(n * sizeof(double));
        vec_subtract(y, x, diff, n);
        if (vec_norm(diff, n) < tol) {
            break;
        }

        // Обновление x
        for (int i = 0; i < n; i++) {
            x[i] = y[i];
        }

        num_iter++;
        free(diff);
    }

    // Вычисление собственного значения
    double *Ax = malloc(n * sizeof(double));
    mat_vec_mult(A, x, Ax, n);
    *eigenvalue = 0;
    for (int i = 0; i < n; i++) {
        *eigenvalue += x[i] * Ax[i];
    }

    // Копирование собственного вектора
    for (int i = 0; i < n; i++) {
        eigenvector[i] = x[i];
    }

    free(x);
    free(y);
    free(Ax);
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Размер матрицы
    int n = 10;

    // Создание и инициализация матрицы A
    double **A = malloc(n * sizeof(double *));
    double A_data[10][10] = {
        {6.65512447596874, -0.757223405549701, 0.946121225380927, -0.637969733520923, -0.132206130012228, -0.586819192128359, 0.639239283929730, 0.193229240472593, 1.20763521353639, 0.364375565436084},
        {-0.757223405549701, 3.72361529777958, 0.0153997039153746, -0.828497522029239, 0.576338954224539, -0.524792894860845, 1.89089497667446, -0.330472557660537, -0.963533281404710, 1.67271845473335},
        {0.946121225380928, 0.0153997039153747, 6.92944511222610, 0.796863333596113, -1.73806589720041, -0.786420910160635, -0.797960344540710, 1.39671524842696, 0.527072625524791, 1.65536510490239},
        {-0.637969733520924, -0.828497522029239, 0.796863333596113, 5.85661361479601, -0.0318031145879893, -0.648284333415378, 1.02699556587859, 0.365363853738181, -0.0413288274028074, -0.0701730517397538},
        {-0.132206130012228, 0.576338954224539, -1.73806589720041, -0.0318031145879894, 5.49823924920218, 0.466124920171559, 0.636290901564490, 0.952429413026534, 0.235533854749935, 0.128389275576055},
        {-0.586819192128360, -0.524792894860845, -0.786420910160635, -0.648284333415378, 0.466124920171559, 5.64227733657273, -1.35303364744319, -0.961565413296649, -0.207489747106871, -0.0426094329202673},
        {0.639239283929730, 1.89089497667446, -0.797960344540710, 1.02699556587859, 0.636290901564490, -1.35303364744319, 5.61273996115470, 0.193512382657893, -0.152744665924218, 0.386818443061718},
        {0.193229240472593, -0.330472557660537, 1.39671524842696, 0.365363853738181, 0.952429413026534, -0.961565413296649, 0.193512382657893, 3.40823750875179, 1.31707042871064, 1.32370209850222},
        {1.20763521353639, -0.963533281404710, 0.527072625524791, -0.0413288274028074, 0.235533854749935, -0.207489747106871, -0.152744665924218, 1.31707042871064, 5.50733288839874, -1.70852741010789},
        {0.364375565436084, 1.67271845473335, 1.65536510490239, -0.0701730517397539, 0.128389275576055, -0.0426094329202673, 0.386818443061718, 1.32370209850222, -1.70852741010789, 6.16637455514942}
    };

    for (int i = 0; i < n; i++) {
        A[i] = malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            A[i][j] = A_data[i][j];
        }
    }

    // Параметры для метода обратных итераций
    double mu = 0.1;
    double tol = 1e-14;
    int max_iter = 1000;
    double eigenvalue;
    double *eigenvector = malloc(n * sizeof(double));

    // Вызов функции
    inverse_iteration(A, mu, tol, max_iter, n, &eigenvalue, eigenvector);

    // Вывод результатов
    printf("Найденное собственное значение: %lf\n", eigenvalue);
    printf("Соответствующий собственный вектор:\n");
    for (int i = 0; i < n; i++) {
        printf("%lf ", eigenvector[i]);
    }
    printf("\n");
    double tolerances[] = {1e-2, 1e-3, 1e-4, 1e-5, 1e-6};
    int num_tols = sizeof(tolerances) / sizeof(tolerances[0]);

    printf("Точность, Фактическая ошибка, Невязка, Число итераций\n");
    for (int t = 0; t < num_tols; t++) {
        double tol = tolerances[t];
        double eigenvalue;
        double *eigenvector = malloc(n * sizeof(double));

        inverse_iteration(A, mu, tol, max_iter, n, &eigenvalue, eigenvector);

        // Вычисление невязки ||Ax - ?x||
        double *Ax = malloc(n * sizeof(double));
        mat_vec_mult(A, eigenvector, Ax, n);
        double residual_norm = 0;
        for (int i = 0; i < n; i++) {
            residual_norm += pow(Ax[i] - eigenvalue * eigenvector[i], 2);
        }
        residual_norm = sqrt(residual_norm);

        // Печать результатов
        printf("%e, %lf, %lf, %d\n", tol,"/*Фактическая ошибка*/", residual_norm, "/*Число итераций*/");

        free(eigenvector);
        free(Ax);
    }

    // Освобождение памяти
    for (int i = 0; i < n; i++) {
        free(A[i]);
    }
    free(A);
    return 0;
}
