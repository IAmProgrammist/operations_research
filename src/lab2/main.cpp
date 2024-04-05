#include <iostream>
#include <iomanip>
#include <array>

#include "../libs/alg/alg.h"

int main() {
    // Инициализируем матрицу и функцию
    std::vector<std::array<double, 7>> matrix = {
    {{{-1}, {3}, {0}, {6}, {0}, {1}, {26}}},
    {{{5}, {1}, {7}, {-3}, {0}, {0}, {14}}},
    {{{2}, {-1}, {0}, {-2}, {1}, {0}, {12}}}};
    std::array<double, 7> function{{{6.0}, {2.0}, {0.0}, {-5.0}, {0.0}, {0.0}, {0.0}}};

    // Вывод ответа
    std::cout << "Zmax: " << solveSimplexMethodMax<7, double>(matrix, function, 0.00000001);
}