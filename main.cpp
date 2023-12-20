#include <iostream>
#include <ctime>
#include <omp.h>
#include <iomanip>

const int N = 2;
double A[N][N];
double B[N];
double E[N];

using namespace std;

/// Функция решения уравнений методом Гаусса
void gauss(double A[N][N], double B[N], double E[N], bool isSequential) {

    int i, k;

// Ограничиваем количество потоков
    if (isSequential) {
        omp_set_num_threads(1);
    }

    for (k = 0; k < N - 1; k++) {

#pragma omp parallel for
        for (i = k + 1; i < N; i++) {
            int j;
            double d;
            d = A[i][k] / A[k][k];

            for (j = k; j < N; j++) {
                A[i][j] = A[i][j] - d * A[k][j];
            }
            B[i] = B[i] - d * B[k];
        }
    }

    for (i = N - 1; i >= 0; i--) {
        E[i] = B[i];

        for (int j = i + 1; j < N; j++) {
            E[i] = E[i] - A[i][j] * E[j];
        }
        E[i] = E[i] / A[i][i];
    }
}

/// Вывод матриц
void printMatrix(const double matrix[N][N], const char *matrixName) {
    if (N > 5) {
        return;
    }

    cout << "Матрица " << matrixName << ":\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << setw(3) << matrix[i][j] << " ";
            if (j == N - 1) {
                cout << "\n";
            }
        }
    }
}

/// Вывод векторов в
void printVector(const double vector[N], const char *vectorName) {
    if (N > 5) {
        return;
    }

    cout << vectorName << endl;
    for (int i = 0; i < N; ++i) {
        cout << setw(2) << vector[i] << endl;
    }
}

/// Метод для вызова функций метода Гаусса с подсчетом затраченного времени
void callGaussFunc(double A[N][N], double B[N], double E[N], bool isSequential,
                   void (*multiplyFunc)(double A[N][N], double B[N], double E[N], bool isSequential),
                   const char *funcName) {
    double start = omp_get_wtime();
    gauss(A, B, E, isSequential);
    double end = omp_get_wtime();
    cout << "Метод: " << funcName << ", время выполнения: " << fixed << (end - start) << " секунд" << endl;
}

int main() {
    system("chcp 65001");
    srand(time(NULL));

// Наполнение матриц
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = -10 + rand() % 20;
        }
        B[i] = -10 + rand() % 20;

    }
    cout << "Количество уравнений и переменных в них: " << N << endl;

// Исходная матрица коэффициентов
    printMatrix(A, "Коэффициент");

// Исходный вектор правой части системы уравнений
    printVector(B, "Правый вектор");

// Вызов функции решения методом Гаусса в параллельном режиме
    callGaussFunc(A, B, E, false, gauss, "Concurrent");

    printVector(E, "Вектор решения");

// Вызов функции решения методом Гаусса в последовательном режиме
    callGaussFunc(A, B, E, true, gauss, "Sequential");
    printVector(E, "Вектор решения");
    return 0;


}