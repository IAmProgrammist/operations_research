#include <iostream>
#include <iomanip>
#include <windows.h>
#include <limits>
#include <algorithm>

#include "../../libs/alg/lab1/task1.tpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    // Инициализируем матрицу
    std::vector<std::vector<double>> matrix = {
    {1,  -4,  8,  9, -3, -1, 87},
    {8,   1, -3,  4,  5,  6, 11},
    {4,   0,  1,  3, -2, -5, 17},
    {-3, -4,  7,  6, -1,  4, 70}};

    // Получаем все базисные решения для матрицы при помощи getAllBasises
    auto res = getAllBasises(matrix);

    // Вводим переменные для хранения максимального значения функции и оптимального плана решения 
    double maxFunctionValue = std::numeric_limits<double>::min();
    std::vector<double> maxB;
    // Задаём функцию z = -3 * x1 + x2 + 121 * x3 - 7 * x6
    std::vector<double> function = {-3, 1, 121, 0, 0, -7};

    // Для всех полученных базисов
    for (auto &basis : res) {
        bool isAllBsMoreOrEqualToZero = true;
        for (int i = 0; i < basis.matrix.size() && isAllBsMoreOrEqualToZero; i++) {
            if (basis.matrix[i].back() < EPS) 
                isAllBsMoreOrEqualToZero = false;
        }

        // Хоть один из br меньше 0?
        if (!isAllBsMoreOrEqualToZero) { 
            // Перейти к следующему базисному решению
            continue;
        }

        double z = 0;
        std::vector<double> B;
        // Для каждого коэффициента в функции z
        for (int i = 0; i < function.size(); i++) {
            // Базисная переменная есть в решении?
            if (std::find(basis.indices.begin(), basis.indices.end(), i) != basis.indices.end()) {
                for (int j = 0; j < basis.matrix.size(); j++) {
                    if (std::abs(basis.matrix[j][i] - 1.0) < EPS) {
                        // Прибавляем значение br, умноженное на коэффициент в функции
                        z += function[i] * basis.matrix[j].back();
                        B.push_back(basis.matrix[j].back());
                        break;
                    }
                }
            } else {
                // Переменная свободная, следовательно равна 0
                B.push_back(0);
            }
        }

        // Полученное значение функции больше сохранённой?
        if (z > maxFunctionValue) {
            // Запоминаем его и базисное решение
            maxFunctionValue = z;
            maxB = B;
        }
    }

    // Выводим значение z и базисное решение
    std::cout << maxFunctionValue << std::endl;
    for (int i = 0; i < maxB.size(); i++) {
        std::cout << maxB[i] << " ";
    }
}