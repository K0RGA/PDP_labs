#include <iostream>
#include <cmath>
#include <omp.h>

using namespace std;

const double LOWER_LIMIT = 1.0;
const double UPPER_LIMIT = 1000.0;
const double NUM_STEPS = 1;

/// Инициализация функции интеграла
double function(double x) {
    return sqrt(x) / (x + 1);
}

/// Расчет интеграла в последовательном режиме
double calculateIntegral(double lowerLimit, double upperLimit, double numSteps) {
    double stepSize = (upperLimit - lowerLimit) / numSteps; //число разбиений
    double integral = 0.0;
    for (int i = 1; i < stepSize; i++) {
        double x = LOWER_LIMIT + NUM_STEPS * (i - 0.5);
        integral += function(x) * NUM_STEPS;
    }

    return integral;
}

/// Расчет интеграла в параллельном режиме
double calculateIntegralConcurrent(double lowerLimit, double upperLimit, double numSteps) {
    int stepSize = (upperLimit - lowerLimit) / numSteps; //число разбиений
    double integral = 0.0;

#pragma omp parallel for reduction(+:integral)
    for (int i = 1; i < stepSize; i++) {
        double x = LOWER_LIMIT + NUM_STEPS * (i - 0.5);
        integral += function(x) * NUM_STEPS;
    }
    return integral;
}

/// Метод для вызова функций расчета интеграла с подсчетом затраченного времени
void callIntegralFunc(double lower, double upper, double steps,
                      double (*integralFunc)(double lowerLimit, double upperLimit, double numSteps),
                      const char *method) {
    double start = omp_get_wtime();
    double res = integralFunc(lower, upper, steps);
    double end = omp_get_wtime();
    cout << "Пройденное время (" << method << "): " << fixed << end - start << " секунд" << endl;
    cout << "Результат (" << method << "): " << fixed << res << endl;
}

int main() {
    system("chcp 65001");
// Форматирование вывода для отображения бОльшего числа знаков после запятой
    cout.precision(8);
    for (int k = 1; k <= 10000; k *= 10) {
        double numSteps = NUM_STEPS / k;
        cout << "Интервал: " << numSteps << endl;

// Вызов последовательной функции
        callIntegralFunc(LOWER_LIMIT, UPPER_LIMIT, numSteps, calculateIntegral, "sequential");

// Вызов параллельной функции
        callIntegralFunc(LOWER_LIMIT, UPPER_LIMIT, numSteps, calculateIntegralConcurrent, "concurrent");

        cout << endl;
    }

    return 0;
}
