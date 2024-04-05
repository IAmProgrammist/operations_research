#include <iostream>
#include <iomanip>
#include <array>

#include "../../libs/alg/alg.h"

int main() {
    // Подготовить входные данные
    std::vector<std::array<double, 6>> matrix = {
    {2, -1, 1, 3, 1, 15},
    {1, 4, 1, 1, -2, 46},
    {-1, 4, 6, 3, -8, 48}};
    std::array<double, 6> function{{10, -4, 1, 7, -5}};

    // Вывод ответа
    std::cout << solveSystemOfLinearEquationsBigPenalties<6, 3>(matrix, function);
}