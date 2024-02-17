#include <iostream>
#include <iomanip>
#include <windows.h>

#include "../../libs/alg/lab1/task1.tpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    // Инициализируем матрицу
    std::vector<std::vector<double>> matrix = {
    {1,  -4,  8,  9, -3, -1, 87},
    {8,   1, -3,  4,  5,  6, 11},
    {4,   0,  1,  3, -2, -5, 17},
    {-3, -4,  7,  6, -1,  4, 70}};;

    // Получаем базисные решения
    auto res = getAllBasises(matrix);

    // Выводим базисные решения
    std::cout << "==================================================================================================================\n";
    for (auto& matrix : res) {
        std::cout << "Выбранные базисные переменные: ";
        for (auto& bas : matrix.indices) {
            std::cout << "x" << (bas + 1) << " ";
        }
        std::cout << "\n\nПолученная система: " << std::endl;


        for (int i = 0; i < matrix.matrix[0].size() - 1; i++) {
            std::stringstream buf;
            buf << "a" << (i + 1);
            std::cout << std::setw(15) << buf.str() << " ";
        }

        std::cout << std::setw(15) << "b" << std::endl;

        for (auto & line : matrix.matrix) {
            for (auto & element : line) {
                std::cout << std::setw(15) << element << " ";
            }

            std::cout << "\n";
        }
        
        std::cout << "==================================================================================================================\n";
    }
}