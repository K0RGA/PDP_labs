#include <iostream>
#include <omp.h>
#include <iomanip>

#define size 3
int A[size][size];
int B[size][size];
int C[size][size];

using namespace std;

/// Функция последовательного умножения матриц
void sequentialMode(int A[][size], int B[][size], int C[][size]) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < size; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/// Функция параллельного умножения матриц
void concurrentMode(int A[][size], int B[][size], int C[][size]) {
#pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < size; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/// Метод для вызова функций перемножения матриц с подсчетом затраченного времени
void matrixMultiplication(int A[][size], int B[][size], int C[][size],
                          void (*multiplyFunc)(int[][size], int[][size], int[][size]), const char *funcName) {
    double start = omp_get_wtime();
    multiplyFunc(A, B, C);
    double end = omp_get_wtime();
    cout << "Функция: " << funcName << ", Время выполнения: " << fixed << (end - start) << " секунд" << endl;
}

void printMatrix(const int matrix[][size], const char *matrixName) {
    if (size > 5) {
        return;
    }

    cout << "Матрица " << matrixName << ":\n";
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << setw(3) << matrix[i][j] << " ";
            if (j == size - 1) {
                cout << "\n";
            }
        }
    }
}

int main() {

    system("chcp 65001");

    // Наполнение матриц значениями от 0 до 9
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }


    cout << "Размер матрицы: [" << size << "," << size << "]" << endl;

// Исходные матрицы
    printMatrix(A, "A");
    printMatrix(B, "B");

// Последовательно
    matrixMultiplication(A, B, C, sequentialMode, "sequentialMode");

// Результат
    printMatrix(C, "C");

// Параллельно
    matrixMultiplication(A, B, C, concurrentMode, "concurrentMode");

// Результат
    printMatrix(C, "C");

    return 0;

}