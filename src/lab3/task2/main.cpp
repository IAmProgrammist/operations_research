#include <iostream>
#include <iomanip>
#include <array>

#include "../../libs/alg/alg.h"

int main() {
    // Подготовить входные данные
    std::vector<std::array<Fraction, 6>> matrix;
    matrix.push_back({{{2}, {-1}, {1}, {3}, {1}, {15}}});
    matrix.push_back({{{1}, {4}, {1}, {1}, {-2}, {46}}});
    matrix.push_back({{{-1}, {4}, {6}, {3}, {-8}, {48}}});
    std::array<Fraction, 6> function{{{10}, {-4}, {1}, {7}, {-5}}};

    // Вывод ответа
    auto res = solveSystemOfLinearEquationsBigPenalties<6, 3, Fraction>(matrix, function, Fraction());
    std::cout << res;
}